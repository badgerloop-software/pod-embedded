/***
 *  Filename: states.c
 *
 *  Summary: All of the in-depth functionality of each state. Each state has its
 *  own actions and transitions and this is where all of the states actions are
 *  defined. It also has a number of helper functions to simplify the states
 *  actions.
 *
 */

/* Includes */
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "bms_fault_checking.h"
#include "connStat.h"
#include "data.h"
#include "hv_iox.h"
#include "motor.h"
#include "pressure_fault_checking.h"
#include "rms.h"
#include "rms_fault_checking.h"
#include "software_parameters.h"
#include "state_machine.h"
#include "states.h"
#include <math.h>
/*#define NO_FAULT*/

#define NO_DASHBOARD

#define NUM_FAIL 4
#define LV_BATT_SOC_CALC(x) (pow(-1.1142 * (x), 6) + pow(78.334 * (x), 5) - pow(2280.5 * (x), 4) + pow(35181.0 * (x), 3) - pow(303240.0 * (x), 2) - (1000000.0 * (x)))

#define MAX_PACKET_LOSS 500
/* Imports/Externs */
extern int internalCount;
extern stateMachine_t stateMachine;
extern stateTransition_t* findTransition(state_t* currState, char* name);
int bErrs, pErrs, rErrs;
int checkNetwork()
{
    #ifndef NO_DASHBOARD
    static int errs = 0;
    if (!checkUDPStat() || !checkTCPStat()) {
        if (checkUDPStat() == 0)
            fprintf(stderr, "UDP ");
        if (checkTCPStat() == 0)
            fprintf(stderr, "TCP ");
        fprintf(stderr, "CONNECTION DROPPED\n");
        errs += 1;
    } else {
        errs = 0;
    }

    if (errs >= MAX_PACKET_LOSS) {
        return -1;
    }
    #endif
    return 0;
}

/***
 * checkStopped - checks a variety of values to make sure the pod is stopped.
 *
 * RETURNS: true if stopped, false if moving
 */
/* DEPRECATED
static bool checkStopped(void) {
	return fabs(getMotionVel()) < MAX_STOPPED_VEL &&  (getuSTimestamp() - getTimersLastRetro()) > TIME_SINCE_LAST_RETRO;
}
*/
/***
 * Actions for all the states.
 * They perform transition and error condition
 * checking and perform the functionality of the state: e.g. brake if in
 * braking.
 *
 */

