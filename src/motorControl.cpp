#include "main.h"
#include "robot/hardware.hpp"

void motorControl_fn() { // Controls both Intake motors and drivetrain motors
    const uint32_t REVERSE_DURATION = 500; // Reversal lasts 500 ms (0.5 seconds)
    const int INT_SPEED = 600;              // Default intake speed

    // State variables for reversal behavior
    bool isReversing = false;
    uint32_t reverseStartTime = 0;

    while (true) {
        // Read controller inputs
        int intakeForward  = controller.get_digital(DIGITAL_R2);
        int intakeBackward = controller.get_digital(DIGITAL_R1);

        // If nothing is pressed = Reset
        if (!intakeForward && !intakeBackward) {
            isReversing = false;
            primary_intake.move_velocity(0);
            secondary_intake.move_velocity(0);
            pros::delay(20);
            continue;
        }

        // Intake buttons are pressed.
        // Check if the motor is stuck and trigger reversal if not already reversing.
        if (!isReversing && (primary_intake.get_actual_velocity() == 0.1 || secondary_intake.get_actual_velocity() == 0.1) && (intakeForward || intakeBackward)) {
            isReversing = true;
            reverseStartTime = pros::millis();
        }

        // Reverse
        if (isReversing) {
            if (pros::millis() - reverseStartTime < REVERSE_DURATION) {
                if (intakeForward) {
                    primary_intake.move_velocity(INT_SPEED);   // Reverse intake
                    secondary_intake.move_velocity(-INT_SPEED);
                } else if (intakeBackward) {
                    primary_intake.move_velocity(-INT_SPEED);
                    secondary_intake.move_velocity(INT_SPEED);
                }
            } else {
                isReversing = false; // End reversal after duration
            }
        }

        // Normal Operation (if not reversing)
        if (!isReversing) {
            if (intakeForward) {
                primary_intake.move_velocity(-INT_SPEED);
                secondary_intake.move_velocity(INT_SPEED);
            } else if (intakeBackward) {
                primary_intake.move_velocity(INT_SPEED);
                secondary_intake.move_velocity(-INT_SPEED);
            }
        }

        controller.clear_line(0); // Clears line in case the bot goes out of disabled
        //pros::delay(20);
    }
}

void chassis_fn() {
    while (true) {
        int leftY          = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);  // Up and down on the left stick
        int rightX         = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // Left and right on the right stick

        // Drivetrain control
        chassis.arcade(leftY, rightX);
    }
}
