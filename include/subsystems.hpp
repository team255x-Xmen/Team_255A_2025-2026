#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');

//inline pros::adi::Pneumatics matchLoadPistons('A', false);
//Prob not needed added in case want to compare

inline pros::MotorGroup intakeLower({11, 14}); //The motors for the lower intake at ports 11 & 13
inline pros::Motor intakeUpper(-12); //The upper intake Motor
inline ez::Piston matchLoadPistons('A', false); //The piston to match load
inline ez::Piston lockPiston('B', false); //The piston to enable the ball lock
inline ez::Piston descorePiston('C', false); //False is descore position (piston for descore wing)