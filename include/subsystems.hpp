#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

// Your motors, sensors, etc. should go here.  Below are examples

// inline pros::Motor intake(1);
// inline pros::adi::DigitalIn limit_switch('A');

inline pros::Motor intakeLower(11);
inline pros::Motor intakeUpper(12);
//inline pros::adi::Pneumatics matchLoadPistons('A', false);
inline ez::Piston matchLoadPistons('A', false);
