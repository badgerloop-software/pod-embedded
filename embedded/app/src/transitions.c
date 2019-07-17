#include <stdio.h>
#include <stdbool.h>
#include <motor.h>
#include <data.h>
#include <transitions.h>
#include <braking.h>

/* If there is nothing special to do */
int genTranAction() {
    return 0;
}

int toPropulsion() {
/*    if (startMotor() != 0) return 1;*/
/*    setTorque(FULL_SPEED_AHEAD);*/
    data->timers->startTime = getuSTimestamp();
    /* FIXME  I need a way to tell if this was successful */    
    return 0;
}

int toBraking() {
/*    if (stopMotor() != 0) return 1;*/
    brakeHV();
    return 0;
}

int toCrawl() {
    brakePrimaryUnactuate();
    brakeSecondaryUnactuate();    /* Usually doesnt do anything */

    data->timers->crawlTimer = getuSTimestamp();

    if (startMotor() != 0) return 1;
    setTorque(CRAWL_TORQUE);
    return 0;
}

int toPostRun() {
    stopMotor();
    setMCUHVEnabled(0);
    brakeHV();
    
    return 0;
}

int toRunFault() {
    setMCUHVEnabled(0);
    brakeHV();
    return 0;
}
