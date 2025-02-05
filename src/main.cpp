#include "main.h"
#include "robot/hardware.hpp"
#include "robot/autonomous.hpp"
#include "robot/motorControl.hpp"
#include "robot/solenoid.hpp"

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
    //ms::initialize(); // Comment out when in prod
}

// Runs while the robot is disabled, following autonomous or opcontrol, and exits when the robot is enabled.
void disabled() {
    controller.print(0, 0, "Robot Disabled"); // incase the driver can't see the warning
    controller.rumble(".-.-.-.-");           // Non-verbal warning to driver
}

void autonomous() {
    ms::call_selected_auton(); // Run selected auton routine
}

// Runs after initalize and before auton. only when connected to field control
void competition_initialize() {
    //ms::initialize(); // Comment out when debugging
}

// Runs the operator control code in its own task when the robot is enabled, stops if disabled or comms lost.
void opcontrol() {
    pros::Task chassisControl(chassis_fn);
    pros::Task solenoidControl(solenoidControl_fn);
    pros::Task motorControl(motorControl_fn);
}
