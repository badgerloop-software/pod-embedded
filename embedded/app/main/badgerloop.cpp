// Includes
#include "HVTCPSocket.h"
#include "data_dump.h"
#include "motor.h"
#include "state_machine.h"
#include <TelemetryLoop.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "iox_rick.h"
#include "iox_morty.h"
#include "TelemetryLoop.h"


// Temp
#include <chrono>
#include <ctime>
#include <iostream>

extern "C" {
#include "NCD9830DBR2G.h"
#include "bbgpio.h"
#include "can_devices.h"
#include "connStat.h"
#include "data.h"
#include "nav.h"
#include "proc_iox.h"
#include <rms.h>
#include <signal.h>

// Software Parameter Loading
#include "load_software_parameters.h"
#include "software_parameters.h"
}


iox_rick ioxRick;
iox_morty ioxMorty;

void emergQuitter(int sig, siginfo_t* inf, void* nul)
{
    printf("shutdown\n");
    rmsCmdNoTorque();
    sleep(1);
    rmsDischarge();
    sleep(1);
    rmsInvDis();
    exit(0);
}

int init(char* directory)
{
    // Success status, return true by default
    int success_status = 0;
    printf("Initting Data...");
    /* Init Data struct */
    if (initData()) {
        success_status = 1;
        printf("FAIL\n");
    } else {
        printf("OK\n");
    }

    printf("Loading Software Params...");
    /* Load Software Parameters */
    if (loadParameters(directory, ACTIVE_RUN_PROFILE)) {
        success_status = 1;
        printf("FAIL\n");
    } else {
        printf("OK\n");
    }

    /* Init all drivers */
    SetupCANDevices();

    printf("Initting IOX 1...");
    if (ioxRick.init(true) != 0) {
        success_status = 1;
        printf("FAIL\n");
    } else {
        printf("OK\n");
    }
    printf("Initting IOX 2...");
    if (ioxMorty.init(true) != 0) {
        success_status = 1;
        printf("FAIL\n");
    } else {
        printf("OK\n");
    }

    printf("Setting up Motor...");
    SetupMotor();
    printf("OK\n");

    /*    initMotor();   */
    /*    initPressureSensors();*/

    /* Allocate needed memory for state machine and create graph */
    printf("Building State Machine...");
    buildStateMachine();
    printf("OK\n");

    /* Init telemetry */
    printf("Setting up Telemetry...");
    SetupTelemetry((char*)DASHBOARD_IP, DASHBOARD_PORT);
    SetupHVTCPServer();
    printf("OK\n");


    printf("Initting Nav...");
    initNav();
    printf("OK\n");

    struct sigaction sig;
    sig.sa_sigaction = emergQuitter;
    sigaction(SIGINT, &sig, NULL);

    /* Start 'black box' data saving */
    /* SetupDataDump(); */

    return success_status;
}

int main(int argc, char* argv[])
{
    /* Create the big data structures to house pod data */
    int i = 0;
    char buffer[100];

    if (init(argv[0]) == 1) {
        fprintf(stderr, "Error in initialization! Exiting...\r\n");
        exit(1);
    }

    while (1) {

        runStateMachine();

        /*        printf("CONN STAT: TCP - %d | UDP - %d\n", */
        /*                checkTCPStat(),*/
        /*                checkUDPStat());*/

        if (getFlagsShouldBrake()) {
            printf("signalling\n");
            signalLV((char*)"brake");
            setFlagsShouldBrake(false);
            printf("signallingDone\n");
        }
        if (getFlagsBrakeInit()) {
            printf("Cancelling brake\n");
            signalLV((char*)"primBrakeOff");
            usleep(1000);
            signalLV((char*)"secBrakeOff");
            setFlagsBrakeInit(false);
        }
        if (getFlagsClrMotionData()) {
            resetNav();
            setFlagsClrMotionData(false);
        }

        // fprintf(stderr, "%d,%d,%d\n", getRmsActualTorque(), getRmsMotorSpeed(), getuSTimestamp());
        usleep(10000);
    }
    return 0;
}
