#include "main.h"
#include "robot/solenoid.hpp"
#include "robot/reuseFunc.hpp"
#include "robot/hardware.hpp"

void solenoidControl_fn() { // Controls all the solenoids on the robot in a single task
    // Initializing vars
    bool isExtendedMogo      = false; // State variable to track piston status
    bool isExtendedLB        = false; // Lady brown mech
    int  lastButtonStateMogo = 0;     // To track the last button state Intake
    int  lastButtonStateLB   = 0;     // Lady brown mech

    while (true) {
        int currentButtonStateMogo = controller.get_digital_new_press(DIGITAL_L1); // Mogo mech
        int currentButtonStateLB   = controller.get_digital_new_press(DIGITAL_L2); //  LB Mech

        // Check for button press
        if (currentButtonStateMogo && !lastButtonStateMogo) {
            togglePistonState(isExtendedMogo, pistonMogo, 100, 75);
        }

        // Same as above but for ladybrown
        if (currentButtonStateLB && !lastButtonStateLB) {
            togglePistonState(isExtendedLB, pistonLB, 50, 50);
        }
        // Resetting value
        lastButtonStateMogo = currentButtonStateMogo;
        lastButtonStateLB   = currentButtonStateLB;

        pros::delay(20); // Saving resources
    }
}
