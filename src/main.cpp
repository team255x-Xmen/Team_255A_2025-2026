#include "main.h"

//Initialize and create the auton manager
AutonManager Kerry; //Creates the manager, named Kerry

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// Chassis constructor
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {-1, 2, -3},     // Left Chassis Ports (negative port will reverse it!)
    {4, -5, 7},  // Right Chassis Ports (negative port will reverse it!)

    7,      // IMU Port
    3.25,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    360);   // Wheel RPM = cartridge * (motor gear / wheel gear)

// Uncomment the trackers you're using here!
// - `8` and `9` are smart ports (making these negative will reverse the sensor)
//  - you should get positive values on the encoders going FORWARD and RIGHT0
// - `2.75` is the wheel diameter
// - `4.0` is the distance from the center of the wheel to the center of the robot
ez::tracking_wheel horiz_tracker(8, 2.75, 1.75);  // This tracking wheel is perpendicular to the drive wheels
// ez::tracking_wheel vert_tracker(9, 2.75, 4.0);   // This tracking wheel is parallel to the drive wheels

bool selectorEnable = true; //The boolean to enable/disable the selector task
std::atomic<bool> secondLock{false}; //An atomic because of multi-threading
//So no data races occur. And the selector updates once per

void screenTouch() { //Function to check where screen is pressed and then feed to auton manager
  if (!secondLock.exchange(true)&&!Kerry.hasTerminated()) { //If prior status was false, run (Not activated then activate).
    pros::screen_touch_status_s_t status = pros::screen::touch_status(); //Two elements. x y
    Kerry.screenTouched(status.x, status.y); //Runs the screen check for everything
    master.print(0, 0, "AS: %-20s", Kerry.selectedAuton()); //Print selected Auton to controller
    pros::delay(50); //Delay so rumble can que
    master.rumble("."); //Rumbles every brain press. So we know when autons might change
    Kerry.store(); //Stores selected
  } //Only runs once until screen is released
}

void screenReleased() {
  secondLock.store(false); //Sets to false (safely across multiple activations)
}

int autonSelector() { //This is what runs the callbacks
  master.print(0, 0, "AS: %-20s", Kerry.selectedAuton()); //When started give starting auton
  while (selectorEnable) {
    pros::screen::touch_callback(screenTouch, TOUCH_PRESSED); //Runs the function
    pros::screen::touch_callback(screenReleased, TOUCH_RELEASED); //Resets for next activation
    /*if (master.get_digital(DIGITAL_L1)&&master.get_digital(DIGITAL_R1)) { //Can only run when not connected to field
      Kerry.store(); //Stores just to make sure
      Kerry.terminateAutons(); //Removes gui of autons. Auton set in stone
      selectorEnable = false; //Stops the loop
    } //Not connected test to make sure the terminate and store work properly*/

    if (Kerry.hasTerminated()) selectorEnable = false; //If terminated elsewhere, end loop
    //Used if autonomous happens before confirmation
    //We don't want this running through the entire program

    pros::delay(20);
  }

  return 1;
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Print our branding over your terminal :D
  //ez::ez_template_print(); //Remove to add in custom stuff!

  pros::delay(500);  // Stop the user from doing anything while legacy ports configure

  // Look at your horizontal tracking wheel and decide if it's in front of the midline of your robot or behind it
  //  - change `back` to `front` if the tracking wheel is in front of the midline
  //  - ignore this if you aren't using a horizontal tracker
  chassis.odom_tracker_back_set(&horiz_tracker);
  // Look at your vertical tracking wheel and decide if it's to the left or right of the center of the robot
  //  - change `left` to `right` if the tracking wheel is to the right of the centerline
  //  - ignore this if you aren't using a vertical tracker
  // chassis.odom_tracker_left_set(&vert_tracker);

  // Configure your chassis controls
  chassis.opcontrol_curve_buttons_toggle(false);   // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0.0);   // Sets the active brake kP. We recommend ~2.  0 will disable.
  chassis.opcontrol_curve_default_set(0.0, 0.0);  // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)

  // Set the drive to your own constants from autons.cpp!
  default_constants();

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);  // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y, pros::E_CONTROLLER_DIGITAL_A);


  //Autons
    //name, blue callback, red callback, (overload) start_selected;
  //The manager switches between the callbacks as necessary
  //The manager also auto-configures sizing, so no need to worry about it anymore

  std::vector<autons> autonPrograms; //Vector to store the autons
  //This will reduce how much code I need to add, and abstracts it
  std::vector<utilAutons> utilityAutons; //Another vector for utility autons

  autonPrograms.push_back(autons("L Simple", simpleLeftSideB, simpleLeftSideR, true)); //Each gets
  autonPrograms.push_back(autons("R Simple", simpleRightSideB, simpleRightSideR));     //added to the vector
  autonPrograms.push_back(autons("L Descore", LeftDescoreB, LeftDescoreR));            //with all the qualities
  autonPrograms.push_back(autons("R Descore", RightDescoreB, RightDescoreR));          //So later the vector
  autonPrograms.push_back(autons("L 2 Goal", LeftDuoAWPB, LeftDuoAWPR));               //Can just be run copied
  autonPrograms.push_back(autons("R 2 Goal", RightDuoAWPB, RightDuoAWPR));             //And reduce lines
  

  //Utilities
  utilityAutons.push_back(utilAutons("Skills", skillsAuton, true)); //And stop me from forgetting stuff
  utilityAutons.push_back(utilAutons("Simple Drive", basicDrive)); //The vectors get auto added when initializing
  utilityAutons.push_back(utilAutons("Odom Diagnostics", measure_offsets));
  utilityAutons.push_back(utilAutons("Simple Odom", simpleOdomDrive));
  //For utilities (single callback autons) they go:
    //name, callback, (overload) is_skills;
  //They are for single callback autons, and get put into the utilities section

  //Initialization

  // Initialize chassis
  chassis.initialize();
  
  //Initializes the manager with the input autons, input utilities, utility bar vertical size (in pixels), and rows (for autons)
  Kerry.initialize(autonPrograms, utilityAutons, 40, 2); //Adds the autons
  pros::Task runSelector(autonSelector); //Run auton selector
  master.rumble(chassis.drive_imu_calibrated() ? "." : "---");
  //At some point add in a color sorter and run the task here (so it runs globally)
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  //...

}

