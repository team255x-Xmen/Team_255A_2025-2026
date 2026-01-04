#include "driver-motions.hpp" //Titular include file

double robotX = 0; //Variable for robot's X
double robotY = 0; //Variable for robot's Y

char quadrant = 0; //Char (0-9) to represent robot's current area. Number = std graph quadrant

void calcQuadrant() { //Updates quadrant of the field for robot
    //Calculate nearest big goal
    robotX = chassis.odom_x_get(); //Updates X
    robotY = chassis.odom_y_get(); //Updates Y

    quadrant = 0; //number 0-9 that represents the qaudrant the robot is currently in

    if (robotX <= 0 && robotY <= 0) quadrant = 3; //Bottom Left
    if (robotX <= 0 && robotY > 0) quadrant = 2; //Top Left
    if (robotX > 0 && robotY <= 0) quadrant = 4; //Bottom Right
    if (robotX > 0 && robotY > 0) quadrant = 1; //Top Right
}

void OdomMovement() { //Odom movement to quadrant's scoring tile line (y position)

    switch (quadrant) {
        case 1: //Quadrant 1 (Upper right)
            chassis.pid_turn_set({44_in, 34_in}, fwd, 80, true); //Turns to safe point
            chassis.pid_wait_quick();
            chassis.pid_odom_set({{{44_in, 34_in}, fwd, 110},
                                  {{46_in, 42_in}, fwd, 110},
                                  {{48_in, 48_in}, fwd, 55}}, true); //Moves to region safe from all angles
            chassis.pid_wait_quick();
            chassis.pid_turn_set(90_deg, 80, true); //Turns to scoring angle
            chassis.pid_wait_quick();
        break; //Prevents fall through
        case 2: //Quadrant 2 (Upper Left)
            chassis.pid_turn_set({-44_in, 34_in}, fwd, 80, true); //Turns to safe point
            chassis.pid_wait_quick();
            chassis.pid_odom_set({{{-44_in, 34_in}, fwd, 110},
                                  {{-46_in, 42_in}, fwd, 110},
                                  {{-48_in, 48_in}, fwd, 55}}, true); //Moves to region safe from all angles
            chassis.pid_wait_quick();
            chassis.pid_turn_set(-90_deg, 80, true); //Turns to scoring angle
            chassis.pid_wait_quick();
        break;
        case 3: //Quadrant 3 (Lower Left)
            chassis.pid_turn_set({-44_in, -34_in}, fwd, 80, true); //Turns to safe point
            chassis.pid_wait_quick();
            chassis.pid_odom_set({{{-44_in, -34_in}, fwd, 110},
                                  {{-46_in, -42_in}, fwd, 110},
                                  {{-48_in, -48_in}, fwd, 55}}, true); //Moves to region safe from all angles
            chassis.pid_wait_quick();
            chassis.pid_wait_quick();
            chassis.pid_turn_set(-90_deg, 80, true); //Turns to scoring angle
            chassis.pid_wait_quick();
        break;
        case 4: //Quadrat 4 (Lower Right)
            chassis.pid_turn_set({44_in, -34_in}, fwd, 80, true); //Turns to safe point
            chassis.pid_wait_quick();
            chassis.pid_odom_set({{{44_in, -34_in}, fwd, 110},
                                  {{46_in, -42_in}, fwd, 110},
                                  {{48_in, -48_in}, fwd, 55}}, true); //Moves to region safe from all angles
            chassis.pid_wait_quick();
            chassis.pid_wait_quick();
            chassis.pid_turn_set(90_deg, 80, true); //Turns to scoring angle
            chassis.pid_wait_quick();
        break;
        default: //Code that runs when not in 4 quadrants (Not possible, but safety)
            calcQuadrant(); //Re-calculates quadrant
            break; //Optional but good practice says I should, so I did
    }
}

void scoreTileLineup() {
    switch (quadrant) {
        case 1:
            chassis.pid_turn_set({48_in, 48_in}, fwd, 80, true); //Turns to safe point
            chassis.pid_wait_quick();
            chassis.pid_odom_set({{48_in, 48_in}, fwd, 110}, true);
            chassis.pid_wait_quick();
            chassis.pid_turn_set(90_deg, 80, true);
            chassis.pid_wait_quick();
            chassis.pid_drive_set(-12_in, 110, true);
            chassis.pid_wait_quick();
            break;
        case 2:
            chassis.pid_turn_set({-48_in, 48_in}, fwd, 80, true); //Turns to safe point
            chassis.pid_wait_quick();
            chassis.pid_odom_set({{-48_in, 48_in}, fwd, 110}, true);
            chassis.pid_wait_quick();
            chassis.pid_turn_set(-90_deg, 80, true);
            chassis.pid_wait_quick();
            chassis.pid_drive_set(-12_in, 110, true);
            chassis.pid_wait_quick();
            break;
        case 3:
            chassis.pid_turn_set({-48_in, -48_in}, fwd, 80, true); //Turns to safe point
            chassis.pid_wait_quick();
            chassis.pid_odom_set({{-48_in, -48_in}, fwd, 110}, true);
            chassis.pid_wait_quick();
            chassis.pid_turn_set(-90_deg, 80, true);
            chassis.pid_wait_quick();
            chassis.pid_drive_set(-12_in, 110, true);
            chassis.pid_wait_quick();
            break;
        case 4:
            chassis.pid_turn_set({48_in, -48_in}, fwd, 80, true); //Turns to safe point
            chassis.pid_wait_quick();
            chassis.pid_odom_set({{48_in, -48_in}, fwd, 110}, true);
            chassis.pid_wait_quick();
            chassis.pid_turn_set(90_deg, 80, true);
            chassis.pid_wait_quick();
            chassis.pid_drive_set(-12_in, 110, true);
            chassis.pid_wait_quick();
            break;
        default:
            calcQuadrant();
            break;
    }

    intakeLower.move(127); //Start moving
}

//Global Functions (used in opcontrol)

void odomScore() { //Motion that finds nearest goal, lines up, and scores
    //Moving to scoring position

    calcQuadrant(); //Calculate current quadrant
    OdomMovement(); //Movement into proper area
    chassis.pid_drive_set(-12_in, 110, true); //Moves into scoring position
    chassis.pid_wait_quick();

    //This just moves into scoring position
    //So I can react to actually scoring
}

void odomMatchLoad() { //Motion that match loads nearest loader (fully). Sits there
    //Moving to scoring position

    calcQuadrant();
    OdomMovement();
    matchLoadPistons.set(true);
    chassis.pid_drive_set(12_in, 110, true); //Moves to match loader
    chassis.pid_wait_quick();
    intakeLower.move(127); //Moves lower at end of motion

    //This moves into position of the match loader
    //And starts moving (will update next driver cycle but not too critical)
}

void odomCloseScore() { //Motion that simply moves into scoring position (use when close to goal)
    //Moving to scoring position

    calcQuadrant();
    scoreTileLineup(); //Simple movement to scoring pos

    //Cycle bottom if needed
}