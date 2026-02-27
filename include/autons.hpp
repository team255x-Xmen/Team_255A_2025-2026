#pragma once

void default_constants();

//Autonomous Routines

//Blue
void simpleLeftSideB();
void simpleRightSideB();
void LeftDuoAWPB();
void RightDuoAWPB();
void LeftDescoreB();
void RightDescoreB();
void soloAWPB();

//Red
void simpleLeftSideR();
void simpleRightSideR();
void LeftDuoAWPR();
void RightDuoAWPR();
void LeftDescoreR();
void RightDescoreR();
void soloAWPR();

//Skills
void skillsAuton();

//Backup (if nothing selected)
void basicDrive();

//Other utility autons
void measure_offsets();
void simpleOdomDrive();
void simpleTurn();
void printTable();