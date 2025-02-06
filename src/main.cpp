#include "main.h"
#include "robot/hardware.hpp"
#include "robot/autonomous.hpp"
#include "robot/motorControl.hpp"
#include "robot/solenoid.hpp"

    rd::Selector autonSelector({  // Auton Selecton
        {"Red Ring AWP", rFar_side_awp},
        {"Red Goal AWP", rNear_side_awp},
        {"Blue Ring AWP", bFar_side_awp},
        {"Blue Goal AWP", bNear_side_awp},
        {"Skills", skills}
    });

// Runs initialization code when the program starts; all other competition modes are blocked, keep exec under few seconds
void initialize() {
    chassis.calibrate(); // calibrate sensors
}

// Runs while the robot is disabled, following autonomous or opcontrol, and exits when the robot is enabled.
void disabled() {
    controller.print(0, 0, "Robot Disabled"); // incase the driver can't see the warning
    controller.rumble(".-.-.-.-");           // Non-verbal warning to driver
}

void autonomous() {
    autonSelector.run_auton();
}

// Runs after initalize and before auton. only when connected to field control
void competition_initialize() {

}

// Runs the operator control code in its own task when the robot is enabled, stops if disabled or comms lost.
void opcontrol() {
    pros::Task chassisControl(chassis_fn);
    pros::Task solenoidControl(solenoidControl_fn);
    pros::Task motorControl(motorControl_fn);
}
