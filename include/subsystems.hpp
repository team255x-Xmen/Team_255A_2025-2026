#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');

//inline pros::adi::Pneumatics matchLoadPistons('A', false);
//Prob not needed added in case want to compare
inline pros::Motor intakeLower(-11);
inline pros::Motor intakeUpper(-12);
inline ez::Piston matchLoadPistons('A', true);
inline ez::Piston UnknownPiston('B', false);
//Placeholder for the piston once we know what it does