#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

// Creating the Motor groups
pros::MotorGroup left_motors({9, 19}, pros::MotorGearset::blue); // left motors on ports 1, 11
pros::MotorGroup right_motors({-1, -11}, pros::MotorGearset::blue); // right motors on ports 9, 19

// Creating the IMU port
pros::Imu imu(20);

// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motors, // left motor group
                             &right_motors, // right motor group
                              16, // 10 inch track width
                              lemlib::Omniwheel::NEW_4, // using new 4" omnis
                              600, // drivetrain rpm is 600
                              2 // horizontal drift is 2 (for now)
);

lemlib::OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
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
    pros::lcd::set_text(7, "Running Autonomous!");
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
    pros::adi::DigitalOut piston ('C'); // Initializing the solenoids
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

            // print measurements from the adi encoder
            pros::lcd::print(5, "ADI Encoder: %i", vertical_encoder.get_value());
			
            // delay to save resources
            pros::delay(20);
        }
    });
}


/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

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
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
    pros::adi::DigitalOut piston ('C'); // Initializing the solenoids
    // set position to x:0, y:0, heading:0
    chassis.setPose(0, 0, 0);
    // turn to face heading 90 with a very long timeout
    chassis.turnToHeading(90, 100000);

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
/** void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::MotorGroup left_mg({1, -2, 3});    // Creates a motor group with forwards ports 1 & 3 and reversed port 2
	pros::MotorGroup right_mg({-4, 5, -6});  // Creates a motor group with forwards port 5 and reversed ports 4 & 6


	while (true) {
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		left_mg.move(dir - turn);                      // Sets left motor voltage
		right_mg.move(dir + turn);                     // Sets right motor voltage
		pros::delay(20);                               // Run for 20 ms then update
	}
} 
*/
pros::Controller controller(pros::E_CONTROLLER_MASTER);

void opcontrol() {
    // loop forever
    pros::adi::DigitalOut pistonExtend('C'); // Initialize the solenoid for extending
    pros::adi::DigitalOut pistonRetract('D'); // Initialize the solenoid for retracting
    bool isExtended = false; // State variable to track piston status
    int lastButtonState = 0; // To track the last button stat

    while (true) {
        // get left y and right x positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

        // move the robot
        chassis.arcade(leftY, -rightX);
        //chassis.curvature(leftY, -rightX);
        
        int currentButtonState = controller.get_digital(DIGITAL_L1);

        // Check for button press
        if (currentButtonState && !lastButtonState) {
            if (isExtended) {
                // Retract the piston
                pistonExtend.set_value(0);
                pistonRetract.set_value(1);
            } else {
                // Extend the piston
                pistonExtend.set_value(1);
                pistonRetract.set_value(0);
            }
            // Toggle the state
            isExtended = !isExtended;
        }

        // Update the last button state
        lastButtonState = currentButtonState;

        // If not actuated, ensure both solenoids are off
        if (!isExtended) {
            pistonExtend.set_value(0);
            pistonRetract.set_value(0);
        }

        // delay to save resources
        pros::delay(25);
    }
}