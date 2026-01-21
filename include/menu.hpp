#include "api.h" //Pros Everything (brain, others)
#include "autons.hpp" //Autos
#include "liblvgl/lvgl.h" //Photos and colors
#include <vector> //Vectors
#include "Custom Extras/extras.hpp" //My conversions
#include <fstream> //File management
#include <atomic> //For the atomic later
#include "iostream" //I/O stream


//Goal of this major revision:
//Redo the menu with 3 sections (red, blue, utility)
//Plus calculations for autons, so all I have to do
//Is create the autons class with a string and two callbacks

//Apparently this idea of an auton selector can also be done using
//pros::lcd. But I don't know how that works right now. I'll look into it later.
//This file has been tested with one exception. It has not been tested in the autonomous phase yet
//I will need to test out and make sure the callbacks work as expected, although that should be quick and easy

struct brainPosition { //Struct to group the bounds of pixels into one name
    int left; //The left bound
    int right; //The right bound
    int top; //The top bound
    int bottom; //The bottom bound
};

#pragma once //Header guard
#ifndef MENUREVAMP_HPP //Header check
#define MENUREVAMP_HPP //The actual definition

#define BLACK 0x000000 //Abbreviates black to its hexadecimal number
#define BLUE 0x0000FF //Abbreviates blue to its hexadecimal number
#define RED 0xFF0000 //Abbreviates red to its hexadecimal number
#define YELLOW 0xFFFF00 //Abbreviates yellow to its hexadecimal number
#define WHITE 0xFFFFFF //Abbreviates white to its hexadecimal number
//This makes it easier to read later

using namespace std; //std namespace is used

extern void drawImage(); //The drawImage function

extern int color; //variable to use as color
extern int textColor; //Variable for text color

class brainSpacing { //Simple class with the name and pos variables, as well as point checking and drawing functions
    public: //Section that gets put into public for the children

    bool containsPoint(int x, int y) const { //Run when brain clicked
        //Checks x point, checks y point. X for l & r, Y for b & t
        return ((x >= pos.left&&x <= pos.right)&&
                (y <= pos.bottom&&y >= pos.top)); //Returns if all are true
    }

    void drawBox() const { //Call when drawing box after making background
        pros::screen::set_pen(color); //Yellow
        pros::screen::fill_rect(pos.left, pos.top, pos.right, pos.bottom); //Draws rectangle
        pros::screen::set_pen(textColor); //Blue
        pros::screen::print(pros::E_TEXT_MEDIUM, (pos.left + 8), ((pos.top + pos.bottom)/2), name.c_str()); //Provides the name
    }

    void setPosition(int left, int right, int top, int bottom) { //Sets the member's position to the inputted numberds
        pos.left = left; //Sets left to left
        pos.right = right; //Sets right to right
        pos.top = top; //Sets top to top
        pos.bottom = bottom; //Sets bottom to bottom
    }

    protected: //This section can't be accessed except by the children
    string name; //The name of the member
    brainPosition pos; //The position for it
};

class autons : public brainSpacing { //The autons class that inherits brainSpacing (parent class) and sets default to public
    public: //The public section

    autons(string auton_name, void (*Blue_Callback)(), void (*Red_Callback)()) { //Default constructor. Sets the name, and the 2 callbakcs
        name = auton_name; //Updates the name
        blueCallback = Blue_Callback; //Sets the blue callback
        redCallback = Red_Callback; //Sets the red callback
    } //Selected is false by default

    autons(string auton_name, void (*Blue_Callback)(), void (*Red_Callback)(), bool starts_selected) { //Constructor for setting a default selected auton
        name = auton_name; //Sets the name
        blueCallback = Blue_Callback; //Sets callback 1
        redCallback = Red_Callback; //Sets callback 2
        selected = starts_selected; //Sets the selected state to what is given
    }

    void (*blue_callback())() { //Returns the blue callback of the auton
        return blueCallback; //Returns it
    }

    void (*red_callback())() { //Returns the red callback of the auton
        return redCallback; //Returns it
    }

    bool isSelected() const { //Read-only for checking if the current auton is selected
        return selected; //Returns the current state
    }

    void setSelected(bool set_to) { //Sets the input boolean to the member's selected variable
        selected = set_to; //Updates the state
    }

