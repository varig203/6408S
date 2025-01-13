#include "main.h"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "robot/hardware.hpp"
#include "robot/reuseFunc.hpp"
#include "robot/autonSelector.hpp"

// void brainScreen_fn() {
//     // print debugging to brain screen
//     while (true) {
//         // print robot location to the brain screen
//         pros::lcd::print(0, "X: %f", chassis.getPose().x);         // x
//         pros::lcd::print(1, "Y: %f", chassis.getPose().y);         // y
//         pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading

//         // IMU readings
//         pros::lcd::print(3, "IMU Heading: %f", imu.get_heading()); // IMU heading
//         pros::lcd::print(4, "Gyro Rate: %f", imu.get_gyro_rate()); // Angular velocity

//         // print measurements from the rVertical Encoder
//         pros::lcd::print(5, "Vertical Encoder: %i", vertical_encoder.get_position());

//         // delay to save resources
//         pros::delay(20);
//     }
// }

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

void motorControl_fn() { // Controls both Intake motors and drivetrain motors
    while (true) {
        // Read controller inputs
        int IntakeForward  = controller.get_digital(DIGITAL_R2);
        int IntakeBackward = controller.get_digital(DIGITAL_R1);
        int leftY          = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);  // Up and down on the left stick
        int rightX         = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // Left and right on the right stick

        // Drivetrain control
        chassis.arcade(leftY, rightX);

        if (IntakeForward) {
            primary_intake.move_velocity(-600); // spinny thingy forward (Out take)
            secondary_intake.move_velocity(-600);
        } else if (IntakeBackward) {
            primary_intake.move_velocity(600); // spinny thingy backward
            secondary_intake.move_velocity(600);
        } else {
            primary_intake.move_velocity(0); // spinny thingy stop
            secondary_intake.move_velocity(0);
        }
        controller.clear_line(0); // Clears line in case the bot goes out of disabled
        pros::delay(20);
    }
}

// Runs initialization code when the program starts; all other competition modes are blocked, keep exec under few seconds
void initialize() {
    chassis.calibrate(); // calibrate sensors
    pros::Task autonSelector(create_auton_selector);
}

// Runs while the robot is disabled, following autonomous or opcontrol, and exits when the robot is enabled.
void disabled() {
    controller.print(0, 0, "Robot Disabled"); // incase the driver can't see the warning
    controller.rumble("...");                 // Non-verbal warning to driver
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
    //pros::Task autonSelector(autonSelector_fn);
}

// Runs the operator control code in its own task when the robot is enabled, stops if disabled or comms lost.
void opcontrol() {
    pros::Task solenoidControl(solenoidControl_fn);
    pros::Task motorControl(motorControl_fn);
}
