#include <stdio.h>
#include <stdbool.h>
#include <hv_iox.h>
#include <transitions.h>
#include <state_machine.h>
#include <motor.h>
#include <rms.h>

extern "C"
{
#include <data.h>
#include <braking.h>
}

extern stateMachine_t stateMachine;
int internalCount = 0;
/* If there is nothing special to do */
int genTranAction()
{
    return 0;
}
/* Gen == general */
int genIdle()
{
    setFlagsBrakeInit(true);
    if (rms.rmsCmdNoTorque() != 0)
        fprintf(stderr, "Failed in genIdle, 1\n");
    usleep(50000);
    if (rms.rmsDischarge() != 0)
        fprintf(stderr, "Failed in genIdle, 2\n");
    usleep(50000);
    if (rms.rmsInvDis() != 0)
        fprintf(stderr, "Failed in genIdle, 3\n");
    hv_iox.setMCUHVEnabled(false);
    return 0;
}

int genPumpdown()
{
    usleep(10000);
    hv_iox.setMCULatch(true);
    usleep(10000);
    hv_iox.setMCULatch(false);
    usleep(10000);
    hv_iox.setMCUHVEnabled(true);
    sleep(1);
    if (rms.rmsEnHeartbeat() != 0)
        printf("EEERR0\n");
    if (rms.rmsClrFaults() != 0)
        printf("eeE1\n");
    if (rms.rmsInvDis() != 0)
        printf("EEERR2\n");
    stateMachine.start = getuSTimestamp();
    return 0;
}

int genPropulsion()
{
    uint64_t timestamp = getuSTimestamp();
    setTimersStartTime(timestamp);
    stateMachine.start = timestamp;
    setFlagsClrMotionData(true);
    setMotorEn();
    /* FIXME  I need a way to tell if this was successful */
    //
    // Wait for a timeout to see success?
    //
    //////////////////////////
    return 0;
}

int genBraking()
{
    printf("BRAKING!\n");
    clrMotorEn();
    if (getRmsDcBusVoltage() > 60)
    {
        usleep(50000);
        rms.rmsCmdNoTorque();
        usleep(50000);
        rms.rmsDischarge();
        usleep(50000);
        rms.rmsInvDis();
        usleep(50000);
    }
    hv_iox.setMCUHVEnabled(false);

    brakeHV();
    stateMachine.start = getuSTimestamp();
    return 0;
}

int genStopped()
{
    if (getRmsDcBusVoltage() > 60)
    {
        rms.rmsCmdNoTorque();
        rms.rmsDischarge();
        rms.rmsInvDis();
    }
    printf("Proper stopper\n");
    setFlagsBrakeInit(true);
    return 0;
}

int genCrawl()
{
    printf("gen crawl\n");
    setMotorCrawl();
    internalCount = getMotionRetroCount();
    setTimersCrawlTimer(getuSTimestamp());
    stateMachine.start = getuSTimestamp();
    return 0;
}

int genPostRun()
{
    clrMotorEn();
    if (getRmsDcBusVoltage() > 60)
    {
        usleep(1000);
        rms.rmsCmdNoTorque();
        usleep(1000);
        rms.rmsDischarge();
        usleep(1000);
        rms.rmsInvDis();
        usleep(1000);
    }
    hv_iox.setMCUHVEnabled(0);
    brakeHV();

    return 0;
}

int genServPrecharge()
{
    printf("PRE CHARGE\n");
    hv_iox.setMCULatch(true);
    usleep(10000);
    hv_iox.setMCULatch(false);
    hv_iox.setMCUHVEnabled(true);
    sleep(1);
    if (rms.rmsEnHeartbeat() != 0)
        printf("EEERR0\n");
    if (rms.rmsClrFaults() != 0)
        printf("eeE1\n");
    if (rms.rmsInvDis() != 0)
        printf("EEERR2\n");
    return 0;
}

int genRunFault()
{
    printf("Entering here\n");
    clrMotorEn();
    usleep(1000);
    rms.rmsCmdNoTorque();
    printf("Entering here2\n");
    usleep(1000);
    rms.rmsDischarge();
    printf("Entering here3\n");
    usleep(1000);
    rms.rmsInvDis();
    usleep(1000);

    hv_iox.setMCUHVEnabled(0);
    printf("Entering here4\n");
    brakeHV();
    printf("Entering here\n");
    return 0;
}

int genNonRunFault()
{
    printf("non run0\n");
    clrMotorEn();
    printf("non run0\n");
    usleep(1000);
    rms.rmsCmdNoTorque();
    printf("non run0\n");
    usleep(1000);
    rms.rmsDischarge();
    printf("non run0\n");
    usleep(1000);
    rms.rmsInvDis();
    usleep(1000);
    hv_iox.setMCUHVEnabled(0);
    printf("non run0\n");
    return 0;
}
