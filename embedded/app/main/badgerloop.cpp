// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <TelemetryLoop.h>
#include "HVTCPSocket.h"
#include "data_dump.h"
#include "hv_iox.h"

// Temp
#include <chrono>
#include <ctime>
#include <iostream>

extern "C"
{
#include "bbgpio.h"
#include "connStat.h"
#include <signal.h>
#include <rms.h>
#include "motor.h"
#include "motor.h"
#include "proc_iox.h"
#include "data.h"
#include "can_devices.h"
#include "state_machine.h"
#include "NCD9830DBR2G.h"
}

HVIox hv_iox;


void emergQuitter(int sig, siginfo_t *inf, void *nul)
{
    printf("shutdown\n");
    hv_iox.setMCUHVEnabled(false);
    rmsCmdNoTorque();
    sleep(1);
    rmsDischarge();
    sleep(1);
    rmsInvDis();
    exit(0);
}

int init()
{
    /* Init Data struct */
    initData();

    /* Init all drivers */
    SetupCANDevices();
    initProcIox(true);
    hv_iox.init(true);

    SetupMotor();

    /*    initMotor();   */
    /*    initPressureSensors();*/

    /* Allocate needed memory for state machine and create graph */
    buildStateMachine();

    /* Init telemetry */
    SetupTelemetry((char *)DASHBOARD_IP, DASHBOARD_PORT);
    SetupHVTCPServer();

    struct sigaction sig;
    sig.sa_sigaction = emergQuitter;
    sigaction(SIGINT, &sig, NULL);

    /* Start 'black box' data saving */
    /*    SetupDataDump();*/

    return 0;
}

int main()
{
    /* Create the big data structures to house pod data */
    int i = 0;
    char buffer[100];

    if (init() == 1)
    {
        fprintf(stderr, "Error in initialization! Exiting...\r\n");
        exit(1);
    }

    while (1)
    {

        runStateMachine();

        /*        printf("CONN STAT: TCP - %d | UDP - %d\n", */
        /*                checkTCPStat(),*/
        /*                checkUDPStat());*/

        if (getFlagsShouldBrake())
        {
            printf("signalling\n");
            signalLV((char *)"brake");
            setFlagsShouldBrake(false);
            printf("signallingDone\n");
        }
        if (getFlagsBrakeInit())
        {
            printf("Cancelling brake\n");
            signalLV((char *)"primBrakeOff");
            usleep(1000);
            signalLV((char *)"secBrakeOff");
            setFlagsBrakeInit(false);
        }
        if (getFlagsClrMotionData())
        {
            printf("signal clear\n");
            signalLV((char *)"clrMotion");
            setFlagsClrMotionData(false);
        }

        if (i >= 50)
        {
            sprintf(buffer, "state%d\n", getDataState() == 1);
            signalLV((char *)buffer);
            i = 0;
        }
        else
        {
            i += 1;
        }
        // fprintf(stderr, "%d,%d,%d\n", getRmsActualTorque(), getRmsMotorSpeed(), getuSTimestamp());
        usleep(10000);
    }
    return 0;
}
