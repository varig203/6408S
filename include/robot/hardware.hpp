#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/trackingWheel.hpp"
#include "pros/adi.hpp"
#include "pros/misc.hpp"
#include "pros/rotation.hpp"

// Motor groups
extern pros::MotorGroup left_motors;
extern pros::MotorGroup right_motors;
extern pros::Motor      primary_intake;
extern pros::Motor      secondary_intake;

// Controllers and Pistons
extern pros::Controller      rizzler;
extern pros::adi::DigitalOut pistonErectMogo;
extern pros::adi::DigitalOut pistonFlaccidMogo;
extern pros::adi::DigitalOut pistonLB;

// Sensors
extern pros::Imu             imu;
extern pros::Rotation        vertical_encoder;
extern pros::Rotation        horizontal_encoder;
extern lemlib::TrackingWheel vertical_tracking_wheel;
extern lemlib::TrackingWheel horizontal_tracking_wheel;

// Drivetrain settings
extern lemlib::Drivetrain  drivetrain;
extern lemlib::OdomSensors sensors;

// PID controllers
extern lemlib::ControllerSettings lateral_controller;
extern lemlib::ControllerSettings angular_controller;

// Input curves
extern lemlib::ExpoDriveCurve throttle_curve;
extern lemlib::ExpoDriveCurve steer_curve;

// Chassis
extern lemlib::Chassis chassis;

void initialize_hardware();

#endif // HARDWARE_HPP
