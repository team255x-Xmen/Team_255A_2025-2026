#include "main.h"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int DRIVE_SPEED = 110;
const int TURN_SPEED = 90;
const int SWING_SPEED = 110;

///
// Constants
///
void default_constants() {
  // P, I, D, and Start I
  chassis.pid_drive_constants_set(24, 0.002, 100.5);         // Fwd/rev constants, used for odom and non odom motions
  chassis.pid_heading_constants_set(11.0, 0.0, 20.0);        // Holds the robot straight while going forward without odom
  chassis.pid_turn_constants_set(4.60, 0.05, 28.5, 15.0);     // Turn in place constants
  chassis.pid_swing_constants_set(6.0, 0.0, 65.0);           // Swing constants
  chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);    // Angular control for odom motions
  chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

  //Top 4 are what to tune

  // Exit conditions
  chassis.pid_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);
  chassis.pid_odom_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 750_ms);
  chassis.pid_odom_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 750_ms);
  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  // Slew constants
  chassis.slew_turn_constants_set(3_deg, 70);
  chassis.slew_drive_constants_set(3_in, 70);
  chassis.slew_swing_constants_set(3_in, 80);

  // The amount that turns are prioritized over driving in odom motions
  // - if you have tracking wheels, you can run this higher.  1.0 is the max
  chassis.odom_turn_bias_set(0.9);

  chassis.odom_look_ahead_set(7_in);           // This is how far ahead in the path the robot looks at
  chassis.odom_boomerang_distance_set(16_in);  // This sets the maximum distance away from target that the carrot point can be
  chassis.odom_boomerang_dlead_set(0.625);     // This handles how aggressive the end of boomerang motions are

  chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
}

float turn(float angle) { //Formula for absolute turns. Input angle for red side and it will swap when blue is selected
  selectedIsBlue ? (angle += 180) : 1; //Since I'm developing on red, blue should be offset by half a rotation (180 deg)
  return angle;
}

float descoreOdomTableL(int index, bool x) {
  bool blue = selectedIsBlue;
  float point = 0;

  switch (index) {
    case 0:
    if (x) {
      point = blue ? 22 : -22;
    } else {
      point = blue ? -60 : 60;
    }
    break;
    case 1:
    if (x) {
      point = blue ? 15 : -15;
    } else {
      point = blue ? -60 : 60;
    }
    break;
  }

  return point;
}

float descoreOdomTableR(int index, bool x) {
  float point = descoreOdomTableL(index, x);
  if (!x) {
    point = -point;
    point += selectedIsBlue ? -22 : 22;
  }
  return point;
}

float goal2TableL(int index, bool x) {
  float point = 0;
  bool blue = selectedIsBlue;

  switch (index) {
    case 0:
    if (x) {
      point = blue ? 27 : -27;
    } else point = blue ? -17 : 17;
    break;
    case 1:
    if (x) {
      point = blue ? 19 : -19;
    } else point = blue ? -14 : 14;
    break;
    case 2:
    if (x) {
      point = blue ? 33 : -33;
    } else point = blue ? -18 : 18;
    break;
    case 3:
    if (x) {
      point = blue ? 43 : -43;
    } else point = blue ? -44 : 44;
    break;
  }

  return point;
}

float goal2TableR(int index, bool x) {
  float point = goal2TableL(index, x);
  if (!x) {
    point = -point;
  }
  return point;
}


void upperShimmy() { //Run to set ball lock to false
  intakeUpper.move(-127);
  pros::delay(100);
  lockPiston.set(false);
  pros::delay(50);
  intakeUpper.move(127);
}

//Driver Control or other motor custom PID Example
//ez::PID functionName(kP, kI, kD, startingTotal, "Name");
//Motor.move(Name, compute_error(target - avgPos, avgPos));

//Wait quick is better, but overshoot is same amount of time.

