#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#include "pros/rtos.hpp"

// Creating the Motor groups
pros::MotorGroup left_motors({7, 9, 8}, pros::MotorGearset::blue); // left motors on ports 7,9,8
pros::MotorGroup right_motors({-20, -18, -17}, pros::MotorGearset::blue); // right motors on ports 9,18,17
pros::Motor primary_intake(11, pros::MotorGearset::blue); // Primary Intake on port 11
pros::Motor secondary_intake(12, pros::MotorGearset::blue); // Secondary Intake on port 12

// Creating Sensors
pros::Optical optical_sensor(15); // Optical Sensor for donuts
pros::Imu imu(19); // IMU

// setting up the vertical Rotation Sensor
pros::Rotation vertical_encoder(14);
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -2.5);

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motors, // left motor group
                             &right_motors, // right motor group
                              16, // 10 inch track width
                              lemlib::Omniwheel::NEW_4, // using new 4" omnis
                              600, // drivetrain rpm is 600
                              0 // horizontal drift is 2 (for now)
);

lemlib::OdomSensors sensors(&vertical_tracking_wheel, // vertical tracking wheel 1
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              20 // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              29, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in degrees
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in degrees
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors, // odometry sensors
						&steer_curve,
						&throttle_curve
);



void on_center_button() {
  static bool pressed = false;
  pressed = !pressed;
  if (pressed) {
    pros::lcd::set_text(6, "Running Autonomous!");
    autonomous();
  } else {
    pros::lcd::clear_line(6);
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
    pros::Task screen_task([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading

            // IMU readings
            pros::lcd::print(3, "IMU Heading: %f", imu.get_heading()); // IMU heading
            pros::lcd::print(4, "Gyro Rate: %f", imu.get_gyro_rate()); // Angular velocity

            // print measurements from the rVertical Encoder
            pros::lcd::print(5, "Vertical Encoder: %i", vertical_encoder.get_position());
            //pros::lcd::print(6,"works");
			
            // delay to save resources
            pros::delay(20);
        }
    });
}

// Helper function to set the brake mode of all motors 
void setMotorBrakeMode(pros::motor_brake_mode_e mode) {
    left_motors.set_brake_mode(mode);
    right_motors.set_brake_mode(mode);
    primary_intake.set_brake_mode(mode);
    secondary_intake.set_brake_mode(mode);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {    
    // Disabling motors
    left_motors.move_velocity(0);
    right_motors.move_velocity(0);
    primary_intake.move_velocity(0);
    secondary_intake.move_velocity(0);
    
    pros::lcd::print(6, "Robot Disabled"); 
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
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task||I 
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
ASSET(test_txt); // Auto test
ASSET(test2_txt);
 
void autonomous() {
    // Changing brake mode
    setMotorBrakeMode(pros::E_MOTOR_BRAKE_COAST);

    chassis.setPose(0,0,0);
    //chassis.follow(test_txt,15,2000);
    pros::lcd::print(7, "Starting motors");
    left_motors.move_velocity(600);
    right_motors.move_velocity(600);
    pros::delay(1000);
    left_motors.move_velocity(0);
    right_motors.move_velocity(0);
    pros::lcd::print(7, "Stopping motors");
}

pros::adi::DigitalOut pistonExtend('B'); // Initialize the solenoid for extending
pros::adi::DigitalOut pistonRetract('A'); // Initialize the solenoid for retracting
pros::Controller controller(pros::E_CONTROLLER_MASTER); // Initialize controller
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

void opcontrol() {; // the semi colon for some reason lets it work DO NOT REMOVE
    // Initializing vars
    bool isExtended = false; // State variable to track piston status
    int lastButtonStateMogo = 0; // To track the last button state Intake
    bool intakeRunning = false;
    bool intakeRunningReverse = false;

    // Lambda threadding to allow the other functions to continue running while this one is doing its thing
    pros::Task mogoControl{[&]() {
        while (true) {
            int currentButtonStateMogo = controller.get_digital(DIGITAL_L1);

            // Check for button press
            if (currentButtonStateMogo && !lastButtonStateMogo) {
                isExtended = !isExtended; // Toggle the piston state

                if (isExtended) {
                    pistonRetract.set_value(0);
                    pros::delay(100);
                    pistonExtend.set_value(1);
                    isExtended = true;
                } else {
                    pistonExtend.set_value(0);
                    pros::delay(100);
                    pistonRetract.set_value(1);
                    isExtended = false;
                }

                pros::delay(500); // delay to prevent spamming the solenoids
            }
        lastButtonStateMogo = currentButtonStateMogo;
        pros::delay(20); // Saving resources
        }
    }};

    // Intake control task running on a separate thread
    pros::Task intakeControl{[&]() {

        bool lastButtonStateIntake = false;
        bool lastButtonStateIntakeReverse = false;

        // Main loop for controlling the intake
        while (true) {
            int currentButtonStateIntake = controller.get_digital_new_press(DIGITAL_R1);  // R1 for normal direction
            int currentButtonStateReverse = controller.get_digital_new_press(DIGITAL_R2); // R2 for reverse

            if (currentButtonStateIntake && !lastButtonStateIntake) {
                // Toggle the intake motor state
                intakeRunning = !intakeRunning;
            
                // If intake is running, move the motor otherwise, stop it
                if (intakeRunning) {
                    primary_intake.move_velocity(-400);  // Runs the motor at 60% power
                    secondary_intake.move_velocity(-360); // runs motor at 60% power
                    intakeRunning = true;
                } else {
                    primary_intake.move_velocity(0);  // Stops the motor
                    secondary_intake.move_velocity(0);
                    intakeRunning = false;
                }
            }

            // Basically the same as last one but the intake is reversed
            if (currentButtonStateReverse && !lastButtonStateIntakeReverse) {
                intakeRunningReverse = !intakeRunningReverse;

                if (intakeRunningReverse) {
                    primary_intake.move_velocity(400);
                    secondary_intake.move_velocity(360);
                    intakeRunning = true;
                } else {
                    primary_intake.move_velocity(0);
                    secondary_intake.move_velocity(0);
                    intakeRunning = false;
                }

            }

            // Update the previous button state for next loop iteration
            lastButtonStateIntake = currentButtonStateIntake;
            lastButtonStateIntakeReverse = currentButtonStateReverse;

            pros::delay(20); // Delay to save resources :3
        }
    }};

    pros::Task chassisTask{[&]() {
        // Chassis loop
        while (true) {
            // get left y and right x positions
            int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

            // move the robot
            chassis.arcade(leftY, -rightX);

            // delay to save resources
            pros::delay(20);
        }
    }};
}