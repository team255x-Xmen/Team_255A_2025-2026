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
//Unless there is a way to automatically have that known | Look into that

//When checked, set a boolean
//When auton runs, look through every member of the class
//Find which one has the boolean set to true
//Auton should then use the callback associated.

//Class definition is needed
//Same as header guards

#pragma once

#ifndef MENU_H //If not already made and defined
#define MENU_H //Start of definition

using namespace std; //So any logs and standard library don't need namespace defintiion

extern void drawImage();

extern int color; //variable to use as color
extern int textColor; //Variable for text color

class autons{ //Autons class
    public: //Accessed by user. These are called

        autons(string n, int l, int r, int t, int b, void (*callback)(), bool blue) { //Constructor
            this->name = n; //Set name to n
            this->positionLeft = l; //set to l
            this->positionRight = r; //set to r
            this->positionTop = t; //set to t
            this->positionBottom = b; //set to b
            this->callback = callback; //set to callback
            this->tag = blue; //set to tag
        }

        autons(string n, int l, int r, int t, int b, void (*callback)(), bool blue, bool s) { //Alternative Contructor for starting Selected
            this->name = n; //Set name to n
            this->positionLeft = l; //set to l
            this->positionRight = r; //set to r
            this->positionTop = t; //set to t
            this->positionBottom = b; //set to b
            this->callback = callback; //set to callback
            this->tag = blue; //set to tag
            this->Selected = s; //set to Selected
        }

        autons(string n, int l, int r, int t, int b, void (*callback)(), bool blue, bool s, bool skills) { //Alternative Contructor for selected & skills
            this->name = n; //Set name to n
            this->positionLeft = l; //set to l
            this->positionRight = r; //set to r
            this->positionTop = t; //set to t
            this->positionBottom = b; //set to b
            this->callback = callback; //set to callback
            this->tag = blue; //set to tag
            this->Selected = s; //set to Selected
            this->skills = skills; //set to skills
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
            pros::screen::set_pen(textColor); //Blue
            pros::screen::print(pros::E_TEXT_MEDIUM, (positionLeft + 8), ((positionTop + positionBottom)/2), name.c_str());
        }
    
        string nameIs() const { //Getter for name
            return name;
        }

        void (*callbackIs())() { //Returns the callback function pointer
            return callback;
        }

        bool isBlue() const { //Read-only to check if blue auton
            return tag;
        }

        bool isSkills() const {
            return skills;
        }

    private: //Hidden from user. Public is what is accessed

        string name; //Name of auton
        int positionLeft; //Left edge of said auton
        int positionRight; //Right edge
        int positionBottom; //Bottom position
        int positionTop; //Top position
        void (*callback)(); //Callback to the function
        bool tag; //Tag for checking if it is blue
        bool skills = false; //Is skills. Defaults to false unless constructed otherwise

        bool Selected = false; //Selected tracker | Starts false
};

class colorManager { //Class to manage toggle color
    public:

        colorManager(int l, int r, int t, int b) { //Constructor. left, right, top, and bottom bounds
            this->positionLeft = l;
            this->positionRight = r;
            this->positionTop = t;
            this->positionBottom = b;
        }

        bool checkPressed(int x, int y) {
            return ((x >= positionLeft&&x <= positionRight)&&
                    (y <= positionBottom&&y >= positionTop)); //Returns if all are true
        }

        void toggle() { //Toggles color | Starts blue |  Blue is true
            this->blue = !blue;
        }

        bool isBlue() { //Checks if blue
            return blue;
        }

        void draw() {
            string name = blue ? "Blue" : "Red";
            pros::screen::set_pen(0xFFFFFF); //Always White (Stick out)
            pros::screen::fill_rect(positionLeft, positionTop, positionRight, positionBottom); //Draws rectangle
            pros::screen::set_pen(blue ? 0x0000FF : 0xFF0000); //if blue make blue
            pros::screen::print(pros::E_TEXT_MEDIUM, (positionLeft + 8), ((positionTop + positionBottom)/2), name.c_str());
        }

    private:
    
        bool blue = true; //When true set textColor to blue (0x0000FF)
                          //Else set to red (0xFF0000)
        int positionLeft; //Left edge of manager
        int positionRight; //Right edge
        int positionBottom; //Bottom edge
        int positionTop; //Top edge

};

extern bool selectedIsBlue;

class AutonManager{ //This class handles the autons. Make 1
    public:

    void add(autons a) {list.push_back(a);} //Adds all of input a
    //Call this to add new autons

    void addColorManager(colorManager c) {cMNG.push_back(c);} //Adds in Color Manager