void simpleLeft() { //Put full left side code here
  chassis.pid_drive_set(14_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(turn(65), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(8_in, 25, false);
  intakeLower.move(127);
  intakeUpper.move(20);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(4_in, 30, false);
  chassis.pid_wait_quick();
  pros::delay(300);
  chassis.pid_turn_set(turn(-30), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(32_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(turn(270), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-11_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(200);
  intakeUpper.move(127);
  pros::delay(3000);
  chassis.pid_drive_set(5_in, 60, false);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-7_in, DRIVE_SPEED, false);
  chassis.pid_wait_quick();
}


void simpleRight() { //Put full code here
  chassis.pid_drive_set(14_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(turn(114), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(8_in, 25, false);
  intakeLower.move(127);
  intakeUpper.move(20);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(4_in, 30, false);
  chassis.pid_wait_quick();
  pros::delay(300);
  chassis.pid_turn_set(turn(210), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(29_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(turn(270), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-11_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(200);
  intakeUpper.move(127);
  pros::delay(3000);
  chassis.pid_drive_set(5_in, 60, false);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-7_in, DRIVE_SPEED, false);
  chassis.pid_wait_quick();
}


void leftDuo() { //Full code
  chassis.pid_drive_set(14_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(turn(65), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(8_in, 25, false);
  intakeLower.move(127);
  intakeUpper.move(20);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(10_in, 30, false);
  chassis.pid_wait_quick();
  pros::delay(300);
  chassis.pid_turn_set(turn(-45), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-13_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(70);
  intakeLower.move(0);
  pros::delay(500);
  intakeLower.move(127);
  intakeUpper.move(20);
  chassis.pid_drive_set(46_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(turn(-90), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-17_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(100);
  intakeUpper.move(127);
  pros::delay(3000);
  chassis.pid_drive_set(5_in, 60, false);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-7_in, DRIVE_SPEED, false);
  chassis.pid_wait_quick();
}


void rightDuo() { //I think you get the drill at this point
  chassis.pid_drive_set(14_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(turn(110), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(8_in, 25, false);
  intakeLower.move(127);
  intakeUpper.move(20);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(6_in, 30, false);
  chassis.pid_wait_quick();
  pros::delay(300);
  chassis.pid_turn_set(turn(45), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(11_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeLower.move(-127);
  pros::delay(800);
  intakeLower.move(127);
  chassis.pid_drive_set(-47_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(turn(-90), TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-20_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(100);
  intakeUpper.move(127);
  pros::delay(3000);
  chassis.pid_drive_set(5_in, 60, false);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-7_in, DRIVE_SPEED, false);
  chassis.pid_wait_quick();
}


void LeftDescore() {
  chassis.pid_drive_set(14_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(-31_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  intakeLower.move(127);
  intakeUpper.move(70);
  lockPiston.set(true);
  chassis.pid_drive_set(10_in, 30, false);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait_quick();
  pros::delay(500);
  chassis.pid_turn_relative_set(-86_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(28_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(-64_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  /*chassis.pid_drive_set(16_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(1400);*/
  chassis.pid_drive_set(-14_in, DRIVE_SPEED, true);
  intakeUpper.move(-50);
  lockPiston.set(false);
  chassis.pid_wait_until(10_in);
  chassis.pid_wait_quick();
  descorePiston.set(true);
  matchLoadPistons.set(false);
  intakeUpper.move(127);
  pros::delay(1100);
  chassis.pid_swing_set(ez::LEFT_SWING, turn(90), 70);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(4_in, 30, false);
  chassis.pid_wait_quick();
  chassis.pid_odom_set({{{descoreOdomTableL(0, true), descoreOdomTableL(0, false)}, fwd, 60},
                        {{descoreOdomTableL(1, true), descoreOdomTableL(1, false)}, fwd, 40}}, true);
  descorePiston.set(false);
  chassis.pid_wait_quick();
  descorePiston.set(true);
}


void RightDescore() {
  chassis.pid_drive_set(14_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(23_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  intakeLower.move(127);
  intakeUpper.move(10);
  lockPiston.set(true);
  chassis.pid_drive_set(12_in, 30, false);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait_quick();
  pros::delay(500);
  chassis.pid_turn_relative_set(94_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(28_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(64_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  /*chassis.pid_drive_set(16_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(1400);*/
  chassis.pid_drive_set(-9_in, DRIVE_SPEED, true);
  intakeUpper.move(-60);
  lockPiston.set(false);
  chassis.pid_wait_until(10_in);
  chassis.pid_wait_quick();
  descorePiston.set(true);
  matchLoadPistons.set(false);
  intakeUpper.move(127);
  pros::delay(1700);
  chassis.pid_swing_set(ez::LEFT_SWING, turn(90), 70);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(4_in, 30, false);
  chassis.pid_wait_quick();
  chassis.pid_odom_set({{{descoreOdomTableR(0, true), descoreOdomTableR(0, false)}, fwd, 60},
                        {{descoreOdomTableR(1, true), descoreOdomTableR(1, false)}, fwd, 40}}, true);
  descorePiston.set(false);
  chassis.pid_wait_quick();
  descorePiston.set(true);
}



void leftB() { //Left blue coordinate setter
  chassis.odom_xyt_set(57_in, -17.75_in, -90_deg);
}

void leftR() { //Left red coordinate setter
  chassis.odom_xyt_set(-57_in, 17.75_in, 90_deg);
}

void rightB() { //Right blue coordinate setter
  chassis.odom_xyt_set(57_in, 17.75_in, -90_deg);
}

void rightR() { //Right red coordinate setter
  chassis.odom_xyt_set(-57_in, -17.75_in, 90_deg);
}

void simpleLeftSideB() { //Only use for setting odom xyt
  leftB();
  simpleLeft(); //Runs the actual (both sides can run the same)
  //As long as no odom is used
  //This is just for coordinate setting
}



void simpleLeftSideR() { //Only use for setting odom xyt
  leftR();
  simpleLeft();
}



void simpleRightSideB() { //Only use for setting odom xyt
  rightB();
  simpleRight();
}



void simpleRightSideR() { //Only use for setting odom xyt
  rightR();
  simpleRight();
}


void LeftDuoAWPB() { //Only use for setting odom xyt
  leftB();
  leftDuo();
}



void LeftDuoAWPR() { //Only use for setting odom xyt
  leftR();
  leftDuo();
}



void RightDuoAWPB() { //Only use for setting odom xyt
  rightB();
  rightDuo();
}



void RightDuoAWPR() { //Only use for setting odom xyt
  rightR();
  rightDuo();
}


void LeftDescoreB() {
  leftB();
  LeftDescore();
}


void RightDescoreB() {
  rightB();
  RightDescore();
}


void LeftDescoreR() {
  leftR();
  LeftDescore();
}

void RightDescoreR() {
  rightR();
  RightDescore();
}

void soloAWPB() {
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);

  
}

void soloAWPR() {
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);


}

void skillsAuton() { // I want to eventually be using odom for this
  chassis.odom_theta_set(180_deg);

  chassis.pid_drive_set(28_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();

  matchLoadPistons.set(true);
  chassis.pid_turn_set(-90_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();

  chassis.pid_drive_set(2_in, DRIVE_SPEED, false);
  chassis.pid_wait_quick();
  intakeLower.move(127);
  intakeUpper.move(60);
  pros::delay(1000);

  chassis.pid_drive_set(-25_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(100);

  intakeUpper.move(127);
  pros::delay(700);

  chassis.pid_drive_set(4_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(220_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(17_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-90.5_deg, TURN_SPEED, true);
  matchLoadPistons.set(false);
  chassis.pid_wait_quick();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(-8_deg, TURN_SPEED, false);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-34_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(12_in);
  descorePiston.set(true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-80_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-22_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();

  chassis.pid_turn_set(45_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(16_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(90_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(6_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(70);
  pros::delay(1000);

  chassis.pid_drive_set(-26_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(100);
  intakeUpper.move(127);
  pros::delay(800);

  intakeUpper.move(60);
  chassis.pid_drive_set(5_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(0_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(93_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(90_deg, TURN_SPEED, true);
  matchLoadPistons.set(true);
  chassis.pid_wait_quick();

  chassis.pid_drive_set(16_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(70);
  pros::delay(1000);
  chassis.pid_drive_set(-27.5_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(100);
  intakeUpper.move(127);
  pros::delay(800);

  chassis.pid_drive_set(4_in, DRIVE_SPEED, true);
  matchLoadPistons.set(false);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(45_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(16_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(90_deg, TURN_SPEED, true);
  intakeUpper.move(60);
  chassis.pid_wait_quick();

  chassis.pid_drive_set(-25_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(-8_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  descorePiston.set(false);
  chassis.pid_drive_set(-34_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(12_in);
  descorePiston.set(true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(100_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-22_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-135_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();

  chassis.pid_drive_set(16_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-90_deg, TURN_SPEED, true);
  matchLoadPistons.set(true);
  chassis.pid_wait_quick();

  chassis.pid_drive_set(6_in, DRIVE_SPEED, true);
  intakeUpper.move(70);
  chassis.pid_wait_quick();
  pros::delay(1000);

  chassis.pid_drive_set(-26_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(100);
  intakeUpper.move(127);
  pros::delay(800);

  chassis.odom_theta_set(-90_deg);
  matchLoadPistons.set(false);
  chassis.pid_drive_set(6_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-180_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  
  chassis.pid_drive_set(46_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-90_deg, 50, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(9_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(40_in, 127, false);
  chassis.pid_wait_quick();
}

void basicDrive() {
  chassis.pid_drive_set(3_in, 40, false); //Moves forward a little bit
  chassis.pid_wait(); //So motion can complete
}

void measure_offsets() {
  // Number of times to test
  int iterations = 10;

  // Our final offsets
  double l_offset = 0.0, r_offset = 0.0, b_offset = 0.0, f_offset = 0.0;

  // Reset all trackers if they exist
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->reset();
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->reset();
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->reset();
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->reset();
  
  for (int i = 0; i < iterations; i++) {
    // Reset pid targets and get ready for running an auton
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    chassis.odom_xyt_set(0_in, 0_in, 0_deg);
    double imu_start = chassis.odom_theta_get();
    double target = i % 2 == 0 ? 90 : 270;  // Switch the turn target every run from 270 to 90

    // Turn to target at half power
    chassis.pid_turn_set(target, 63, ez::raw);
    chassis.pid_wait();
    pros::delay(250);

    // Calculate delta in angle
    double t_delta = util::to_rad(fabs(util::wrap_angle(chassis.odom_theta_get() - imu_start)));

    // Calculate delta in sensor values that exist
    double l_delta = chassis.odom_tracker_left != nullptr ? chassis.odom_tracker_left->get() : 0.0;
    double r_delta = chassis.odom_tracker_right != nullptr ? chassis.odom_tracker_right->get() : 0.0;
    double b_delta = chassis.odom_tracker_back != nullptr ? chassis.odom_tracker_back->get() : 0.0;
    double f_delta = chassis.odom_tracker_front != nullptr ? chassis.odom_tracker_front->get() : 0.0;

    // Calculate the radius that the robot traveled
    l_offset += l_delta / t_delta;
    r_offset += r_delta / t_delta;
    b_offset += b_delta / t_delta;
    f_offset += f_delta / t_delta;
  }

  // Average all offsets
  l_offset /= iterations;
  r_offset /= iterations;
  b_offset /= iterations;
  f_offset /= iterations;

  // Set new offsets to trackers that exist
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->distance_to_center_set(l_offset);
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->distance_to_center_set(r_offset);
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->distance_to_center_set(b_offset);
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->distance_to_center_set(f_offset);
}

void simpleOdomDrive() {
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);
  chassis.pid_odom_set({{0_in, 12_in}, fwd, DRIVE_SPEED}, false);
  chassis.pid_wait();
}

void simpleTurn() {
  chassis.pid_turn_set(90_deg, TURN_SPEED, false);
  chassis.pid_wait();
}

void printTable() {
  pros::screen::print(pros::E_TEXT_MEDIUM, 10, 1, "Point 0 X: %f", goal2TableL(0, true));
  pros::screen::print(pros::E_TEXT_MEDIUM, 10, 30, "Point 0 Y: %f", goal2TableL(0, false));

  pros::screen::print(pros::E_TEXT_MEDIUM, 10, 60, "Point 1 X: %f", goal2TableL(1, true));
  pros::screen::print(pros::E_TEXT_MEDIUM, 10, 90, "Point 1 Y: %f", goal2TableL(1, false));
}