    string nameIs() const { //Returns the name of the member's string
        return name; //Returns it
    }

    private: //The private section. The protected section of the parent gets added to this section
    void (*blueCallback)(); //Blue callback
    void (*redCallback)(); //Red callback
    bool selected = false; //Selected variable
};


class ManagerUtil : public brainSpacing { //The ManagerUtil structure for the top bar. brainSpacing is the parent
    public: //Public section

    ManagerUtil(string util_name, char ID) { //Constructor for setting the name and ID of the utility
        utilID = ID; //Sets the id
        name = util_name; //Sets the name
    }

    char getID() const { //Returns the id of the member utility
        return utilID; //Returns ID
    }

    int bottomPos() const { //Returns the bottom position of the member
        return pos.bottom; //Returns the integer
    }
    
    private: //The private section
    char utilID; //The ID of the member
};


class utilAutons : public brainSpacing { //Class that holds utility autons. Parent is brainSpacing
    public: //Public section

    utilAutons(string utilName, void (*singleCallback)()) { //The constructor for giving the name and callback
        name = utilName; //Sets the name
        callback = singleCallback; //Sets the callback
    } //Skills is assumed false by default

    utilAutons(string utilName, void (*singleCallback)(), bool is_skills) { //Alternative constructor for setting skills
        name = utilName; //Sets the name
        callback = singleCallback; //Sets the callback
        skills = is_skills; //Sets skills
    }

    void setSelected(bool state) { //Sets the selected state to the input provided
        selected = state; //Sets the state
    }

    bool isSelected() const { //Returns the current selected state of the member
        return selected; //Returns it
    }

    void (*callbackIs())() { //Returns the callback stored in the utilAutons Member
        return callback; //Returns the callback
    }

    string nameIs() const { //Returns the name of utilAutons member
        return name; //Returns the name
    }

    bool is_skills() const { //Returns the boolean of if it is skills
        return skills; //Returns the boolean
    }

    private: //Private section
    void (*callback)(); //The callback for the utilAuton
    bool selected = false; //The selected boolean
    bool skills = false; //Boolean for if it is skills. False by default
};

extern bool selectedIsBlue;

/*
 * This class does everything for the autonomous selector
 *  - Handles touches
 *  - Handles screen ID
 *  - Gets everything for the user
 *  - Generates utilities
 *  - Stores Util and Non-Util Autons
 *  - And More
*/
class AutonManager {
    public:
    
    void initialize(vector<autons> autonomousRoutines, vector<utilAutons> specialAutons, int utilityHeight) { //Initializes the manager
        if (isInit) return; //Leaves early if already initialized

        for (auto list : autonomousRoutines) { //Takes the vector provided
            autos.push_back(list); //And stores it
        }

        for (auto autos : specialAutons) { //Takes the utility autons
            utilAutos.push_back(autos); //And stores them
        }

        for (int i = 1; i <= 3; ++i) { //Creates the utility sections. Can add more by adjusting numbers
            string name = ""; //Empty string for the name
            if (i == 1) name = "Blue"; //Blue has ID of 1
            if (i == 2) name = "Red"; //Red has ID of 2
            if (i == 3) name = "Utility"; //Utility has ID of 3
            utilities.push_back(ManagerUtil(name, i)); //Adds them to utility
        }

        setupUtil(utilityHeight); //Sets the utility bar with input pixels of height
        setupMenu(utilities[0].bottomPos()); //Sets up the autons starting at the utility bottom position

        drawBG(); //Draws the background (Bottom Layer)

        pros::delay(20); //Waits so brain can catch up

        load_sd_card_save(); //Loads the SD card (sets something to true)
        drawUtilBar(); //Draws utility bar from the function it's stored in
        drawScreen(); //Draws the screen after trying to load
        isInit = true; //Updates the initialized value to true when done all of this
    }

    char getID() const { //Returns the current screen ID
        return screenID; //Returns it
    }

    void setID(char ID) { //Sets the screen ID to input ID
        screenID = ID; //Sets it
    }

