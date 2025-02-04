#include "main.h"

// Reusable function as this is used everytime I want to switch a piston
void togglePistonState(bool& isExtended, pros::adi::DigitalOut& piston, int extendDelay, int retractDelay) {
    isExtended = !isExtended;
    if (isExtended) {
        piston.set_value(0);
        pros::delay(retractDelay);
    } else {
        piston.set_value(1);
        pros::delay(extendDelay);
    }
}
