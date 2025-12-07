#include "main.h"
#include "menu.h"

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

    9,      // IMU Port
    3.5,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    320);   // Wheel RPM = cartridge * (motor gear / wheel gear)

// Uncomment the trackers you're using here!
// - `8` and `9` are smart ports (making these negative will reverse the sensor)
//  - you should get positive values on the encoders going FORWARD and RIGHT0
// - `2.75` is the wheel diameter
// - `4.0` is the distance from the center of the wheel to the center of the robot
ez::tracking_wheel horiz_tracker(8, 2.75, 0.5);  // This tracking wheel is perpendicular to the drive wheels
// ez::tracking_wheel vert_tracker(9, 2.75, 4.0);   // This tracking wheel is parallel to the drive wheels

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
  chassis.opcontrol_curve_buttons_toggle(true);   // Enables modifying the controller curve with buttons on the joysticks
  chassis.opcontrol_drive_activebrake_set(0.0);   // Sets the active brake kP. We recommend ~2.  0 will disable.
  //chassis.opcontrol_curve_default_set(0.0, 0.0);  // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)

  // Set the drive to your own constants from autons.cpp!
  default_constants();

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);  // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y, pros::E_CONTROLLER_DIGITAL_A);


  //Autons
    //name, left bound, right bound, bottom bound, top bound, callback
  
  autons skills("Skills Auton", 4, 240, 240, 20, skillsAuton);


  // Autonomous Selector using LLEMU
  /*ez::as::auton_selector.autons_add({
      {"Simple Left side\n\nA basic left side auton to score 3-4 balls in the big goal", simpleLeftSide},
      {"Simple Right Side\n\nA basic right side auton to score 3-4 balls in the big goal", simpleRightSide},
      {"Left Solo AWP\n\nAn autonomous for the left side to score the solo AWP\n\nUnfinished", LeftSoloAWP},
      {"Right Solo AWP\n\nAn autonomous for the right side to score the solo AWP\n\nUnfinished", RightSoloAWP},
      {"Left Duo AWP\n\nAn autonomous for the left side to match load", LeftDuoAWP},
      {"Right Duo AWP\n\nAn autonomous for the right side to match load", RightDuoAWP},
      {"Skills Auton\n\nA full skills auton that scores as many points as possible", skillsAuton}
  });*/


  // Initialize chassis
  chassis.initialize();
  //ez::as::initialize(); //Auton Selector init

  //Adding autons to manager
  Kerry.add(skills);

  
  //After adding initialize manager
  Kerry.printAutons(); //Set up screen after adding autons
  master.rumble(chassis.drive_imu_calibrated() ? "." : "---");
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // . . .
}

void screenTouch() { //Function to check where screen is pressed and then feed to auton manager
  pros::screen_touch_status_s_t status = pros::screen::touch_status(); //Two elements. x y
  Kerry.screenTouched(status.x, status.y);
  master.clear_line(3);
  master.print(3, 1, Kerry.selectedAuton().c_str()); //Prints current auton selected to screen
  master.rumble("..");
  //selectedAuton gets converted to a char string. How it works
  Kerry.store(); //Stores selected
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
  // . . .
  pros::screen::touch_callback(screenTouch, TOUCH_PRESSED); //Runs the function
  if (master.get_digital(DIGITAL_L1)&&master.get_digital(DIGITAL_R1)) {
    Kerry.store(); //Stores just to make sure
    Kerry.terminateAutons(); //Removes gui of autons. Auton set in stone
  }
}

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
  Kerry.store(); //Stores before clearing screen
  Kerry.terminateAutons(); //Remove autons interaction and ui. Reprints photo
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

  //ez::as::auton_selector.selected_auton_call();  // Calls selected auton from autonomous selector
  Kerry.runSelectedAuton(); //Run selected auton
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
  bool driveStyleSwitch = false; //false is arcade, true is tank

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

    if (master.get_digital(DIGITAL_R1)) { //Lower Intake
      intakeLower.move(127);
    } else if (master.get_digital(DIGITAL_R2)) {
      intakeLower.move(-127);
    } else intakeLower.move(0);
    
    if (master.get_digital(DIGITAL_L1)) { //Upper Intake
      intakeUpper.move(127);
    } else if (master.get_digital(DIGITAL_L2)) {
      intakeUpper.move(-127);
    } else intakeUpper.move(0);

    if (master.get_digital_new_press(DIGITAL_B)) {
      driveStyleSwitch = !driveStyleSwitch;
      master.rumble(".");  // Rumble to let the user know the switch happened
      master.clear_line(3); //Clear bottom line
      if (driveStyleSwitch) { //Print current drive style
        master.print(3, 1, "Tank Drive");
      } else {
        master.print(3, 1, "Arcade Drive");
      }
    }

    //New press is every click

    if (master.get_digital_new_press(DIGITAL_DOWN)) {
      matchLoadPistons.set(!matchLoadPistons.get());  //Piston toggle
    }


    pros::delay(ez::util::DELAY_TIME);  // This is used for timer calculations!  Keep this ez::util::DELAY_TIME
  }
}