    void screenTouched(int x, int y) {

        if (cMNG[0].checkPressed(x, y)) { //Checks if manager was pressed
            cMNG[0].toggle(); //Switch Color on press
        } //Checks first so only right color autons can be selected

        for (auto &a : list) { //For every item in list (called a)
            if (a.isSkills()) { //Checks if skills. Ignores color difference
                a.setSelected(a.containsPoint(x, y));
            } else {
                a.isBlue() == cMNG[0].isBlue() ? a.setSelected(a.containsPoint(x, y)) : a.setSelected(false);
            } //Checks if correct color
            //Runs this function. This updates every auton to only select the last touched
        } //Sets selected if it contains that point

        textColor = cMNG[0].isBlue() ? 0x0000FF : 0xFF0000; //Sets text to blue if blue

        for (const auto &a : list) { //Redraw after finding new selected
            color = a.isSelected() ? 0xFFFF00 : 0x000000; //Set color yellow if selected
            if (a.isSkills()) {
                a.drawBox(); //Draws if skills regardless of color
            } else { //If not skills then check color
                cMNG[0].isBlue() == a.isBlue() ? (a.drawBox(), 1) : 1; /*
                *Only draw if correct color
                *(, 1) and : 1 are added so both sides are int
                *And I don't have to add something to the false branch*/
            }
        } //Update visually. No need for background redraw, the boxes stay in place
        cMNG[0].draw(); //Draws color manager (Once)
    } 

    string selectedAuton() { //Return the string name of the selected
        string b = "";
        for (auto &a : list) {
            if (a.isSelected()) {
                b = a.nameIs();
                if (a.isSkills()) {
                    return b; //If skills end before adding color tag
                }
            }
        }
        if (b == "") { //If no auton selected
            return cMNG[0].isBlue() ? "Blue" : "Red"; //State full color with no extra spaces
        } //Otherwise returns full name
        //Will always default to this one (outside of any conditionals for this purpose)
        return b + (cMNG[0].isBlue() ? " B" : " R"); //Adds color tag onto the name
    }
    
    void runSelectedAuton() { //Run the auton
        if (storedCallback&&autonWasSelected) { //Checks if initialized
            storedCallback(); //Runs callback
        } else {
            basicDrive(); //Runs backup if nothing was selected
        }
    }

    void printAutons() {
        drawImage();

        pros::delay(10); //Tiny Delay so brain screen can catch up
        //If it moves on before refresh rate can catch up it doesn't save autons

        textColor = cMNG[0].isBlue() ? 0x0000FF : 0xFF0000; //Sets text to blue if blue

        for (const auto &a : list) {
            color = a.isSelected() ? 0xFFFF00 : 0x000000; //Set color yellow if selected
            if (a.isSkills()) {
                a.drawBox(); //Draws if skills regardless of color
            } else { //If not skills then check color
                cMNG[0].isBlue() == a.isBlue() ? (a.drawBox(), 1) : 1; //Draws auton if correct
            }
        }
        cMNG[0].draw(); //Draws color manager (Once)
    }

    void store() { //Stores selected auton's callback for after termination
        bool wasSelected = false;
        for (auto &a : list) { //Stores the current callback for selected function
            if (a.isSelected()) {
                storedCallback = a.callbackIs();
                selectedIsBlue = a.isBlue(); //Updates the seleted color with selected auton
                wasSelected = true;
                autonWasSelected = true;
            }
        }

        if (!wasSelected) {
            storedCallback = nullptr; //Unintiializes if nothing was selected
            autonWasSelected = false;
        }
    }

    void terminateAutons() { //Run after storing
        terminated = true; //Sets to true
        list.clear(); //Clear the list that contains autons
        cMNG.clear(); //Clears storage so it doesn't reprint
        //Removes them from being able to be touched
        //No need to deconstruct them. They're fine how they are
        drawBG(); //Rerun Removes all boxes
    }

    bool hasTerminated() const { //Read-only for checking if terminated already
        return terminated; //Returns state of variable
    }

    void drawBG() {
        drawImage();
    }

    private:

    bool terminated = false; //False by default | Helper to check if already terminated
    vector<autons> list; //Vector for the list that contains autons
    void (*storedCallback)() = nullptr; //Initialized for safety
    vector<colorManager> cMNG;
    bool autonWasSelected = false;
};

//autons should not be used to interact with them besides creation
//Instead use the manager to call things
//It automatically interacts with autons
//Safely (interacts with every member)

//If more functionality is needed
//Add to AutonManager
//So it maintains the authority
//Use add to add autons.
//Can be defined beforehand.

//Also:
//Come up with formula that takes in what autons you want
//Plus their name and callback
//And auto figure out pixel requirements
//Can be split into two rows for easier code writing.

//Not immediately needed but an option for the future

#endif // End of file