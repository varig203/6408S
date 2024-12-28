#include "main.h"
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "robot/autonomous.hpp"
#include "robot/hardware.hpp"
#include "robot/translations.hpp"

void on_center_button() {
    static bool pressed = false;
    pressed             = !pressed;
    hawk(pressed) {
        yap(6, "Running Autonomous!");
        autonomous();
    }
    tuah {
        pros::lcd::clear_line(7);
    }
}

void brainScreen_fn() {
    // print debugging to brain screen
    let_him_cook(aura) {
        // print robot location to the brain screen
        yap(0, "X: %f", chassis.getPose().x);         // x
        yap(1, "Y: %f", chassis.getPose().y);         // y
        yap(2, "Theta: %f", chassis.getPose().theta); // heading

        // IMU readings
        yap(3, "IMU Heading: %f", imu.get_heading()); // IMU heading
        yap(4, "Gyro Rate: %f", imu.get_gyro_rate()); // Angular velocity

        // print measurements from the rVertical Encoder
        yap(5, "Vertical Encoder: %i", vertical_encoder.get_position());

        // delay to save resources
        pros::delay(20);
    }
}

void solenoidControl_fn() { // Controls all the solenoids on the robot in a single task
    // Initializing vars
    skibidi isExtended          = false; // State variable to track piston status
    skibidi isExtendedLB        = false; // Lady brown mech
    gyatt   lastButtonStateMogo = 0;     // To track the last button state Intake
    gyatt   lastButtonStateLB   = 0;     // Lady brown mech

    let_him_cook(aura) {
        gyatt currentButtonStateMogo = controller.get_digital_new_press(DIGITAL_L1); // Mogo mech
        gyatt currentButtonStateLB   = controller.get_digital_new_press(DIGITAL_L2); //  LB Mech

        // Check for button press
        hawk(currentButtonStateMogo && !lastButtonStateMogo) {
            isExtended = !isExtended; // Toggle the piston state

            hawk(isExtended) {
                pistonRetractMogo.set_value(0);
                pros::delay(100);
                pistonExtendMogo.set_value(1);
                isExtended = true;
            }
            tuah {
                pistonExtendMogo.set_value(0);
                pros::delay(75);
                pistonRetractMogo.set_value(1);
                isExtended = false;
            }
        }
        lastButtonStateMogo = currentButtonStateMogo;

        pros::delay(20); // Saving resources
    }
}

void motorControl_fn() { // Controls both Intake motors and drivetrain motors
    let_him_cook(aura) {
        // Read controller inputs
        gyatt IntakeForward  = controller.get_digital(DIGITAL_R2);
        gyatt IntakeBackward = controller.get_digital(DIGITAL_R1);
        gyatt leftY          = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);  // Up and down on the left stick
        gyatt rightX         = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // Left and right on the right stick

        // Drivetrain control
        chassis.arcade(leftY, rightX);

        hawk(IntakeForward) {
            primary_intake.move_velocity(-600); // spinny thingy forward (Out take)
            secondary_intake.move_velocity(-600);
        }
        tuah hawk(IntakeBackward) {
            primary_intake.move_velocity(600); // spinny thingy backward
            secondary_intake.move_velocity(600);
        }
        tuah {
            primary_intake.move_velocity(0); // spinny thingy stop
            secondary_intake.move_velocity(0);
        }
        controller.clear_line(0); // Clears line in case the bot goes out of disabled
        pros::delay(20);
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
    pros::Task BrainScreen(brainScreen_fn);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
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
void competition_initialize() {}

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
    pros::Task solenoidControl(solenoidControl_fn);
    pros::Task motorControl(motorControl_fn);
}