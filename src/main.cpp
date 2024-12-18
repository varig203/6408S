#include "main.h"
#include "robot/hardware.hpp"
#include <iostream>
#include <ostream>
#include "pros/rtos.hpp"
#include "pros/misc.h"
#include "pros/llemu.hpp"

void on_center_button() {
    static bool pressed = false;
    pressed             = !pressed;
    if (pressed) {
        pros::lcd::set_text(6, "Running Autonomous!");
        autonomous();
    } else {
        pros::lcd::clear_line(7);
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    pros::lcd::initialize(); // initialize brain screen
    pros::lcd::register_btn0_cb(on_center_button);
    chassis.calibrate(); // calibrate sensors

    // print debugging to brain screen
    while (true) {
        // print robot location to the brain screen
        pros::lcd::print(0, "X: %f", chassis.getPose().x);         // x
        pros::lcd::print(1, "Y: %f", chassis.getPose().y);         // y
        pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading

        // IMU readings
        pros::lcd::print(3, "IMU Heading: %f", imu.get_heading()); // IMU heading
        pros::lcd::print(4, "Gyro Rate: %f", imu.get_gyro_rate()); // Angular velocity

        // print measurements from the rVertical Encoder
        pros::lcd::print(5, "Vertical Encoder: %i", vertical_encoder.get_position());

        // Shows the driver what comp status the bot is in
        controller.print(0, 0, "STATUS: %c", pros::competition::get_status());

        // delay to save resources
        pros::delay(20);
    }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    controller.print(1, 0, "Robot Disabled"); // incase the driver can't see the previous warning
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
void competition_initialize() {}

void solenoidControl_fn() {                                   // Controls all the solenoids on the robot in a single task
    std::cout << "Solenoid Control Starting..." << std::endl; // Print into PROS Terminal for debugging

    // Initializing vars
    bool isExtended          = false; // State variable to track piston status
    bool isExtendedLB        = false; // Lady brown mech
    int  lastButtonStateMogo = 0;     // To track the last button state Intake
    int  lastButtonStateLB   = 0;     // Lady brown mech

    while (true) {
        int currentButtonStateMogo = controller.get_digital_new_press(DIGITAL_L1); // Mogo mech
        int currentButtonStateLB   = controller.get_digital_new_press(DIGITAL_L2); //  LB Mech

        // Check for button press
        if (currentButtonStateMogo && !lastButtonStateMogo) {
            isExtended = !isExtended; // Toggle the piston state

            if (isExtended) {
                pistonRetractMogo.set_value(0);
                pros::delay(100);
                pistonExtendMogo.set_value(1);
                isExtended = true;
            } else {
                pistonExtendMogo.set_value(0);
                pros::delay(75);
                pistonRetractMogo.set_value(1);
                isExtended = false;
            }
        } else if (currentButtonStateLB && !lastButtonStateLB) {
            isExtendedLB = !isExtendedLB; // Togle thing

            if (isExtendedLB) {
                pistonLB.set_value(0);
                pros::delay(100);
            } else {
                pistonLB.set_value(1);
                pros::delay(100);
            }
        }
        lastButtonStateMogo = currentButtonStateMogo;

        pros::delay(20); // Saving resources
    }
}

void motorControl_fn() {                                   // Controls both Intake motors and drivetrain motors
    std::cout << "Motor Control Starting..." << std::endl; // Print into PROS Terminal for debugging

    while (true) {
        // Read controller inputs
        int IntakeForward  = controller.get_digital(DIGITAL_R2);
        int IntakeBackward = controller.get_digital(DIGITAL_R1);
        int leftY          = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);  // Up and down on the left stick
        int rightX         = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // Left and right on the right stick

        // Drivetrain control
        chassis.arcade(leftY, rightX);

        if (IntakeForward) {
            primary_intake.move_velocity(-400); // spinny thingy forward (Out take)
            secondary_intake.move_velocity(-400);
        } else if (IntakeBackward) {
            primary_intake.move_velocity(400); // spinny thingy backward
            secondary_intake.move_velocity(400);
        } else {
            primary_intake.move_velocity(0); // spinny thingy stop
            secondary_intake.move_velocity(0);
        }

        pros::delay(20);
    }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

void opcontrol() {
    std::cout << "Starting functions..." << std::endl;
    pros::Task solenoidControl(solenoidControl_fn);
    pros::Task motorControl(motorControl_fn);
    std::cout << "Finished!" << std::endl;
}