    void screenTouched(int x, int y) { //Checks everything for touch identification, then redraws screen
        for (auto &u : utilities) { //Checks utilities
            if (u.containsPoint(x, y)) setID(u.getID()); //Updates ID if it should
        }

        if (screenID != 3) { //If not 3 (utility screen)
            for (auto &u : utilAutos) { //Set all utilities to false
                u.setSelected(false); //Sets to false
            }

            for (auto &a : autos) { //Check the autons
                a.setSelected(a.containsPoint(x, y)); //Checks them
            }

        } else { //If the utility screen is up
            for (auto &a : autos) { //Set all autos to false
                a.setSelected(false); //Does that
            }

            bool skillsActivated = false;
            for (auto &u : utilAutos) { //Check the utilities for touch
                u.setSelected(u.containsPoint(x, y)); //Checks them

                if (u.is_skills()&&u.isSelected()) {isSkills = true; skillsActivated = true;} //If the selected is skills update boolean
                else if (!skillsActivated) isSkills = false; //Otherwise set it to false
            }

        }

        save_sd_card(); //Saves what is currently selected to the SD card
        drawScreen(); //Redraw after being done
    }

    void store() { //Updates the manager's internal pointer to what is selected
        bool autoWasSelected = false; //Tracker for if something is selected
        for (auto &a : autos) { //Goes through the autos
            if (a.isSelected()) { //If an auto is selected
                if (getID() == 1) { //Check the screen ID
                    storedCallback = a.blue_callback(); //Get blue if screen is blue
                    selectedIsBlue = true; //Updates the global variable
                } else if (getID() == 2) { //If screen is red
                    storedCallback = a.red_callback(); //Get the red callback
                    selectedIsBlue = false; //Updates the global variable
                }
                autoWasSelected = true; //Update the internal tracker
                autoIsSelected = true; //Update the global tracker (2 variable lock)
            }
        }

        for (auto &u : utilAutos) { //Then check the utility auton's
            if (u.isSelected()) { //If utility is selected
                storedCallback = u.callbackIs(); //Set internal pointer to the callback it has
                autoWasSelected = true; //Update the internal tracker
                autoIsSelected = true; //Update the global tracker
            }
        }

        if (!autoWasSelected) { //If no auton was selected (based on internal variables)
            storedCallback = nullptr; //Reset the pointer (lock 1)
            autoIsSelected = false; //Set global tracker to false (lock 2)
        }
    }

    void autonomous() { //Runs the stored autonomous routine
        if (storedCallback&&autoIsSelected) { //If stored callback != nullptr (lock 1) and global says something is selected (lock 2)
            storedCallback(); //Run the stored callback
        } else { //Otherwise
            basicDrive(); //Run the fallback auton
        } //Basic drive will also be added to the utility screen, but it acts as a default fallback
    }

    void drawBG() { //Draws the background of the brain screen
        drawImage(); //Function that draws the background
    }

    void terminateAutons() { //Prevents further brain printing.
        terminated = true; //Updates global tracker for termination
        utilities.clear(); //Clears the utilities
        autos.clear(); //Clears the autons
        utilAutos.clear(); //Clears the utility autons
        isInit = false; //Resets this (I don't plan on init and terminating the manager multiple times, but with this it can be done)
        drawBG(); //Redraws background after
    }

    bool hasTerminated() const { //Reads the current state of the termination variable
        return terminated; //Returns the variable
    }

    string selectedAuton() { //Returns the string name of the selected autonomous
        string output = ""; //String to store the output

        if (getID() != 3) { //If not the utility screen
            getID() == 1 ? output = "Blue" : output = "Red"; //Sets it to this by default (if nothing selected, show color)
            for (const auto &a : autos) { //Goes through the autons
                if (a.isSelected()) { //If any are selected
                    output = a.nameIs(); //Set the string to the name and
                    output += getID() == 1 ? " B" : " R"; //The color tag
                }
            }
        } else { //If it is the utility screen
            output = "Utility"; //Set it to this by default (same reason as before)
            for (const auto &u : utilAutos) { //Go through the utilAutons
                if (u.isSelected()) { //If one is selected
                    output = u.nameIs() + " U"; //Set the output to the name plus the U (Utility) tag.
                }
            }
        }

        return output; //Returns the output string
    }

    bool skills_is_selected() const { //Reads if the auton selected is skills
        return isSkills; //Returns the boolean
    }

