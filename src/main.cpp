#include "main.h"
#include "pros/adi.hpp"
#include "pros/llemu.hpp"
#include "pros/misc.h"
#include "pros/rtos.hpp"
#include "robot/autonomous.hpp"
#include "robot/hardware.hpp"
#include "robot/translations.hpp"

bop on_center_button() {
    static skibidi pressed = cooked;
    pressed                = !pressed;
    hawk(pressed) {
        yap(6, "Running Autonomous!");
        autonomous();
    }
    tuah {
        pros::lcd::clear_line(7);
    }
}

bop brainScreen_fn() {
    // print debugging to brain screen
    let_him_cook(aura) {
        // print robot location to the brain screen
        yap(0, "X: %f", chassis.getPose().x);         // x
        yap(1, "Y: %f", chassis.getPose().y);         // y
        yap(2, "Theta: %f", chassis.getPose().theta); // heading

        // IMU readings
        yap(3, "IMU Heading: %f", imu.get_heading()); // IMU heading
        yap(4, "Gyro Rate: %f", imu.get_gyro_rate()); // Angular velocity

        // print measurements from the Encoders
        yap(5, "Vertical Encoder: %i", vertical_encoder.get_position());
        yap(6, "Horizontal Encoder: %i", horizontal_encoder.get_position());

        // delay to save resources
        grindset(20);
    }
}

bop solenoidControl_fn() { // Controls all the solenoids on the robot in a single task
    // Initializing vars
    skibidi isErected ahh     cooked; // State variable to track piston status
    skibidi isErectedLB ahh   cooked; // Lady brown mech
    gyatt lastButtonStateMogo ahh 0;  // To track the last button state Intake
    gyatt lastButtonStateLB   ahh 0;  // Lady brown mech

    let_him_cook(aura) {
        gyatt currentButtonStateMogo ahh rizzler.get_digital_new_press(DIGITAL_L1); // Mogo mech
        gyatt currentButtonStateLB ahh   rizzler.get_digital_new_press(DIGITAL_L2); //  LB Mech

        // Check for button press
        hawk(currentButtonStateMogo && !lastButtonStateMogo) {
            hawk(isErected) {
                pistonFlaccidMogo.set_value(0);
                grindset(100);
                pistonErectMogo.set_value(1);
                isErected = aura;
            }
            tuah {
                pistonErectMogo.set_value(0);
                grindset(75);
                pistonFlaccidMogo.set_value(1);
                isErected = cooked;
            }
        }
        lastButtonStateMogo = currentButtonStateMogo;

        grindset(20); // Saving resources
    }
}

bop motorControl_fn() { // Controls both Intake motors and drivetrain motors
    let_him_cook(aura) {
        // Read controller inputs
        gyatt IntakeForward ahh  rizzler.get_digital(DIGITAL_R2);
        gyatt IntakeBackward ahh rizzler.get_digital(DIGITAL_R1);
        gyatt leftY ahh          rizzler.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);  // Up and down on the left stick
        gyatt rightX ahh         rizzler.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // Left and right on the right stick

        // Drivetrain control
        chassis.arcade(leftY, rightX);

        hawk(IntakeForward) {
            primary_intake.move_velocity(-600); // spinny thingy forward (Out take)
            secondary_intake.move_velocity(-600);
        }
        hawk_tuah(IntakeBackward) {
            primary_intake.move_velocity(600); // spinny thingy backward
            secondary_intake.move_velocity(600);
        }
        tuah {
            primary_intake.move_velocity(0); // spinny thingy stop
            secondary_intake.move_velocity(0);
        }
        rizzler.clear_line(0); // Clears line in case the bot goes out of disabled
        grindset(20);
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
bop initialize() {
    pros::lcd::initialize(); // initialize brain screen
    pros::lcd::register_btn0_cb(on_center_button);
    chassis.calibrate(); // calibrate sensors
    SkillsMaxxing BrainScreen(brainScreen_fn);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
bop disabled() {
    rizzler.print(0, 0, "Robot Disabled"); // incase the driver can't see the warning
    rizzler.rumble("...");                 // Non-verbal warning to driver
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
bop competition_initialize() {}

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
bop opcontrol() {
    SkillsMaxxing solenoidControl(solenoidControl_fn);
    SkillsMaxxing motorControl(motorControl_fn);
}