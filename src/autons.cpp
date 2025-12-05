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

//Driver Control or other motor custom PID Example
//ez::PID functionName(kP, kI, kD, startingTotal, "Name");
//Motor.move(Name, compute_error(target - avgPos, avgPos));

void Autonomous12inDrive() {

  //Drive forward 12 inches, at 110/127 volts, with slew
  //Slew for first 4 inches is at half power (of 110)
    //Don't mess with the constants, it's fine how it's setup
    //It's just an example so I don't forget
  //Waits until motion completes
  chassis.slew_drive_set(true);
  chassis.slew_drive_constants_set(4_in, 55);
  chassis.pid_drive_set(12_in, DRIVE_SPEED, false);
  chassis.pid_wait_quick();


}

void AutonomousTurn90Degrees() {

  //Turn in place to 90 degrees, at 90/127 volts
  //Waits until motion completes
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

}

//Add mine. Simple ones above
//Wait quick is better, but overshoot is same amount of time.

void simpleLeftSide() {
  matchLoadPistons.set(false);
  chassis.pid_drive_set(23_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-90_deg, TURN_SPEED, true);
  chassis.pid_wait();
  intakeLower.move(127);
  chassis.pid_drive_set(8_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(0_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(14_in, 40, true);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait();
  pros::delay(1000);
  chassis.pid_turn_set(-120_deg, 50, true);
  chassis.pid_wait();
  chassis.pid_drive_set(29_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  matchLoadPistons.set(false);
  chassis.pid_turn_set(-180_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-11_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
  pros::delay(1000);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
}




void simpleRightSide() {
  matchLoadPistons.set(false);
  chassis.pid_drive_set(23_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(90_deg, TURN_SPEED, true);
  chassis.pid_wait();
  intakeLower.move(127);
  chassis.pid_drive_set(8_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(0_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(14_in, 40, true);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait();
  pros::delay(1000);
  chassis.pid_turn_set(120_deg, 50, true);
  chassis.pid_wait();
  chassis.pid_drive_set(29_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  matchLoadPistons.set(false);
  chassis.pid_turn_set(180_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-11_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
  pros::delay(1000);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
}




void LeftSoloAWP() {

}




void RightSoloAWP() {

}




void LeftDuoAWP() {
  matchLoadPistons.set(false);
  chassis.pid_drive_set(23_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-90_deg, TURN_SPEED, true);
  chassis.pid_wait();
  intakeLower.move(127);
  chassis.pid_drive_set(8_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(0_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(12_in, 40, true);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_turn_set(-120_deg, 50, true);
  chassis.pid_wait();
  chassis.pid_drive_set(27.5_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(-180_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-11_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
  pros::delay(400);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
  pros::delay(1600);
  chassis.pid_drive_set(25_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(0);
  pros::delay(2200);
  chassis.pid_drive_set(-26_in, DRIVE_SPEED, true);
  intakeUpper.move(127);
  pros::delay(400);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
}




void RightDuoAWP() {
  matchLoadPistons.set(false);
  chassis.pid_drive_set(23_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(90_deg, TURN_SPEED, true);
  chassis.pid_wait();
  intakeLower.move(127);
  chassis.pid_drive_set(8_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(0_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(12_in, 40, true);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait();
  pros::delay(500);
  chassis.pid_turn_set(120_deg, 50, true);
  chassis.pid_wait();
  chassis.pid_drive_set(27.5_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(180_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-11_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
  pros::delay(400);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
  pros::delay(1600);
  chassis.pid_drive_set(25_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(0);
  pros::delay(2200);
  chassis.pid_drive_set(-26_in, DRIVE_SPEED, true);
  intakeUpper.move(127);
  pros::delay(400);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
}




void skillsAuton() {
  matchLoadPistons.set(false);
  chassis.pid_drive_set(23_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(90_deg, TURN_SPEED, true);
  chassis.pid_wait();
  intakeLower.move(127);
  chassis.pid_drive_set(8_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(0_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(14_in, 40, true);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait();
  chassis.pid_drive_set(18_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(10_in);
  matchLoadPistons.set(false);
  chassis.pid_wait_quick();
  chassis.pid_drive_set(14_in, 40, true);
  chassis.pid_wait_until(4_in);
  matchLoadPistons.set(true);
  chassis.pid_wait();
  pros::delay(1200);
  chassis.pid_drive_set(-92_in, DRIVE_SPEED, true);
  chassis.pid_wait_until(-40_in);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(90_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(23.5_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  chassis.pid_turn_set(180_deg, TURN_SPEED, true);
  chassis.pid_wait();
  chassis.pid_drive_set(-18_in, DRIVE_SPEED, true);
  chassis.pid_wait_quick();
  intakeUpper.move(127);
  pros::delay(1000);
  intakeLower.move(-127);
  pros::delay(200);
  intakeLower.move(127);
}