    private: //Private Section of the manager
    vector<autons> autos; //Vector to store the autons
    vector<ManagerUtil> utilities; //Vector to place utilities (color and debug)
    char screenID = 1; //1 by default
    void (*storedCallback)() = nullptr; //The callback storage
    bool autoIsSelected = true; //Global Tracker for autons. Auton does start selected by default
    bool terminated = false; //Tracker for if termination has occured
    vector<utilAutons> utilAutos; //Vector for the autons on the ManagerUtil screen
    bool isSkills = false; //Boolean to store if selected auton is skills
    bool isInit = false; //Boolean to store if the manager has been initialized
    atomic<bool> bgRedrawLock{true};

    void setupUtil(int height) { //Sets up the utility buttons, based on input height
        int xPixel = 0; //Starting xPixel
        int xSlide = (480 - (utilities.size() * 8)) / utilities.size(); //Determines spacing by total amount
        for (auto &utils : utilities) { //For loop to set the utility's positions
            xPixel += 4; //Starts by moving forward 4 (spacing)
            int topBound = 0; //Sets top to 0
            int bottomBound = height; //Sets bottom to what is wanted
            int leftBound = xPixel; //Current pixel is left bound
            xPixel += xSlide; //Moves to the right however much is needed
            int rightBound = xPixel; //Sets the right bound to current pixel
            utils.setPosition(leftBound, rightBound, topBound, bottomBound); //Sets the positions
            xPixel += 4; //Spaces out once more
        } //It does this for every utility
    }

    void setupMenu(int minVertical) { //Sets the auton's (& ManagerUtil) positions based on what order they were added. Does not draw the screen
        //Assume 2 rows per screen
        //3 screens in total (red, blue, ManagerUtil)
        int horizontalPixel = 0; //Starts at left edge
        int verticalPixel = minVertical; //Sets the upper limit to the input provided

        //Before we can start setting the positions, we need to plan out distances
        //To do that, check how many autons there are (= amount on both screens)
        int bottomRowCount = autos.size() / 2; //Integer division, truncates
        int topRowCount = autos.size() - bottomRowCount; //Will always be the rest
        int ySize = 240 - minVertical;

        int autoni = 0; //Separate index for the autons themselves

        for (int i = 0; i < topRowCount; ++i) { //Sets up the top row using my function (arguments explained later)
            horizontalPixel = setPosition(horizontalPixel, topRowCount, autos[autoni], verticalPixel, utilAutos[0], false, ySize); //Runs this, for regular autons
            ++autoni; //Indexes this as well
        }

        horizontalPixel = 0; //Reset before doing bottom row
        verticalPixel = (240 + minVertical) / 2; //Moves this to the middle

        for (int i = 0; i < bottomRowCount; ++i) { //Sets up the bottom row after setting it up
            horizontalPixel = setPosition(horizontalPixel, bottomRowCount, autos[autoni], verticalPixel, utilAutos[0], false, ySize); //Runs this
            ++autoni; //Indexes this as well
        }

        int bottomRow = utilAutos.size() / 2; //Figures out how many are needed for the utility bottom
        int topRow = utilAutos.size() - bottomRow; //And then utility top

        int utili = 0; //New index for the utility

        horizontalPixel = 0; //Resets x position
        verticalPixel = minVertical; //Resets y counter

        for (int i = 0; i < topRow; ++i) { //Makes the top row
            horizontalPixel = setPosition(horizontalPixel, topRow, autos[0], verticalPixel, utilAutos[utili], true, ySize); //Runs this for utility
            ++utili; //Indexes this as well
        }

        horizontalPixel = 0; //Reset before doing bottom row
        verticalPixel = (240 + minVertical) / 2; //Moves this to the middle

        for (int i = 0; i < bottomRow; ++i) { //Bottom row for utility
            horizontalPixel = setPosition(horizontalPixel, bottomRow, autos[0], verticalPixel, utilAutos[utili], true, ySize); //Runs this
            ++utili; //Indexes this as well
        }
    }

