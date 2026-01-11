#include "menu.hpp" /* *Has color variables
* Not directly needed but has main file
* And subsystems as well
*/
#include "subsystems.hpp" //Has motors and everything

//Header file for driver motions
//Declare the motions here, and use them as callbacks in opcontrol
//Then define them in the titular cpp file

#pragma once
#ifndef DRIVER-MOTIONS_HPP
#define DRIVER-MOTIONS_HPP

void odomScore();
void odomMatchLoad();
void odomCloseScore();

#endif