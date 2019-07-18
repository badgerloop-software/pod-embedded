#include <hv_iox.h>
#include <stdio.h>
#include <stdbool.h>
#include <motor.h>
#include <data.h>
#include <transitions.h>
#include <braking.h>
#include <rms.h>


/* If there is nothing special to do */
int genTranAction() {
    return 0;
}

int toPumpdown() {
    printf("LETS GO\n");
    setMCULatch(true);
    usleep(10000);
    setMCULatch(false);
    setMCUHVEnabled(true);
    sleep(1);
    if(rmsEnHeartbeat() != 0) printf("EEERR0\n");
    if (rmsClrFaults() != 0) printf("eeE1\n");
    if(rmsInvDis() != 0) printf("EEERR2\n");
    return 0;
/*    sleep(3);*/
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
    clrMotorEn();
    usleep(1000);
    rmsCmdNoTorque();
    usleep(1000);
    rmsDischarge();
    usleep(1000);
    rmsInvDis();
    usleep(1000);
   
    setMCUHVEnabled(false);
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
