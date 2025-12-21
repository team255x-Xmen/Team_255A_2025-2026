#include "driver-motions.hpp" //Titular include file



//I do know that all of these motions could be streamlined with
//The use of functions (e.g. the turning to the point and quadrants being in a function)
//But at the same time I did this in like an hour when I was bored
//And I don't really care too much
//Plus the computer does the same amount of stuff so I don't care



void odomScore() { //Motion that finds nearest goal, lines up, and scores
    //Calculate nearest big goal
    double robotX = chassis.odom_x_get();
    double robotY = chassis.odom_y_get();

    char quadrant = 0; //number 0-9 that represents the qaudrant the robot is currently in
    if (robotX <= 0 && robotY <= 0) quadrant = 3; //Bottom Left
    if (robotX <= 0 && robotY > 0) quadrant = 2; //Top Left
    if (robotX > 0 && robotY <= 0) quadrant = 4; //Bottom Right
    if (robotX > 0 && robotY > 0) quadrant = 1; //Top Right

    //Moving to scoring position

    //Qaudrant 1 (top right)
    if (quadrant == 1) {
        chassis.pid_turn_set({44_in, 34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{44_in, 34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{48_in, 48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        chassis.pid_drive_set(-12_in, 110, true); //Moves into scoring position
        chassis.pid_wait_quick();
    } else if (quadrant == 2) {
        chassis.pid_turn_set({-44_in, 34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{-44_in, 34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{-48_in, 48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(-90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        chassis.pid_drive_set(-12_in, 110, true); //Moves into scoring position
        chassis.pid_wait_quick();
    } else if (quadrant == 3) {
        chassis.pid_turn_set({-44_in, -34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{-44_in, -34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{-48_in, -48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(-90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        chassis.pid_drive_set(-12_in, 110, true); //Moves into scoring position
        chassis.pid_wait_quick();
    } else if (quadrant == 4) {
        chassis.pid_turn_set({44_in, -34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{44_in, -34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{48_in, -48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        chassis.pid_drive_set(-12_in, 110, true); //Moves into scoring position
        chassis.pid_wait_quick();
    }

    //This just moves into scoring position
    //So I can react to actually scoring
}

void odomMatchLoad() { //Motion that match loads nearest loader (fully). Sits there
    //Calculate nearest big goal
    double robotX = chassis.odom_x_get();
    double robotY = chassis.odom_y_get();

    char quadrant = 0; //number 0-9 that represents the qaudrant the robot is currently in
    if (robotX <= 0 && robotY <= 0) quadrant = 3; //Bottom Left
    if (robotX <= 0 && robotY > 0) quadrant = 2; //Top Left
    if (robotX > 0 && robotY <= 0) quadrant = 4; //Bottom Right
    if (robotX > 0 && robotY > 0) quadrant = 1; //Top Right

    //Moving to scoring position

    //Qaudrant 1 (top right)
    if (quadrant == 1) {
        chassis.pid_turn_set({44_in, 34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{44_in, 34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{48_in, 48_in}, fwd, 110}, true); //Moves in line with goal & loader
        chassis.pid_wait_quick();
        chassis.pid_turn_set(90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        matchLoadPistons.set(true);
        chassis.pid_drive_set(12_in, 110, true); //Moves to match loader
        chassis.pid_wait_quick();
    } else if (quadrant == 2) {
        chassis.pid_turn_set({-44_in, -34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{-44_in, 34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{-48_in, 48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(-90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        matchLoadPistons.set(true);
        chassis.pid_drive_set(12_in, 110, true); //Moves to match loader
        chassis.pid_wait_quick();
    } else if (quadrant == 3) {
        chassis.pid_turn_set({-44_in, -34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{-44_in, -34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{-48_in, -48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(-90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        matchLoadPistons.set(true);
        chassis.pid_drive_set(12_in, 110, true); //Moves to match loader
        chassis.pid_wait_quick();
    } else if (quadrant == 4) {
        chassis.pid_turn_set({44_in, -34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{44_in, -34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{48_in, -48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        matchLoadPistons.set(true);
        chassis.pid_drive_set(12_in, 110, true); //Moves to match loader
        chassis.pid_wait_quick();
    }

    intakeLower.move(127); //Moves lower at end of motion

    //This moves into position of the match loader
    //And starts moving (will update next driver cycle but not too critical)
}

void odomMatchLoadScore() { //Motion that match loads nearest goal for ~3 seconds and scores it
    //Calculate nearest big goal
    double robotX = chassis.odom_x_get();
    double robotY = chassis.odom_y_get();

    char quadrant = 0; //number 0-9 that represents the qaudrant the robot is currently in
    if (robotX <= 0 && robotY <= 0) quadrant = 3; //Bottom Left
    if (robotX <= 0 && robotY > 0) quadrant = 2; //Top Left
    if (robotX > 0 && robotY <= 0) quadrant = 4; //Bottom Right
    if (robotX > 0 && robotY > 0) quadrant = 1; //Top Right

    //Moving to scoring position

    //Qaudrant 1 (top right)
    if (quadrant == 1) {
        chassis.pid_turn_set({44_in, 34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{44_in, 34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{48_in, 48_in}, fwd, 110}, true); //Moves in line with goal & loader
        chassis.pid_wait_quick();
        chassis.pid_turn_set(90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        matchLoadPistons.set(true);
        chassis.pid_drive_set(12_in, 110, true); //Moves to match loader
        chassis.pid_wait_quick();
    } else if (quadrant == 2) {
        chassis.pid_turn_set({-44_in, 34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{-44_in, 34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{-48_in, 48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(-90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        matchLoadPistons.set(true);
        chassis.pid_drive_set(12_in, 110, true); //Moves to match loader
        chassis.pid_wait_quick();
    } else if (quadrant == 3) {
        chassis.pid_turn_set({-44_in, -34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{-44_in, -34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{-48_in, -48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(-90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        matchLoadPistons.set(true);
        chassis.pid_drive_set(12_in, 110, true); //Moves to match loader
        chassis.pid_wait_quick();
    } else if (quadrant == 4) {
        chassis.pid_turn_set({44_in, -34_in}, fwd, 80, true); //Turns to safe point
        chassis.pid_wait();
        chassis.pid_odom_set({{44_in, -34_in}, fwd, 110}, true); //Moves to region safe from all angles
        chassis.pid_wait_quick();
        chassis.pid_odom_set({{48_in, -48_in}, fwd, 110}, true); //Moves in line with goal
        chassis.pid_wait_quick();
        chassis.pid_turn_set(90_deg, 80, true); //Turns to scoring angle
        chassis.pid_wait();
        matchLoadPistons.set(true);
        chassis.pid_drive_set(12_in, 110, true); //Moves to match loader
        chassis.pid_wait_quick();
    }

    intakeLower.move(127); //Starts moving it to load
    pros::delay(3000); //Waits for three seconds
    chassis.pid_drive_set(-24_in, 110, true); //Moves in reverse
    chassis.pid_wait_quick();
    intakeUpper.move(127); //Moves upper
    matchLoadPistons.set(false); //Turns off at end of motion

    //Cycle bottom if needed
}