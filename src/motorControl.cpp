#include "main.h"
#include "robot/hardware.hpp"

void motorControl_fn() {
    // Constants: reversal duration and stuck threshold (ms), and motor speed.
    const uint32_t REVERSE_DURATION = 500, STUCK_THRESHOLD = 500;
    const int SPEED = 600;

    // State variables.
    bool isReversing = false, isStuck = false;
    uint32_t reverseStart = 0, stuckStart = 0;

    while (true) {
        // Cache the current time for efficiency.
        uint32_t now = pros::millis();
        bool intakeForward = controller.get_digital(DIGITAL_R2);
        bool intakeBackward = controller.get_digital(DIGITAL_R1);

        // If no button is pressed, reset states and stop motors.
        if (!intakeForward && !intakeBackward) {
            isReversing = isStuck = false;
            primary_intake.move_velocity(0);
            secondary_intake.move_velocity(0);
        }
        // If in reversal mode, run motors in reverse for REVERSE_DURATION.
        else if (isReversing) {
            if (now - reverseStart < REVERSE_DURATION) {
                if (intakeForward) {
                    primary_intake.move_velocity(SPEED);
                    secondary_intake.move_velocity(-SPEED);
                } else if (intakeBackward) {
                    primary_intake.move_velocity(-SPEED);
                    secondary_intake.move_velocity(SPEED);
                }
            } else {
                isReversing = isStuck = false;
            }
        }
        // Normal operation: check for stuck condition and run intake normally.
        else {
            // Check if either motor's velocity is zero.
            if (primary_intake.get_actual_velocity() == 0 || secondary_intake.get_actual_velocity() == 0) {
                if (!isStuck) {
                    isStuck = true;
                    stuckStart = now;
                } else if (now - stuckStart >= STUCK_THRESHOLD) {
                    isReversing = true;
                    reverseStart = now;
                }
            } else {
                isStuck = false;
            }
            // Run motors normally if not reversing.
            if (!isReversing) {
                if (intakeForward) {
                    primary_intake.move_velocity(-SPEED);
                    secondary_intake.move_velocity(SPEED);
                } else if (intakeBackward) {
                    primary_intake.move_velocity(SPEED);
                    secondary_intake.move_velocity(-SPEED);
                }
            }
        }
        controller.clear_line(0);
        pros::delay(20);
    }
}

void chassis_fn() {
    while (true) {
        int leftY  = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);  // Up and down on the left stick
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // Left and right on the right stick

        // Drivetrain control
        chassis.arcade(leftY, rightX);
        pros::delay(20);
    }
}
