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
  selectedIsBlue ? (angle = -angle) : 1; //Since I'm developing on red, blue should be swapped
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
      point = blue ? 17 : -17;
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
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(-31_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  intakeLower.move(127);
  intakeUpper.move(70);
  lockPiston.set(true);
  chassis.pid_drive_set(14_in, 20, false);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait_quick();
  pros::delay(500);
  chassis.pid_turn_relative_set(-86_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(27_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(-63_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-14_in, DRIVE_SPEED, true);
  intakeUpper.move(-50);
  pros::delay(300);
  lockPiston.set(false);
  chassis.pid_wait_until(2_in);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
}


void simpleRight() { //Put full code here
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(31_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  intakeLower.move(127);
  intakeUpper.move(70);
  lockPiston.set(true);
  chassis.pid_drive_set(14_in, 20, false);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait_quick();
  pros::delay(500);
  chassis.pid_turn_relative_set(86_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(27_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(63_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-16_in, DRIVE_SPEED, true);
  intakeUpper.move(-50);
  lockPiston.set(false);
  pros::delay(300);
  chassis.pid_wait_until(2_in);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
}


void leftDuo() { //Full code
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(-31_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  intakeLower.move(127);
  intakeUpper.move(70);
  lockPiston.set(true);
  chassis.pid_drive_set(16_in, 30, false);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait_quick();
  pros::delay(500);
  chassis.pid_turn_relative_set(-105_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-18_in, DRIVE_SPEED, true);
  intakeUpper.move(-50);
  pros::delay(300);
  lockPiston.set(false);
  intakeUpper.move(70);
  chassis.pid_wait_quick();
  pros::delay(1100);
  chassis.pid_drive_set(49_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  lockPiston.set(true);
  intakeUpper.move(-60);
  chassis.pid_turn_relative_set(-46_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  /*chassis.pid_drive_set(13_in, DRIVE_SPEED, false);
  chassis.pid_wait_quick();
  pros::delay(600);*/
  chassis.pid_drive_set(-23_in, DRIVE_SPEED, true);
  intakeUpper.move(-50);
  lockPiston.set(false);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
}


void rightDuo() { //I think you get the drill at this point
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(31_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  intakeLower.move(127);
  intakeUpper.move(70);
  lockPiston.set(true);
  chassis.pid_drive_set(16_in, 30, false);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait_quick();
  pros::delay(500);
  chassis.pid_turn_relative_set(105_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(-18_in, DRIVE_SPEED, true);
  lockPiston.set(false);
  intakeUpper.move(-50);
  pros::delay(100);
  intakeUpper.move(70);
  chassis.pid_wait_quick();
  pros::delay(1100);
  chassis.pid_drive_set(49_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(-60);
  lockPiston.set(true);
  chassis.pid_turn_relative_set(46_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  /*chassis.pid_drive_set(13_in, DRIVE_SPEED, false);
  chassis.pid_wait_quick();
  pros::delay(600);*/
  chassis.pid_drive_set(-23_in, DRIVE_SPEED, true);
  intakeUpper.move(-50);
  lockPiston.set(false);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
}


void LeftDescore() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
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
  master.print(0, 0, "Y: %f", descoreOdomTableR(0, false));
  chassis.pid_drive_set(26_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(23_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  intakeLower.move(127);
  intakeUpper.move(70);
  lockPiston.set(true);
  chassis.pid_drive_set(12_in, 30, false);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait_quick();
  pros::delay(500);
  chassis.pid_turn_relative_set(94_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(28.5_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_relative_set(64_deg, TURN_SPEED, true);
  chassis.pid_wait_quick();
  /*chassis.pid_drive_set(16_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(1400);*/
  chassis.pid_drive_set(-15_in, DRIVE_SPEED, true);
  intakeUpper.move(-50);
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
  chassis.odom_xyt_set(66.75_in, -17.75_in, -90_deg);
}

void leftR() { //Left red coordinate setter
  chassis.odom_xyt_set(-66.75_in, 17.75_in, 90_deg);
}

void rightB() { //Right blue coordinate setter
  chassis.odom_xyt_set(66.75_in, 17.75_in, -90_deg);
}

void rightR() { //Right red coordinate setter
  chassis.odom_xyt_set(-66.75_in, -17.75_in, 90_deg);
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

void skillsAuton() { // I want to eventually be using odom for this
  chassis.odom_xyt_set(-47.536_in, 9.337_in, 45_deg); //Starting position

  //Movement to first set
  chassis.pid_odom_set({{{-44.383_in, 17.826_in}, fwd, DRIVE_SPEED},
                       {{-33.227_in, 23.162_in}, fwd, DRIVE_SPEED},
                       {{-24.253_in, 25.102_in, 90_deg}, fwd, 60}},
                      true); //To first group of 4
  chassis.pid_wait_until_index(1); //Waits until it passes -33.227 and 23.162
  intakeLower.move(127);
  intakeUpper.move(127);
  lockPiston.set(true);
  chassis.pid_wait();
  pros::delay(500); //Delay to grab them

  /*
  //Movement to second set of 4
  chassis.pid_drive_set(26_in, DRIVE_SPEED, true); //Get to other set
  chassis.pid_wait_quick();
  chassis.pid_drive_set(6_in, 50, true); //Straight line using PID.
  chassis.pid_wait_quick();
  pros::delay(600); //Time to grab
  */

  
  /*
  //Movement to the first big goal
  chassis.pid_odom_set({{{39.29_in, 28.74_in}, fwd, DRIVE_SPEED},
                        {{44.383_in, 40.381_in}, fwd, DRIVE_SPEED},
                        {{33.469_in, 47.9_in, 90_deg}, fwd, 60}},
                       true); //To line up with goal
  chassis.pid_wait_until_index(1);
  intakeLower.move(0); //Stop on the way
  intakeUpper.move(0); //Same thing
  chassis.pid_wait();
  */

  /*
  //First scoring set (8)
  chassis.pid_drive_set(-12_in, DRIVE_SPEED, true); //Movement to scoring position
  chassis.pid_wait_quick();
  lockPiston.set(false);
  intakeUpper.move(127);
  intakeLower.move(127);
  pros::delay(400);
  intakeLower.move(-127); //unstick any clogged
  pros::delay(200);
  intakeLower.move(127);
  matchLoadPistons.set(true); //Get ready for match load
  pros::delay(1200); //Time to fully score
  intakeUpper.move(0);
  lockPiston.set(true);
  */

  /*
  //First match load. Upper disabled. (6)
  chassis.pid_drive_set(27_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  pros::delay(3000); //Grab all of them
  chassis.pid_drive_set(-27_in, DRIVE_SPEED, true); //Reverse back to goal
  chassis.pid_wait_quick();
  lockPiston.set(false);
  intakeUpper.move(127);
  intakeLower.move(127);
  pros::delay(400);
  matchLoadPistons.set(false);
  intakeLower.move(-127); //Unclog
  pros::delay(200);
  intakeLower.move(127);
  pros::delay(1200);
  intakeUpper.move(0);
  intakeLower.move(0);
  lockPiston.set(true);
  */

  /*
  //Next Odom
  chassis.pid_drive_set(15_in, DRIVE_SPEED, true); //Move to middle of the goal and loader
  chassis.pid_wait_quick();
  chassis.pid_turn_set(90_deg, TURN_SPEED, true); //Turn to face inwards
  chassis.pid_wait();
  chassis.pid_odom_set({{{43.656_in, 11.52_in}, fwd, DRIVE_SPEED},
                        {{40.26_in, -14.188_in}, fwd, DRIVE_SPEED},
                        {{22.798_in, -22.677_in, 180_deg}, fwd, 60}},
                        true); //Movement to third group of 4
  chassis.pid_wait_until_index(1);
  intakeLower.move(127); //Start spinning before arriving
  chassis.pid_wait();
  pros::delay(400); //Time to grab
  */

  /*
  //Last set of 4 movement
  chassis.pid_drive_set(26_in, DRIVE_SPEED, true); //Get to other set
  chassis.pid_wait_quick();
  chassis.pid_drive_set(6_in, 50, true); //Slow down when closer
  chassis.pid_wait_quick();
  pros::delay(600); //Delay to grab
  */

  /*
  //Odom movement to goal area
  chassis.pid_odom_set({{{-40.017_in, -30.923_in}, fwd, DRIVE_SPEED},
                        {{-42.658_in, -48_in, -90_deg}, fwd, 60}},
                        true); //To line up with goal
  chassis.pid_wait_until_index(1);
  intakeLower.move(0); //Stop moving to prevent clogging
  intakeUpper.move(0); //Same thing
  chassis.pid_wait();
  */

  /*
  //Scoring those cubes (8)
  chassis.pid_drive_set(-12_in, DRIVE_SPEED, true); //Reverse into it
  chassis.pid_wait_quick();
  lockPiston.set(false);
  intakeUpper.move(127);
  intakeLower.move(127);
  pros::delay(400);
  intakeLower.move(-127); //Drill already known
  pros::delay(200);
  intakeLower.move(127);
  matchLoadPistons.set(true);
  pros::delay(1200);
  intakeUpper.move(0); //Stop before match loading. If block built remove
  lockPiston.set(true);
  */

  /*
  //Second Match Loader
  chassis.pid_drive_set(27_in, DRIVE_SPEED, true); //Movement to it
  chassis.pid_wait_quick();
  pros::delay(3000); //Grabbing all of them
  */

  /*
  //Scoring second match loader (6)
  chassis.pid_drive_set(-27_in, DRIVE_SPEED, true); //Rev to goal
  chassis.pid_wait_quick();
  lockPiston.set(false);
  intakeUpper.move(127); //Move them!
  intakeLower.move(127); //Yeah!
  pros::delay(400);
  matchLoadPistons.set(false);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
  pros::delay(1200);
  intakeUpper.move(0);
  intakeLower.move(0);
  lockPiston.set(true);
  chassis.pid_drive_set(15_in, DRIVE_SPEED, true); //Return to middle
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-45_deg, TURN_SPEED, true); //Turn to movable angle
  */

  /*
  //Odom to third match loader
  chassis.pid_odom_set({{{-49.961_in, -60.269_in}, fwd, DRIVE_SPEED},
                        {{20.615_in, -59.784_in}, fwd, DRIVE_SPEED},
                        {{48.021_in, -47.172_in, 90_deg}, fwd, 60}},
                        true); //In line with goal and match loader
  chassis.pid_wait();
  */

  /*
  //Match Loader #3
  chassis.pid_drive_set(16_in, DRIVE_SPEED, true); //Move into
  chassis.pid_wait_quick();
  intakeUpper.move(127);
  intakeLower.move(127);
  pros::delay(3000); //Grab them
  */

  /*
  //Scoring Match Loader #3 (6)
  chassis.pid_drive_set(-27_in, DRIVE_SPEED, true); //Reverse into goal
  chassis.pid_wait_quick();
  lockPiston.set(false);
  intakeUpper.move(127);
  intakeLower.move(127);
  pros::delay(400);
  matchLoadPistons.set(false);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
  pros::delay(1200);
  intakeUpper.move(0);
  intakeLower.move(0);
  lockPiston.set(true);
  chassis.pid_drive_set(15_in, DRIVE_SPEED, true); //Move to middle
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-90_deg, TURN_SPEED, true); //Turn inwards
  chassis.pid_wait();
  */

  /*
  //Odom to last match loader (top left)
  chassis.pid_odom_set({{{37.107_in, 7.64_in}, fwd, DRIVE_SPEED},
                        {{17.462_in, 29.225_in}, fwd, DRIVE_SPEED},
                        {{-33.469_in, 33.105_in}, fwd, DRIVE_SPEED},
                        {{-47.051_in, 47.172_in, -90_deg}, fwd, 60}}, //
                        true);
  chassis.pid_wait();
  */

  /*
  //Match Loader #4
  chassis.pid_drive_set(16_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
  intakeLower.move(127);
  pros::delay(3000); //Grab them
  */

  /*
  //Scoring match loader #4 (6)
  chassis.pid_drive_set(-27_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  lockPiston.set(false);
  intakeUpper.move(127);
  intakeLower.move(127);
  pros::delay(400);
  matchLoadPistons.set(false);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
  pros::delay(1200);
  intakeUpper.move(0);
  intakeLower.move(0);
  lockPiston.set(true);
  chassis.pid_drive_set(15_in, DRIVE_SPEED, true); //Move away from goal
  chassis.pid_wait_quick();
  */


  //Park
  //Add
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