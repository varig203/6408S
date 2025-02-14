#include "main.h"
#include "robodash/views/selector.hpp"
#include "robot/hardware.hpp"
#include "robot/autonomous.hpp"
#include "robot/motorControl.hpp"
#include "robot/solenoid.hpp"

/**************************
 *         TODO:          *
 *  - FINISH AUTONOMOUS   *
 *     - FINISH ROBOT     *
 * - FIX POTENTIAL ISSUES *
 **************************/

rd::Selector autonSelector({  //  Initalize Auton Selecton
    {"Red Ring AWP", rFar_side_awp},
    {"Red Goal AWP", rNear_side_awp},
    {"Blue Ring AWP", bFar_side_awp},
    {"Blue Goal AWP", bNear_side_awp},
    {"Skills", skills}
});

rd::Console console; // Initalize RD console

// Runs initialization code when the program starts; all other competition modes are blocked, keep exec under few seconds
void initialize() {
    chassis.calibrate(); // calibrate sensors

    autonSelector.focus();
}

// Runs while the robot is disabled, following autonomous or opcontrol, and exits when the robot is enabled.
void disabled() {
    controller.print(0, 0, "Robot Disabled"); // incase the driver can't see the warning
    controller.rumble(".-.-.-.-");           // Non-verbal warning to driver
}

// Runs after initalize and before auton. only when connected to field control
void competition_initialize() {}

void autonomous() {
    left_motors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    right_motors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    autonSelector.run_auton();
}

// Runs the operator control code in its own task when the robot is enabled, stops if disabled or comms lost.
void opcontrol() {
    pros::Task chassisControl(chassis_fn);
    pros::Task solenoidControl(solenoidControl_fn);
    pros::Task motorControl(motorControl_fn);
}
