#include "robot/autonomous.hpp"
#include "main.h"
#include "robot/hardware.hpp"

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

void rNear_side_awp() {}

void rFar_side_awp() {}

void bNear_side_awp() {}

void bFar_side_awp() {}

void skills() {}