bool autonTerminate = true; //Boolean to control when autonomous terminates

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  if (!Kerry.hasTerminated()) { //If it hasn't happened
    Kerry.store(); //Stores before clearing screen
    if (autonTerminate) { //If it should terminate, then terminate
      Kerry.terminateAutons(); //Remove autons interaction and ui. Reprints photo
    } //Doesn't terminate when manually called. Reset after so functionality works as intended
    autonTerminate = true; //Since manually calling autonomous sets it to false, set it to true so match autons work
  } //Otherwise no need to store empty vector and redraw screen
  //This allows for autonomous to be called multiple times in a single program run

  chassis.pid_targets_reset();                // Resets PID targets to 0
  chassis.drive_imu_reset();                  // Reset gyro position to 0
  chassis.drive_sensor_reset();               // Reset drive sensors to 0
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);    // Set the current position, you can start at a specific position with this
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD);  // Set motors to hold.  This helps autonomous consistency

  /*
  Odometry and Pure Pursuit are not magic

  It is possible to get perfectly consistent results without tracking wheels,
  but it is also possible to have extremely inconsistent results without tracking wheels.
  When you don't use tracking wheels, you need to:
   - avoid wheel slip
   - avoid wheelies
   - avoid throwing momentum around (super harsh turns, like in the example below)
  You can do cool curved motions, but you have to give your robot the best chance
  to be consistent
  */

  Kerry.autonomous(); //Calls selected Auton
}

/**
 * Simplifies printing tracker values to the brain screen
 */
void screen_print_tracker(ez::tracking_wheel *tracker, std::string name, int line) {
  std::string tracker_value = "", tracker_width = "";
  // Check if the tracker exists
  if (tracker != nullptr) {
    tracker_value = name + " tracker: " + util::to_string_with_precision(tracker->get());             // Make text for the tracker value
    tracker_width = "  width: " + util::to_string_with_precision(tracker->distance_to_center_get());  // Make text for the distance to center
  }
  ez::screen_print(tracker_value + tracker_width, line);  // Print final tracker text
}

/**
 * Ez screen task
 * Adding new pages here will let you view them during user control or autonomous
 * and will help you debug problems you're having
 */
