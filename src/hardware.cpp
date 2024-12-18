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
lemlib::Drivetrain  drivetrain(&left_motors, &right_motors, 16, lemlib::Omniwheel::NEW_4, 600, 2);
lemlib::OdomSensors sensors(&vertical_tracking_wheel, nullptr, nullptr, nullptr, &imu);

// PID controllers
lemlib::ControllerSettings lateral_controller(10, 0, 3, 0, 0, 0, 0, 0, 0);
lemlib::ControllerSettings angular_controller(2, 0, 10, 0, 0, 0, 0, 0, 0);

// Input curves
lemlib::ExpoDriveCurve throttle_curve(3, 10, 1.019);
lemlib::ExpoDriveCurve steer_curve(3, 10, 1.019);

// Chassis
lemlib::Chassis chassis(drivetrain, lateral_controller, angular_controller, sensors, &steer_curve, &throttle_curve);
