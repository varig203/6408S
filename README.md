# V5 6048S | High Stakes 2024-2025

This bot code includes multithreading and mechanisms for solenoid control and chassis and intake control. The version of everything included is PROS 4.1.1 and LemLib 0.5.5

## Structure

src/main.cpp contains everything used at runtime.
src/autonomous.cpp contains all of the autonomous routines.
src/hardware.cpp contains the hardware definitions and configuration.
src/reuseFunc.cpp contains the reusable functions.
src/Master-Selector is everything for the auton selector.

include/Master-Selector is for the auton selector headers.
include/robot is for the headers included in src/

## Licensing

- Overall the project is under an MIT license.
- The src/Master-Selector and include/Master-Selector directories contain code adapted to work on PROS 4.1.1 from [TheMaster3558's project](https://github.com/TheMaster3558/Master-Selector), and retains its original license with modifications to reflect updates by [Daniel T-B](https://github.com/varig203).
