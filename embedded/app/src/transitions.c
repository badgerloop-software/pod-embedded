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
/* Gen == general */
int genIdle() {
    data->flags->brakeInit = true;
    if (rmsCmdNoTorque() != 0) fprintf(stderr, "Failed in genIdle, 1\n");
    usleep(50000);
    if (rmsDischarge() != 0) fprintf(stderr, "Failed in genIdle, 2\n");
    usleep(50000);
    if (rmsInvDis() != 0) fprintf(stderr, "Failed in genIdle, 3\n");
    return 0;
}

int genPumpdown() {
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
}

int toPropulsion() {
/*    if (startMotor() != 0) return 1;*/
/*    setTorque(FULL_SPEED_AHEAD);*/
    data->timers->startTime = getuSTimestamp();
    
    /* FIXME  I need a way to tell if this was successful */    
    return 0;
}



int toBraking() {
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

    setMotorCrawl(); 
    data->timers->crawlTimer = getuSTimestamp();
    
    return 0;
}

int toPostRun() {
    setMCUHVEnabled(0);
    brakeHV();
    
    return 0;
}

int toServPrecharge() {
    data->flags->brakeInit = true;
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
}

int toRunFault() {
    clrMotorEn();
    usleep(1000);
    rmsCmdNoTorque();
    usleep(1000);
    rmsDischarge();
    usleep(1000);
    rmsInvDis();
    usleep(1000);
 
    setMCUHVEnabled(0);
    brakeHV();
    return 0;
}

int toNonRunFault() {
    clrMotorEn();
    usleep(1000);
    rmsCmdNoTorque();
    usleep(1000);
    rmsDischarge();
    usleep(1000);
    rmsInvDis();
    usleep(1000);
 
    setMCUHVEnabled(0);
    return 0;
}
