#include "api.h" //Pros Everything (brain, others)
#include "autons.hpp" //Autos
#include "liblvgl/lvgl.h" //Photos and colors
#include <vector> //Vectors
#include "Custom Extras/extras.hpp" //My conversions
#include <fstream> //File management

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

        bool isSkills() const { //Returns if current auton is skills
            return skills;
        }

        ~autons() { //Destructor for the class. Will remove the ability to write to the screen
            //Cleanup can be added. I can have it print to something, or just do what I need to
            //Since this class has no pointers to internal variables, it will be fine keeping how it is
            //If another pointer (say void *food) was set to an internal variable using the keyword new, I would
            //Need to delete it, to prevent a memory leak. I don't have it, so this will be empty.
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

    void screenTouched(int x, int y) {

        if (cMNG.checkPressed(x, y)) { //Checks if manager was pressed
            cMNG.toggle(); //Switch Color on press
        } //Checks first so only right color autons can be selected

        for (auto &a : list) { //For every item in list (called a)
            if (a.isSkills()) { //Checks if skills. Ignores color difference
                a.setSelected(a.containsPoint(x, y));
            } else {
                a.isBlue() == cMNG.isBlue() ? a.setSelected(a.containsPoint(x, y)) : a.setSelected(false);
            } //Checks if correct color
            //Runs this function. This updates every auton to only select the last touched
        } //Sets selected if it contains that point

        update_saved_auton();
        print(); //Prints the autons
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
            return cMNG.isBlue() ? "Blue" : "Red"; //State full color with no extra spaces
        } //Otherwise returns full name
        //Will always default to this one (outside of any conditionals for this purpose)
        return b + (cMNG.isBlue() ? " B" : " R"); //Adds color tag onto the name
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

        load_previously_saved_auton(); //Loads auton if it can via SD card
        print(); //Prints the autons to start
    }

    void store() { //Stores selected auton's callback for after termination
        bool wasSelected = false;
        for (auto &a : list) { //Stores the current callback for selected function
            if (a.isSelected()) {
                storedCallback = a.callbackIs();
                selectedIsBlue = a.isBlue(); //Updates the seleted color with selected auton
                wasSelected = true;
                autonWasSelected = true;
                a.isSkills() ? skillsSelected = true : skillsSelected = false; //Updates is skills
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
        //Clears storage so it doesn't reprint
        //Removes them from being able to be touched
        //They will deconstruct here because they leave scope, using the destructor I made in that class
        drawBG(); //Rerun Removes all boxes
    }

    bool hasTerminated() const { //Read-only for checking if terminated already
        return terminated; //Returns state of variable
    }

    void drawBG() {
        drawImage();
    }

    bool skills_is_selected() const {
        return skillsSelected;
    }

    private:

    bool terminated = false; //False by default | Helper to check if already terminated
    vector<autons> list; //Vector for the list that contains autons
    void (*storedCallback)() = nullptr; //Initialized for safety
    colorManager cMNG{364, 476, 124, 236}; //This creates the color manager cMNG
    bool autonWasSelected = true; //Starts true because an auton starts selected
    bool skillsSelected = false;

    int print() { //Function to draw boxes and cMNG using correct color.
        if (terminated) return 0; //Early exit if already terminated. Makes sure it cannot reprint after termination

        textColor = cMNG.isBlue() ? 0x0000FF : 0xFF0000; //Sets text to blue if blue

        for (const auto &a : list) {
            color = a.isSelected() ? 0xFFFF00 : 0x000000; //Set color yellow if selected
            if (a.isSkills()) {
                a.drawBox(); //Draws if skills regardless of color
            } else { //If not skills then check color
                cMNG.isBlue() == a.isBlue() ? (a.drawBox(), 1) : 1; //Draws auton if correct
            }
        }
        cMNG.draw(); //Draws color manager (Once)

        return 1;
    }

    void load_previously_saved_auton() { //Checks and loads save data for autonomous
        try { //Statement to do this. Exceptions can occur, so try for safety
            string selected = sdSelectedCheck(); //Creates a shorter name
            bool selectedBlue = color_auton(); //Runs this once and gives it to the variable
            for (auto &a : list) { //Goes through every auton
                if (a.nameIs() == selected && a.isBlue() == selectedBlue)  { //Checks if correct name and color
                    a.setSelected(true); //Sets to true if fully correct
                } else a.setSelected(false); //Sets to false otherwise
            }
        }
        catch (int errorCode) { //Handles exceptions
            switch (errorCode) { //Goes through errorCodes
                case 1: //If error 1
                printf("Error opening file. File most likely does not exist"); //Print to terminal why
                break; //Leave switch
                default: //If not my own error
                printf("Other unknown error occured"); //Then explain why
                break; //Leave switch
            }
        }
    }

    void update_saved_auton() { //Saves current auton to SD card for starting selected later
        try { //Try for safety
            saveAutonomous(list); //Save it with this function, using list
        }
        catch (int code) { //Catch any thrown errors
            if (code == 2) { //If my own error
                printf("Couldn't open file"); //Explain why
            } else printf("Unknown exception received"); //Print anything else
        }
    }

    string sdSelectedCheck() { //Reads to check autonLog.txt. Returns what the auton is in a string
        ifstream selectedAutonomous("autonLog.txt"); //Creates, writes, and reads through selectedAutonomous
        if (!selectedAutonomous.is_open()) throw 1; //If the file can't open, exit
        string autonSelected; //Creates output
        while (getline(selectedAutonomous, autonSelected)) { //Goes through file. Should be 1 line
            cout<<autonSelected; //Prints current line to autonSelected
        }
        selectedAutonomous.close(); //Close for safety
        return autonSelected; //End the function
    }

    bool color_auton() { //A function to check the color of the auton, and correct the color manager
        ifstream selected("autonLog.txt"); //Opens the file is reading mode
        if (!selected.is_open()) throw 1; //If it doesn't open, throw exception
        bool blue; //Boolean for the color
        string color; //String to store the name of the color
        int line = 1; //Line index number
        while (getline(selected, color)&&line <= 2) { //Goes through until line 2
            if (line == 2) { //When line 2
                cout<<color; //Print current line to color
            }
            ++line; //Index the line
        }
        color == "blue" ? blue = true : blue = false; //Sets boolean to the current color
        if (blue != cMNG.isBlue()) cMNG.toggle(); //If the color manager is wrong correct it
        selected.close(); //Close the file to save it
        return blue; //End the function
    }

    void saveAutonomous(vector<autons> autos) { //Saves the selected autonomous to a file for later use
        fstream saveFile("autonLog.txt"); //Not opened in appending mode (so it overwrites)
        if (!saveFile.is_open()) throw 2; //If it would error it throws an exception
        for (auto &auton : autos) { //Goes through the inputted list
            if (auton.isSelected()) { //If selected
                saveFile<<auton.nameIs()<<endl; //Save the name, move to line 2
                string color = auton.isBlue() ? "blue" : "red"; //Create the color tag
                saveFile<<color<<endl; //Save the color to line 2
            }
        }
        saveFile.close(); //Close the file to save it
    }
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