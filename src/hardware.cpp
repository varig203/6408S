#include "robot/hardware.hpp"

/*
// Subsystems
- Drivetrain left  = 4, 5, 6
- Drivetrain right = 1, 2, 3
- Primary intake   = 7
- Secondary intake = 8

- Mogo      = A
- Ladybrown = B

// Sensors
- IMU = 17
- Vertical Encoder   = 18
- Horizontal Encoder = 19
- Optical Sensor     = 20
- Radio              = 21
*/

// Motor groups
pros::MotorGroup left_motors({-5, -6, -7}, pros::MotorGearset::blue); // left motors on ports 4, 5, 6
pros::MotorGroup right_motors({1, 2, 3}, pros::MotorGearset::blue);   // right motors on ports 1, 2, 3
pros::Motor      primary_intake(5, pros::MotorGearset::blue);            // Primary Intake on port 7
pros::Motor      secondary_intake(4, pros::MotorGearset::blue);          // Secondary Intake on port 8

// Controller and Pistons
pros::Controller      controller(pros::E_CONTROLLER_MASTER); // Initialize controller
pros::adi::DigitalOut pistonMogo('A');                       // Initialize the solenoid for MOGO mech
pros::adi::DigitalOut pistonLB('B');                         // Initialize the LB mech

// Sensors
pros::Imu             imu(17); // IMU
pros::Rotation        vertical_encoder(18);
pros::Rotation        horizontal_encoder(19);
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_2, -2.5);
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_2, 0);

// Drivetrain settings
lemlib::Drivetrain  drivetrain(&left_motors,               // left motor group
                               &right_motors,              // right motor group
                               16,                         // 10 inch track width
                               lemlib::Omniwheel::NEW_325, // using new 4" omnis
                               600,                        // drivetrain rpm is 360
                               2                           // horizontal drift is 2 (for now)
 );
lemlib::OdomSensors sensors(&vertical_tracking_wheel,   // Vertical tracking wheel
                            nullptr,                    // 2nd Vertical Tracking Wheel
                            &horizontal_tracking_wheel, // Horizontal Tracking wheel
                            nullptr,                    // 2nd Horizontal tracking wheel
                            &imu                        // IMU Sensor
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
