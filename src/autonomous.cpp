#include "robot/autonomous.hpp"
#include "main.h"
#include "robot/hardware.hpp"
#include "robot/reuseFunc.hpp"

// Assets
ASSET(bFarSide_txt);

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

void bFar_side_awp() {
    chassis.setPose(58.409, 37.612, 75); // Seting pos for pure pursuit
    chassis.follow(bFarSide_txt, 20 , 2000); // 20 lookahead, 2s timeout
}

void skills() {}