stateTransition_t* idleAction()
{
    setDataState(1);

    if (getFlagsEmergencyBrake()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    return NULL;
}

stateTransition_t* pumpdownAction()
{
    // First check for nominal values?
    setDataState(2);

    /* Check IMD status */

    if (getuSTimestamp() - stateMachine.start > PUMPDOWN_TIMEOUT) {
        fprintf(stderr, "[FAULT] EXCEEDED PUMPDOWN TIMEOUT OF %d USECONDS\n", PUMPDOWN_TIMEOUT);
        fprintf(stderr, "\tTOOK %d USECONDS\n", getuSTimestamp() - stateMachine.start);
        return stateMachine.currState->fault;
    }

    if (getFlagsEmergencyBrake()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    // CHECK PRESSURE
    if (!checkPrerunPressures()) {
        fprintf(stderr, "Pressure failure\n");
        pErrs += 1;
    } else
        pErrs = 0;

    if (!checkPrerunBattery()) {
        fprintf(stderr, "prerun batt fault\n");
        bErrs += 1;
    } else
        bErrs = 0;

    if (!checkPrerunRMS()) {
        fprintf(stderr, "prerun rms fault\n");
        rErrs += 1;
    } else
        rErrs = 0;
    /* FIXME, need to fix robust failure cases */
    if (pErrs >= NUM_FAIL || bErrs >= NUM_FAIL || rErrs >= NUM_FAIL)
        return stateMachine.currState->fault;

    return NULL;
}

stateTransition_t* propulsionAction()
{
    setDataState(3);
    /* Check IMD status */
    if (getFlagsEmergencyBrake()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    /* Check HV Indicator light */

    if (checkNetwork() != 0) {
        fprintf(stderr, "[FAULT] LOST NETWORK CONNECTION");
        return stateMachine.currState->fault;
    }

    // CHECK FAULT CRITERIA
    // CHECK PRESSURE -- PreRun function still valid here
    if (!checkPrerunPressures()) {
        fprintf(stderr, "Pressure failing\n");
        pErrs += 1;
    } else
        pErrs = 0;

    if (!checkRunBattery()) {
        printf("Failed battery\n");
        bErrs += 1;
    } else
    bErrs = 0;

    if (!checkRunRMS()) {
        printf("run rms failed\n");
        rErrs += 1;
    } else
    rErrs = 0;

    if (getuSTimestamp() - getTimersStartTime() > MAXIMUM_RUN_TIME) {
        fprintf(stderr, "Prop timeout\n");
        return stateMachine.currState->next;
    }

    if (getMotionRetroCount() >= RUN_RETRO_COUNT && getFlagsReadyToBrake()) {
        printf("retro transition\n");
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }

    // CHECK TRANSITION CRITERIA
    if (getFlagsShouldStop()) {
        printf("Should Stop\n");
        return stateMachine.currState->next;
    }
    fprintf(stderr, "BMS ERRS: %d\t PRESSURE ERRS: %d\t RMS ERRS: %d\n", bErrs, pErrs, rErrs);
    if (bErrs >= NUM_FAIL || pErrs >= NUM_FAIL || rErrs >= NUM_FAIL)
        return stateMachine.currState->fault;

    return NULL;
}

stateTransition_t* brakingAction()
{
    setDataState(4);
    // TODO Do we differenciate between primary and secondary braking systems?
    // TODO Add logic to handle switches / actuate both

    if (getFlagsEmergencyBrake()) {
        printf("EMERG BRAKE\n");
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    if (!getIMDStatus()) {
        fprintf(stderr, "getIMDStatus()");
        return stateMachine.currState->fault;
    }

    if (checkNetwork() != 0) {
        printf("lost connection\n");
        return stateMachine.currState->fault;
    }

    // CHECK FAULT CRITERIA

    if (getuSTimestamp() - stateMachine.start > BRAKING_CHECK_PRESSURE_TIMEOUT) {
        if (!checkBrakingPressures()) {
            printf("braking==================\n");
            pErrs += 1;
        } else
            pErrs = 0;
    }

    if (!checkBrakingBattery()) {
        printf("battery bad\n");
        bErrs += 1;
    } else
        bErrs = 0;

    if (getuSTimestamp() - stateMachine.start > BRAKING_CHECK_RMS_TIMEOUT) {
        if (!checkBrakingRMS()) {
            printf("rms bad\n");
            rErrs += 1;
        } else
            rErrs = 0;
    }

    if ((getuSTimestamp() - stateMachine.start) > BRAKING_TRANSITION_STOPPED_TIMEOUT) {
        printf("going to stopped\n");
        return findTransition(stateMachine.currState, STOPPED_NAME);
    }

    if (pErrs >= NUM_FAIL || bErrs >= NUM_FAIL || rErrs >= NUM_FAIL) {
        fprintf(stderr, "NUM FAILS: pErrs - %d | bErrs - %d | rErrs - %d\n", pErrs, bErrs, rErrs);

        return stateMachine.currState->fault;
    }

    return NULL;
}

stateTransition_t* stoppedAction()
{
    setDataState(5);
    /* Check IMD status */
    if (getFlagsEmergencyBrake()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    if (checkNetwork() != 0)
        return stateMachine.currState->fault;
    // CHECK FAULT CRITERIA

    if (!checkBrakingPressures()) { // Still unchanged
        printf("Pressures failing\n");
        pErrs += 1;
    } else
        pErrs = 0;

    if (!checkBrakingBattery()) { // Still unchanged
        fprintf(stderr, "Battery error\n");
        bErrs += 1;
    } else
        bErrs = 0;

    if (!checkStoppedRMS()) { // Still unchanged
        printf("failrms\n");
        rErrs += 1;
    } else
        rErrs = 0;

    if (bErrs >= NUM_FAIL || pErrs >= NUM_FAIL || rErrs >= NUM_FAIL)
        return stateMachine.currState->fault;

    return NULL;
}

stateTransition_t* servPrechargeAction()
{
    setDataState(6);
    if (!checkBrakingPressures()) {
        fprintf(stderr, "Pressure failed\n");
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    } else
        pErrs = 0;

    if (!getIMDStatus()) {
        fprintf(stderr, "getIMDStatus()");
        return stateMachine.currState->fault;
    }
    if (getFlagsEmergencyBrake()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    if (checkNetwork() != 0)
        return stateMachine.currState->fault;
    return NULL;
}

static int prevRet = 0;

stateTransition_t* crawlAction()
{
    setDataState(7);
    /* Check IMD status */
    prevRet = getMotionRetroCount();
    if (!getIMDStatus()) {
        return stateMachine.currState->fault;
    }

    if (checkNetwork() != 0)
        return stateMachine.currState->fault;

    if (getFlagsEmergencyBrake()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    if ((getMotionRetroCount() - internalCount) >= CRAWL_RETRO_COUNT) {
        printf("retro transition\n");
        return findTransition(stateMachine.currState, POST_RUN_NAME);
    }

    if (!checkCrawlPostrunPressures()) {
        printf("pres fail\n");
        pErrs += 1;
    } else
        pErrs = 0;

    if (!checkCrawlBattery()) {
        printf("batt fail\n");
        bErrs += 1;
    } else
        bErrs = 0;

    if (!checkCrawlRMS()) { // Still unchanged
        printf("rms fail\n");
        rErrs += 1;
    } else
        rErrs = 0;

    // CHECK TRANSITION CRITERIA

    if (getuSTimestamp() - stateMachine.start > MAXIMUM_CRAWL_TIME) {
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }

    if (getFlagsShouldStop()) {

        return findTransition(stateMachine.currState, POST_RUN_NAME);
    }

    // printf("PRIM LINE: %f\n", getPressurePrimLine());
    if (bErrs >= NUM_FAIL || pErrs >= NUM_FAIL || rErrs >= NUM_FAIL) {
        printf("fail\n");
        return stateMachine.currState->fault;
    }

    return NULL;
}

stateTransition_t* postRunAction()
{
    setDataState(8);
    /* Check IMD status */
    if (getFlagsEmergencyBrake()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    // TODO fixme there is a seg fault around here lol
    /* Check HV Indicator light */
    // printf("FAILURE STATE: %p\n", stateMachine.currState);

    if (checkNetwork() != 0)
        findTransition(stateMachine.currState, RUN_FAULT_NAME);

    if (!checkPostrunBattery()) {
        printf("battfail\n");
        bErrs += 1;
    } else
        bErrs = 0;

    if (!checkPostRMS()) {
        printf("rmsfail\n");
        rErrs += 1;
    } else
        rErrs = 0;

    if (bErrs >= NUM_FAIL || rErrs >= NUM_FAIL) {
        printf("postFail\n");
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    return NULL;
}

stateTransition_t* safeToApproachAction()
{
    /*    if (isHVEnabled()) {*/
    /*        return stateMachine.currState->fault;*/
    /*    } */
    if (getPressurePrimTank() > 30 || getPressurePrimTank() < -15 || getPressurePrimLine() > 20 || getPressurePrimLine() < -10 || getPressurePrimAct() > 20 || getPressurePrimAct() < -10 || getPressureSecTank() > 30 || getPressureSecTank() < -15 || getPressureSecLine() > 20 || getPressureSecLine() < -10 || getPressureSecAct() > 20 || getPressureSecAct() < -10 || getPressurePv() > 20 || getPressurePv() < 13) {
        fprintf(stderr, "Pressures are out of the safe range\n");
        return findTransition(stateMachine.currState, NON_RUN_FAULT_NAME);
    }

    if (checkNetwork() != 0)
        return findTransition(stateMachine.currState, NON_RUN_FAULT_NAME);
    if (getFlagsEmergencyBrake()) {
        return findTransition(stateMachine.currState, NON_RUN_FAULT_NAME);
    }
    setDataState(9);
    return NULL;
}
//
//  We're removing pre and post faults and making them non run faults.
// When you change this make 11 the non run fault action. Ty - EU
stateTransition_t* nonRunFaultAction()
{
    setFlagsEmergencyBrake(false);
    fprintf(stderr, "NON RUN FAULT\n");
    static int mcuDisPulse = 0;
    if (mcuDisPulse >= 50) {
        clrMotorEn();
        usleep(1000);
        rmsCmdNoTorque();
        usleep(1000);
        rmsDischarge();
        usleep(1000);
        rmsInvDis();
        usleep(1000);
        setMCUHVEnabled(0);
        mcuDisPulse = 0;
    } else {
        mcuDisPulse += 1;
    }
    setDataState(10);
    return NULL;
}

stateTransition_t* runFaultAction()
{
    printf("RUN FAULT\n");
    setFlagsEmergencyBrake(false);
    static int mcuDisPulse = 0;
    setDataState(11);
    if (mcuDisPulse >= 50) {
        setMCUHVEnabled(0);
        clrMotorEn();
        usleep(1000);

        rmsCmdNoTorque();
        usleep(1000);
        rmsDischarge();
        usleep(1000);
        rmsInvDis();
        usleep(1000);
        mcuDisPulse = 0;
    } else {
        mcuDisPulse += 1;
    }

    return NULL;
}

stateTransition_t* brakingFault()
{
    //TODO
    setDataState(12);
    return NULL;
}
