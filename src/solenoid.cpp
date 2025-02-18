#include "main.h"
#include "robot/solenoid.hpp"
#include "pros/misc.h"
#include "robot/reuseFunc.hpp"
#include "robot/hardware.hpp"

void solenoidControl_fn() { // Controls all the solenoids on the robot in a single task
    // Initializing vars
    bool isExtendedMogo       = true; // State variable to track piston status
    bool isExtendedLB         = false; // Lady brown mech
    bool isExtendedDoink      = false; // Doinker
    int  lastButtonStateMogo  = 0;     // To track the last button state Intake
    int  lastButtonStateLB    = 0;     // Lady brown mech
    int  lastButtonStateDoink = 0;     // doinker

    while (true) {
        int currentButtonStateMogo  = controller.get_digital(DIGITAL_L1); // Mogo mech
        int currentButtonStateLB    = controller.get_digital_new_press(DIGITAL_L2); //  LB Mech
        int currentButtonStateDoink = controller.get_digital_new_press(DIGITAL_A);

        // Check for button press
        if (currentButtonStateMogo && !lastButtonStateMogo) {
            togglePistonState(isExtendedMogo, pistonMogo, 100, 75);
        }

        // Same as above but for ladybrown
        if (currentButtonStateLB && !lastButtonStateLB) {
            togglePistonState(isExtendedLB, pistonLB, 50, 50);
        }

        if (currentButtonStateDoink && !lastButtonStateDoink) {
            togglePistonState(isExtendedDoink, pistonDoink, 50, 50);
        }
        // Resetting value
        lastButtonStateMogo  = currentButtonStateMogo;
        lastButtonStateLB    = currentButtonStateLB;
        lastButtonStateDoink = currentButtonStateDoink;

        pros::delay(20); // Saving resources
    }
}
