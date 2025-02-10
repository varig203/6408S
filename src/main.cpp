#include "main.h"
#include "robodash/views/selector.hpp"
#include "robot/hardware.hpp"
#include "robot/autonomous.hpp"
#include "robot/motorControl.hpp"
#include "robot/solenoid.hpp"
#include <optional>

/**************************
 *         TODO:          *
 *  - FINISH AUTONOMOUS   *
 *    - FINISH JOURNAL    *
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

void brainDebug() {
    while (true) {
        console.printf("HEADING: ", imu.get_heading());
        console.printf("Y: ", vertical_encoder.get_position());
    }
}


// Runs initialization code when the program starts; all other competition modes are blocked, keep exec under few seconds
void initialize() {
    chassis.calibrate(); // calibrate sensors

    autonSelector.focus(); // COMMENT IN PROD
}

// Runs while the robot is disabled, following autonomous or opcontrol, and exits when the robot is enabled.
void disabled() {
    controller.print(0, 0, "Robot Disabled"); // incase the driver can't see the warning
    controller.rumble(".-.-.-.-");           // Non-verbal warning to driver
}

// Runs after initalize and before auton. only when connected to field control
void competition_initialize() {
    //autonSelector.focus(); // UNCOMMENT IN PROD
}

void autonomous() {
    autonSelector.run_auton();
}

// Runs the operator control code in its own task when the robot is enabled, stops if disabled or comms lost.
void opcontrol() {
    pros::Task chassisControl(chassis_fn);
    pros::Task solenoidControl(solenoidControl_fn);
    pros::Task motorControl(motorControl_fn);
}
