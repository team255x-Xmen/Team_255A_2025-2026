#include "main.h"
#include "liblvgl/lvgl.h"
#include <vector>

//Overall purpose is to create a class that can have objects added
//to that have their own position on the brain screen known
//So manual touch identification is not required
//Items are then added to this class in main.cpp
//And defined in autons.cpp
//Now how to accomplish it?


//Now you can add an auton with a constructor that gives its information
//Then we need to add something into the class that checks if the brain is touched
//In the bounds of l, r, b, and t
//Unless there is a way to automatically have that known

//When checked, set a boolean
//When auton runs, look through every member of the class
//Find which one has the boolean set to true
//Auton should then use the callback associated.
//Find out how.

//Class definition is needed
//Same as header guards

#pragma once

#ifndef MENU_H //If not already made and defined
#define MENU_H //Start of definition

using namespace std; //So any logs and standard library doesn't need namespace defintiion

extern void drawImage();

extern int color; //variable to use as color

class autons{ //Autons class
    public: //Accessed by user. These are called

        autons(string n, int l, int r, int t, int b, void (*callback)()) { //Constructor
            this->name = n; //Set name to n
            this->positionLeft = l; //set to l
            this->positionRight = r; //set to r
            this->positionTop = t; //set to t
            this->positionBottom = b; //set to b
            this->callback = callback; //set to callback
        }

        bool containsPoint(int x, int y) const { //Run when brain clicked
            //Checks x point, checks y point. X for l & r, Y for b & t
            return ((x >= positionLeft&&x <= positionRight)&&
                    (y <= positionBottom&&y >= positionTop)); //Returns if all are true
        }

        void setSelected(bool a) { //Function to set selected. Setter
            this->Selected = a; //Sets boolean
        }

        //use const for read-only functions.
        bool isSelected() const { //Returns if it is selected
            return Selected;
        }

        void drawBox() const { //Call when drawing box after making background
            pros::screen::set_pen(color); //Yellow
            pros::screen::fill_rect(positionLeft, positionTop, positionRight, positionBottom); //Draws rectangle
            pros::screen::set_pen(0x0000FF); //Blue
            pros::screen::print(pros::E_TEXT_MEDIUM, (positionLeft + 8), ((positionTop + positionBottom)/2), name.c_str());
        }
    
        string nameIs() const { //Getter for name
            return name;
        }

        void (*callbackIs())() { //Returns the callback function pointer
            return callback;
        }

    private: //Hidden from user. Public is what is accessed

        string name; //Name of auton
        int positionLeft; //Left edge of said auton
        int positionRight; //Right edge
        int positionBottom; //Bottom position
        int positionTop; //Top position
        void (*callback)(); //Callback to the function

        bool Selected = false; //Selected tracker | Starts false
};

class AutonManager{ //This class handles the autons. Make 1
    public:

    void add(autons a) {list.push_back(a);} //Adds all of input a
    //Call this to add new autons

    void screenTouched(int x, int y) {
        for (auto &a : list) { //For every item in list (called a)
            a.setSelected(a.containsPoint(x, y)); //Runs this function. This updates every auton to only select the last touched
        } //Sets selected if it contains that point

        for (const auto &a : list) { //Redraw after finding new selected
            color = a.isSelected() ? 0xFFFF00 : 0x000000; //Set color yellow if selected
            a.drawBox(); //Draw every auton onto the screen
        }; //Update visually. No need for background redraw, the boxes stay in place
    } 

    string selectedAuton() { //Return the string name of the selected
        string b = "";
        for (auto &a : list) {
            if (a.isSelected()) {
                b = a.nameIs();
            }
        }
        return b;
    }
    
    void runSelectedAuton() { //Run the auton
        if (storedCallback) { //Checks if initialized
            storedCallback(); //Runs callback
        }
    }

    void printAutons() {
        drawImage();

        pros::delay(10); //Tiny Delay so brain screen can catch up
        //If it moves on before refresh rate can catch up it doesn't save autons

        for (const auto &a : list) {
            color = a.isSelected() ? 0xFFFF00 : 0x000000; //Set color yellow if selected
            a.drawBox(); //Draw every auton onto the screen
        }
    }

    void store() {
        for (auto &a : list) { //Stores the current callback for selected function
            if (a.isSelected()) {
                storedCallback = a.callbackIs();
            }
        }
    }

    void terminateAutons() { //Run after storing
        terminated = true; //Sets to true
        list.clear(); //Clear the list that contains autons
        //Removes them from being able to be touched
        //No need to deconstruct them. They're fine how they are
        printAutons(); //Rerun. Should remove all boxes because none are left
    }

    bool hasTerminated() const { //Read-only for checking if terminated already
        return terminated; //Returns state of variable
    }

    private:

    bool terminated = false; //False by default | Helper to check if already terminated
    vector<autons> list; //Vector for the list that contains autons
    void (*storedCallback)() = nullptr; //Initialized for safety
};

//autons should not be used
//Instead use the manager to call things
//It automatically interacts with autons
//Safely

//If more functionality is needed
//Add to AutonManager
//So it maintains the authority
//Use add to add autons.
//Can be defined beforehand.
//This is to pad out to 150 lines

#endif // End of file