    int setPosition(int pixel, int totalObjects, autons& currentAuto, int verticalPixel, utilAutons& utilAuto, bool usingUtil, int availableYPixels) { //Sets the input member's position
        pixel += 4; //Adds 4 for starting offset
        int leftBound = pixel; //What the left bound is
        int horizontalDistance = (480 - (totalObjects * 8)) / totalObjects; //*8 is for spacing, then divide for the slide
        pixel += horizontalDistance; //Moves to the right edge
        int rightBound = pixel; //Right bound is now the current pixel
        pixel += 4; //Moves it right 4 again

        //Vertical Calculations
        verticalPixel += 4; //Offset
        int topBound = verticalPixel; //Top is now the current pixel
        int verticalDistance = (availableYPixels - (8 * 2)) / 2; //2 is row count
        verticalPixel += verticalDistance; //Slides that distance
        int bottomBound = verticalPixel; //Creates bottom bound
        //No need to offset vertical pixel again, since the create menu does that automatically
        //It shifts the starting y for the bottom group before doing them

        if (!usingUtil) { //Checks the input boolean for what it should set. If setting the autons
            currentAuto.setPosition(leftBound, rightBound, topBound, bottomBound); //Sets the auto positioning
        } else { //If setting the utility
            utilAuto.setPosition(leftBound, rightBound, topBound, bottomBound); //Sets the ManagerUtil positioning
        }
        return pixel; //Returns pixel so it can stack
    }

    void drawScreen() { //Draws the brain screen based on current ID and what's selected
        if (terminated) throw 1; //If already terminated, leave early.

        if (getID() == 3) { //If the screen is the utility screen

            bool expected = true; //This is what will make the background redraw, if the lock is == true
            bool desired = false; //This is what it will set the lock to when it is true (should redraw)

            if (bgRedrawLock.compare_exchange_strong(expected, desired)) { //This checks if it is true, and if so, sets it to false
                if (autos.size() != utilAutos.size()) {
                    drawBG(); //Then if it was the expected value, draw the background
                    pros::delay(20); //Delay
                    drawUtilBar(); //Redraw the utility bar (util bar doesn't need redraw besides when background is redrawn)
                }
            }

            for (auto &u : utilAutos) { //Goes through every utility Auton
                textColor = WHITE; //Sets the text color to black
                color = u.isSelected() ? YELLOW : WHITE; //And the background to yellow if selected and white otherwise
                u.drawBox(); //Then it draws the auton
            }
            //All screens auto-fill in any available space,
            //So even though the utilities probably will have less
            //With at least 2 it covers up the old screen when drawing, so no need to redraw background

        } else { //If any other screen

            bool expected = false; //This is what will make the background redraw, if the lock is == false
            bool desired = true; //This is what it will set the lock to when it is false (should redraw)

            if (bgRedrawLock.compare_exchange_strong(expected, desired)) { //For the same reason as explained above
                if (autos.size() != utilAutos.size()) {
                    drawBG(); //Draws background
                    pros::delay(20); //Refresh delay
                    drawUtilBar(); //Redraw utility bar
                }
            } //This is the opposite of what is used for the utility check

            for (auto &a : autos) { //Then it goes through every auton
                if (screenID == 1) textColor = BLUE; //If the screen is blue, set text to blue
                if (screenID == 2) textColor = RED; //If the screen is red, set text to red
            
                color = a.isSelected() ? YELLOW : BLACK; //Sets background color to yellow if selected, black if not
                a.drawBox(); //Draws the auton
            }
        }
    }

    void drawUtilBar() { //Draws the utility bar to the screen
        pros::screen::set_pen(BLACK); //Sets pen to black
        pros::screen::fill_rect(0, 0, 480, utilities[0].bottomPos()); //Draws box at the top of the screen

        for (auto &u : utilities) { //This draws the utilities
            if (u.getID() == 1) textColor = BLUE; //If blue, set text to blue
            if (u.getID() == 2) textColor = RED; //If red, set text to red
            if (u.getID() == 3) textColor = WHITE; //If utility, set text to black
            color = WHITE; //Background color is white
            u.drawBox(); //Draws the utility box
        } //They get drawn once
    }