void ez_screen_task() {
  while (true) {
    // Only run this when not connected to a competition switch
    if (!pros::competition::is_connected()) {
      // Blank page for odom debugging
      if (chassis.odom_enabled() && !chassis.pid_tuner_enabled()) {
        // If we're on the first blank page...
        if (ez::as::page_blank_is_on(0)) {
          // Display X, Y, and Theta
          ez::screen_print("x: " + util::to_string_with_precision(chassis.odom_x_get()) +
                               "\ny: " + util::to_string_with_precision(chassis.odom_y_get()) +
                               "\na: " + util::to_string_with_precision(chassis.odom_theta_get()),
                           1);  // Don't override the top Page line

          // Display all trackers that are being used
          screen_print_tracker(chassis.odom_tracker_left, "l", 4);
          screen_print_tracker(chassis.odom_tracker_right, "r", 5);
          screen_print_tracker(chassis.odom_tracker_back, "b", 6);
          screen_print_tracker(chassis.odom_tracker_front, "f", 7);
        }
      }
    }

    // Remove all blank pages when connected to a comp switch
    else {
      if (ez::as::page_blank_amount() > 0)
        ez::as::page_blank_remove_all();
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
pros::Task ezScreenTask(ez_screen_task);

/**
 * Gives you some extras to run in your opcontrol:
 * - run your autonomous routine in opcontrol by pressing DOWN and B
 *   - to prevent this from accidentally happening at a competition, this
 *     is only enabled when you're not connected to competition control.
 * - gives you a GUI to change your PID values live by pressing X
 */
void ez_template_extras() {
  // Only run this when not connected to a competition switch
  if (!pros::competition::is_connected()) {
    // PID Tuner
    // - after you find values that you're happy with, you'll have to set them in auton.cpp

    // Enable / Disable PID Tuner
    //  When enabled:
    //  * use A and Y to increment / decrement the constants
    //  * use the arrow keys to navigate the constants
    if (master.get_digital_new_press(DIGITAL_X))
      chassis.pid_tuner_toggle();

    // Trigger the selected autonomous routine
    if (master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
      pros::motor_brake_mode_e_t preference = chassis.drive_brake_get();
      autonTerminate = false; //Set to false before running autonomous
      autonomous();
      chassis.drive_brake_set(preference);
    }

    // Allow PID Tuner to iterate
    chassis.pid_tuner_iterate();
  }

  // Disable PID Tuner when connected to a comp switch
  else {
    if (chassis.pid_tuner_enabled())
      chassis.pid_tuner_disable();
  }
}

bool driveStyleSwitch = false; //false is arcade, true is tank

void driveSwitch() {
  driveStyleSwitch = !driveStyleSwitch; //Switches boolean
  master.print(0, 0, "%-20s", driveStyleSwitch ? "Tank Drive" : "Arcade Drive"); //Prints new style
  pros::delay(50); //Waits so rumble can que
  master.rumble(".");  // Rumble to let the user know the switch happened
}

void displayOdom() { //Displays the current odometry information. For debugging
  pros::screen::set_pen(0xFFFFFF); //Sets color to white
  pros::screen::print(pros::E_TEXT_MEDIUM, 0, 20, "X: %f", chassis.odom_x_get()); //Prints the x pos
  pros::screen::print(pros::E_TEXT_MEDIUM, 0, 50, "Y: %f", chassis.odom_y_get()); //Prints the y pos
  pros::screen::print(pros::E_TEXT_MEDIUM, 0, 80, "T: %f", chassis.odom_theta_get()); //Prints the angle
} //Covers up custom background, so it can only be activated in opcontrol when not connected to comp controller

int calculateQuad() {
  int quad = 0; //Char for quadrant
  double robotX = chassis.odom_x_get(); //Sets to simpler name
  double robotY = chassis.odom_y_get(); //Sets to simpler name

  if (robotX <= 0 && robotY <= 0) quad = 3; //Bottom Left
  if (robotX <= 0 && robotY > 0) quad = 2; //Top Left
  if (robotX > 0 && robotY <= 0) quad = 4; //Bottom Right
  if (robotX > 0 && robotY > 0) quad = 1; //Top Right

  return quad; //Returns the calculated quadrant
}

void motorTemp() {
  pros::screen::set_pen(0xFFFFFF);
  pros::screen::print(pros::E_TEXT_MEDIUM, 150, 80, "Lower Temp: %f", intakeLower.get_temperature());
  pros::screen::print(pros::E_TEXT_MEDIUM, 150, 100, "Upper Temp: %f", intakeUpper.get_temperature());
}

void debugScreen() {
  pros::screen::set_pen(0x0000FF); //Sets color to blue
  pros::screen::fill_rect(0, 0, 480, 240); //Draws a rectangle (entire screen)
  displayOdom(); //Displays current odom
  pros::screen::set_pen(0xFFFFFF); //Sets pen to white
  pros::screen::print(pros::E_TEXT_MEDIUM, 150, 30, "Quadrant: %i", calculateQuad()); //Prints quadrant
  motorTemp();
  //More room to add to the debug screen as I think of it!
}

bool intakeGroup = false;

void intakeGroupToggle() { //Toggles the intake grouping preference
  intakeGroup = !intakeGroup; //Toggles the variable
  master.print(0, 0, "%-28s", intakeGroup ? "Intakes Grouped" : "Intakes Disjoint"); //Prints new style
  pros::delay(50); //Waits so rumble can que
  master.rumble(".");  // Rumble to let the user know the switch happened
}

/*
 * A constant list (enum) that contains the speeds for the intake:
 * -ZERO (0)
 * -FWD (127, full)
 * -REV (-127, full)
*/
enum class stdMotorSpeed {
  ZERO = 0,
  FWD = 127,
  REV = -127
};

int calcIntakeSpeed(bool lower) { //Function that calculates intake speed based on controllers, preferences, and a bool
  stdMotorSpeed speed = stdMotorSpeed::ZERO; //Zero by default. Called speed to set value to
  bool R1 = master.get_digital(DIGITAL_R1); //Temporary boolean for R1 being pressed
  bool R2 = master.get_digital(DIGITAL_R2); //Temporary boolean for R2 being pressed
  bool L1 = master.get_digital(DIGITAL_L1); //Temporary boolean for L1 being pressed
  bool L2 = master.get_digital(DIGITAL_L2); //Temporary boolean for L2 being pressed

  if (!intakeGroup) { //If not grouped
    if (R1&&lower) { //Then if trying to move lower intake (and is lower intake)
      lower ? (speed = stdMotorSpeed::FWD, 1) : 1; //Set speed to full forward
    } else if (R2&&lower) { //Same but for reverse
      lower ? (speed = stdMotorSpeed::REV, 1) : 1; //Sets to reverse
    } else if (L1&&!lower) { //Checks if trying to move upper
      lower ? 1 : (speed = stdMotorSpeed::FWD, 1); //Sets to full speed
    } else if (L2&&!lower) { //If trying to reverse
      lower ? 1 : (speed = stdMotorSpeed::REV, 1); //Set to reverse
    }
  } else { //When supposed to be grouped
    if (R1) speed = stdMotorSpeed::FWD; //Sets to full forward
    if (R2) speed = stdMotorSpeed::REV; //Sets to full reverse
    if (L1) speed = stdMotorSpeed::FWD; //Same but for other side
    if (L2) speed = stdMotorSpeed::REV; //Same but for other set of buttons
  }

  return static_cast<int>(speed); //Returns what speed is. It has to be converted to an integer so it returns properly (doesn't default)
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
  // This is preference to what you like to drive on
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  //chassis.opcontrol_joystick_practicemode_toggle(true); //Comment or remove at comps
  if (Kerry.skills_is_selected()) chassis.odom_xyt_set(66.75_in, -17.75_in, -90_deg); //Sets position if skills (for driver control)
  //Change based on where skills will start

  while (true) {
    // Gives you some extras to make EZ-Template ezier
    ez_template_extras();

    if (driveStyleSwitch) {
    chassis.opcontrol_tank();  // Tank control
    } else {
    chassis.opcontrol_arcade_standard(ez::SPLIT); // Standard split arcade
    }
    // chassis.opcontrol_arcade_standard(ez::SINGLE);  // Standard single arcade
    // chassis.opcontrol_arcade_flipped(ez::SPLIT);    // Flipped split arcade
    // chassis.opcontrol_arcade_flipped(ez::SINGLE);   // Flipped single arcade

    // . . .
    // Put more user control code here!
    // . . .

    if (master.get_digital_new_press(DIGITAL_LEFT)) {
      intakeGroupToggle(); //Runs toggle
      if (!pros::competition::is_connected()) { //Also checks if not connected
        debugScreen(); //And then prints debug screen
      }
    }

    if (master.get_digital_new_press(DIGITAL_B)) {
      driveSwitch();
    }

    if (master.get_digital_new_press(DIGITAL_A)) {
      odomScore(); //Run score task
    }

    if (master.get_digital_new_press(DIGITAL_Y)) {
      odomMatchLoad(); //Run match load setup
    }

    if (master.get_digital_new_press(DIGITAL_X)) {
      odomCloseScore(); //Moves to scoring pos (when close to goal already)
    }

    if (master.get_digital_new_press(DIGITAL_UP)) {
      lockPiston.set(!lockPiston.get());
    }

    if (master.get_digital_new_press(DIGITAL_RIGHT)) {
      descorePiston.set(!descorePiston.get());
    }

    //New press is every click

    if (master.get_digital_new_press(DIGITAL_DOWN)) {
      matchLoadPistons.set(!matchLoadPistons.get());  //Piston toggle
    }

    intakeLower.move(calcIntakeSpeed(true));
    intakeUpper.move(calcIntakeSpeed(false));

    pros::delay(ez::util::DELAY_TIME);  // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
