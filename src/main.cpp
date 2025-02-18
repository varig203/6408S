#include "main.h"
#include "robodash/views/selector.hpp"
#include "robot/hardware.hpp"
#include "robot/autonomous.hpp"
#include "robot/motorControl.hpp"
#include "robot/solenoid.hpp"
#include "robot/reuseFunc.hpp"

/****************************************************************
 * THIS PROGRAM IS FOR THE FINISHED 6408S (NOT QUALIFIED) TEAM. *
 * WE PLACED 7TH IN THE QUALS MATCHES AT WPRA HIGH STEAKS GAME  *
 *                       ON FEB 17, 2025                        *
 ****************************************************************/

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

    autonSelector.focus(); // Focuses Auton selector.
}

// Runs while the robot is disabled, following autonomous or opcontrol, and exits when the robot is enabled.
void disabled() {
    controller.print(0, 0, "Robot Disabled"); // incase the driver can't see the warning
    controller.rumble(".-.-.-.-");           // Non-verbal warning to driver
}

// Runs after initalize and before auton. only when connected to field control
void competition_initialize() {}

// Quick and dirty auton
// DO NOT USE FOR REAL COMPETITIONS
// USE PURE PURSUIT WHEN YOU CAN
void autonomous() {
    left_motors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    right_motors.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    //autonSelector.run_auton(); // Runs selected auton
    right_motors.move_velocity(-300);
    left_motors.move_velocity(-300);
    pros::delay(1000);
    left_motors.move_velocity(0);
    right_motors.move_velocity(0);
    pistonMogo.set_value(1);
    secondary_intake.move_velocity(-600);
    pros::delay(3000);
    secondary_intake.move_velocity(-0);
}

// Runs the operator control code in its own task when the robot is enabled, stops if disabled or comms lost.
void opcontrol() {
    // These just run the functions on seperate threads for async
    pros::Task chassisControl(chassis_fn);
    pros::Task solenoidControl(solenoidControl_fn);
    pros::Task motorControl(motorControl_fn);
}
