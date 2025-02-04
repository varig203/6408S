#include "main.h"
#include "robot/hardware.hpp"
#include "robot/reuseFunc.hpp"
#include "robot/autonomous.hpp"
#include "robot/motorControl.hpp"

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

// Runs initialization code when the program starts; all other competition modes are blocked, keep exec under few seconds
void initialize() {
    chassis.calibrate(); // calibrate sensors

    // Initialize the Auton Selector
    ms::set_autons({
        ms::Category("Red Side", {
            ms::Auton("Near side AWP", rNear_side_awp),
            ms::Auton("Far side AWP", rFar_side_awp)
        }),
        ms::Category("Blue Side", {
            ms::Auton("Near side AWP", bNear_side_awp),
            ms::Auton("Far side AWP", bFar_side_awp)
        }),
        ms::Category("Skills", {
            ms::Auton("Skills", skills)
        })
    });
    ms::initialize(); // Comment out when in prod
}

// Runs while the robot is disabled, following autonomous or opcontrol, and exits when the robot is enabled.
void disabled() {
    controller.print(0, 0, "Robot Disabled"); // incase the driver can't see the warning
    controller.rumble(".-.-.-.-");           // Non-verbal warning to driver
}

void autonomous() {
    ms::call_selected_auton(); // Run selected auton routine
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
    //ms::initialize(); // Comment out when debugging
}

// Runs the operator control code in its own task when the robot is enabled, stops if disabled or comms lost.
void opcontrol() {
    pros::Task solenoidControl(solenoidControl_fn);
    pros::Task motorControl(motorControl_fn);
}
