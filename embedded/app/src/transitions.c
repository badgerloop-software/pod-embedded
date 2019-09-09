#include <hv_iox.h>
#include <stdio.h>
#include <stdbool.h>
#include <motor.h>
#include <data.h>
#include <transitions.h>
#include <state_machine.h>
#include <braking.h>
#include <rms.h>

extern stateMachine_t stateMachine;
int internalCount = 0;
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
    setMCUHVEnabled(false);
    return 0;
}

int genPumpdown() {
    usleep(10000);
    setMCULatch(true);
    usleep(10000);
    setMCULatch(false);
    usleep(10000);
    setMCUHVEnabled(true);
    sleep(1);
    if(rmsEnHeartbeat() != 0) printf("EEERR0\n");
    if (rmsClrFaults() != 0) printf("eeE1\n");
    if(rmsInvDis() != 0) printf("EEERR2\n");
    stateMachine.start = getuSTimestamp();
    return 0;
}

int genPropulsion() {
    stateMachine.start = data->timers->startTime = getuSTimestamp();
    data->flags->clrMotionData = true;
    setMotorEn();
    /* FIXME  I need a way to tell if this was successful */    
    //
    // Wait for a timeout to see success?
    //
    //////////////////////////
    return 0;
}


int genBraking() {
    printf("BRAKING!\n");
    clrMotorEn();
    if (data->rms->dcBusVoltage > 60) {
        usleep(50000);
        rmsCmdNoTorque();
        usleep(50000);
        rmsDischarge();
        usleep(50000);
        rmsInvDis();
        usleep(50000);
    }   
    setMCUHVEnabled(false);
    
    brakeHV();
    stateMachine.start = getuSTimestamp();
    return 0;
}

int genStopped() {
    if (data->rms->dcBusVoltage > 60) {
        rmsCmdNoTorque();
        rmsDischarge();
        rmsInvDis();
    }
    printf("Proper stopper\n");
    data->flags->brakeInit = true;
    return 0;
}



int genCrawl() {
    printf("gen crawl\n");
    setMotorCrawl(); 
    internalCount = data->motion->retroCount;
    data->timers->crawlTimer = getuSTimestamp();
    
    stateMachine.start = getuSTimestamp();    
    return 0;
}

int genPostRun() {
    clrMotorEn();
    if (data->rms->dcBusVoltage > 60) {
        usleep(1000);
        rmsCmdNoTorque();
        usleep(1000);
        rmsDischarge();
        usleep(1000);
        rmsInvDis();
        usleep(1000);
    }
    setMCUHVEnabled(0);
    brakeHV();
    
    return 0;
}

int genServPrecharge() {
    printf("PRE CHARGE\n");
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


int genRunFault() {
    printf("Entering here\n");
    clrMotorEn();
    usleep(1000);
    rmsCmdNoTorque();
    printf("Entering here2\n");
    usleep(1000);
    rmsDischarge();
    printf("Entering here3\n");
    usleep(1000);
    rmsInvDis();
    usleep(1000);
 
    setMCUHVEnabled(0);
    printf("Entering here4\n");
    brakeHV();
    printf("Entering here\n");
    return 0;
}

int genNonRunFault() {
    printf("non run0\n");
    clrMotorEn();
    printf("non run0\n");
    usleep(1000);
    rmsCmdNoTorque();
    printf("non run0\n");
    usleep(1000);
    rmsDischarge();
    printf("non run0\n");
    usleep(1000);
    rmsInvDis();
    usleep(1000);
    setMCUHVEnabled(0);
    printf("non run0\n");
    return 0;
}
