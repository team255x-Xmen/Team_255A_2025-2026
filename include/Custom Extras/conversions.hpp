#include "iostream"
#include <cstdlib>

#pragma once
#ifndef CONVERSIONS_HPP
#define CONVERSIONS_HPP

int convInt(float input);
int convInt(double input);
int convInt(char input);
float convFloat(int input);
float convFloat(double input);
float convFloat(char input);
double convDouble(int input);
double convDouble(float input);
double convDouble(char input);
char convChar(int input);
char convChar(float input);
char convChar(double input);

#endif