    void load_sd_card_save() { //Reads what's stored in the SD Card to load an auton
        try { //Attempts to check (if SD card not inserted this will fail, leading to the catch statement)
            string autonName = savedName(); //Reads the auton name
            char autonID = savedID(); //Reads the ID of the auton
            bool normalAutoSelected = false; //Used for checking if it should go to utility
            for (auto &a : autos) { //For every auton stored
                a.setSelected(false); //Sets to false automatically
                if (autonName == a.nameIs()) { //If the name is correct
                    setID(autonID); //Sets the screen ID to what it should be (color correction)
                    normalAutoSelected = true; //Sets this to true (so it doesn't go to utility screen)
                    a.setSelected(true); //Sets the auton to true
                }
            }

            if (!normalAutoSelected&&autonID == 3) { //If no auton was selected and the auto was a utility auto
                setID(autonID); //Sets to screen 3
            }

            for (auto &u : utilAutos) { //Then it looks through utilAutos
                u.setSelected(false); //Sets them to false by default
                if (autonName == u.nameIs()) { //If the correct name
                    bgRedrawLock.store(!bgRedrawLock.load()); //This makes the lock default to what it should be, correcting an issue
                    u.setSelected(true); //Set it to true
                }
            }
        }
        catch (int error) { //If an exception is thrown, prevent crashes by using this
            switch (error) { //Checks the error code
                case 1: //My error I made
                printf("Error opening file"); //It prints to the console why
                break;
                default: //If not my own error
                printf("Unknown error handled"); //Prints that to console
                break;
            }
        }
    }

    void save_sd_card() { //Saves the current autonomous selected to the SD card
        try { //Try statement (to prevent crashes)
            save(); //Tries to save the auton
        }
        catch (int error) { //If it errored
            switch (error) { //Check the error code
                case 1: //If error code == 1 (my own custom)
                printf("Error opening file"); //Explain why
                break;
                default: //If not my own error
                printf("Unknown error handled"); //Print it to the console
                break;
            }
        }
    }

    void save() { //Saves the autonomous to the SD card
        ofstream autonFile("autonLog.txt"); //Opens the file (ofstream also creates the file)
        if (!autonFile.is_open()) throw 1; //If it isn't open, throw an exception
        for (auto &a : autos) { //For every auton
            string outputString = ""; //Create the output string
            if (a.isSelected()) { //If it is selected
                outputString += a.nameIs() + "\n"; //Set the name and make a new line
                if (getID() == 1) { //Then get the screen id
                    outputString += "1"; //If it is currently blue, add that
                } else if (getID() == 2) { //Otherwise
                    outputString += "2"; //If it is currently red, add that
                }
                autonFile<<outputString<<endl; //Sets to file
            }
        }

        for (auto &u : utilAutos) { //Goes through the utilities
            string outputString = ""; //Creates the string for the output
            if (u.isSelected()) { //If the utility is selected
                outputString += u.nameIs() + "\n" + "3"; //Give it name and utility tag
                autonFile<<outputString<<endl; //Save it to the file
            }
        }

        autonFile.close(); //Closes the file to save it and clean up the output buffer
    }

    string savedName() { //Returns the name of the saved auton
        string name = ""; //string to put the name into
        ifstream autonFile("autonLog.txt"); //Opens the file
        if (!autonFile.is_open()) throw 1; //If not open, throw an exception
        int line = 1; //Line counter
        while (getline(autonFile, name)) { //Goes through every line
            if (line == 1) { //The name is stored at line 1
                cout<<name; //Save the line to the string
                break; //Leave the loop (we got what was wanted)
            }
            ++line; //Indexes line after the line check
        }
        autonFile.close(); //Closes the file to keep it clean
        return name; //Returns the fetched name
    }
    
    char savedID() { //Returns the ID of the selected's screen ID
        char id = 1; //Holds the id, 1 by default
        ifstream autonFile("autonLog.txt"); //Opens the file
        if (!autonFile.is_open()) throw 1; //If not open throw exception
        int line = 1; //Line counter
        string words; //String to store what the tag is
        while (getline(autonFile, words)) { //Goes through the file
            if (line == 2) { //At line 2
                cout<<words; //Save the tag to the words string
                break; //Leave the loop
            }
            ++line; //Index the line counter
        }
        if (words == "1") { //If it the tag is 1
            id = 1; //Set the id to blue (1)
        } else if (words == "2") { //If the tag is 2
            id = 2; //Set the id to red (2)
        } else { //Otherwise (when it would be 3)
            id = 3; //Set id to utility (3)
        }
        autonFile.close(); //Closes the file to keep it safe
        return id; //Returns the found id
    }
};


#endif //End directive