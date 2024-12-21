#include "robot/hardware.hpp"

// Motor groups
pros::MotorGroup left_motors({-14, -15, -19}, pros::MotorGearset::blue); // left motors on ports 14, 15, 19
pros::MotorGroup right_motors({13, 16, 17}, pros::MotorGearset::blue);   // right motors on ports 13, 16, 17
pros::Motor      primary_intake(20, pros::MotorGearset::blue);           // Primary Intake on port 20
pros::Motor      secondary_intake(18, pros::MotorGearset::blue);         // Secondary Intake on port 18

// Controller and Pistons
pros::Controller      controller(pros::E_CONTROLLER_MASTER); // Initialize controller
pros::adi::DigitalOut pistonExtendMogo('A');                 // Initialize the solenoid for extending MOGO
pros::adi::DigitalOut pistonRetractMogo('B');                // Initialize the solenoid for retracting MOGO
pros::adi::DigitalOut pistonLB('C');                         // Initialize the LB mech

// Sensors
pros::Imu             imu(12); // IMU
pros::Rotation        vertical_encoder(11);
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -2.5);

// Drivetrain settings
lemlib::Drivetrain  drivetrain(&left_motor_group,          // left motor group
                               &right_motor_group,         // right motor group
                               16,                         // 10 inch track width
                               lemlib::Omniwheel::NEW_325, // using new 4" omnis
                               600,                        // drivetrain rpm is 360
                               2                           // horizontal drift is 2 (for now)
 );
lemlib::OdomSensors sensors(&vertical_tracking_wheel, // Vertical tracking wheel
                            nullptr,                  // 2nd Vertical Tracking Wheel
                            nullptr,                  // Horizontal Tracking wheel
                            nullptr,                  // 2nd Horizontal tracking wheel
                            &imu                      // IMU Sensor
);

// PID controllers
lemlib::ControllerSettings angular_controller(2,  // proportional gain (kP)
                                              0,  // integral gain (kI)
                                              10, // derivative gain (kD)
                                              0,  // anti windup
                                              0,  // small error range, in inches
                                              0,  // small error range timeout, in milliseconds
                                              0,  // large error range, in inches
                                              0,  // large error range timeout, in milliseconds
                                              0   // maximum acceleration (slew)
);
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0,  // integral gain (kI)
                                              3,  // derivative gain (kD)
                                              0,  // anti windup
                                              0,  // small error range, in inches
                                              0,  // small error range timeout, in milliseconds
                                              0,  // large error range, in inches
                                              0,  // large error range timeout, in milliseconds
                                              0   // maximum acceleration (slew)
);

// Input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttle_curve(3,    // joystick deadband out of 127
                                      10,   // minimum output where drivetrain will move out of 127
                                      1.019 // expo curve gain
);

// Input curve for steer input during driver control
lemlib::ExpoDriveCurve steer_curve(3,    // joystick deadband out of 127
                                   10,   // minimum output where drivetrain will move out of 127
                                   1.019 // expo curve gain
);

// Create the chassis
lemlib::Chassis chassis(drivetrain,
                        lateral_controller,
                        angular_controller,
                        sensors,
                        &throttle_curve,
                        &steer_curve);