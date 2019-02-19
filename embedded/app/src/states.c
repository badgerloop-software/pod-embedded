/***
 *  Filename: states.c
 *
 *  Summary: All of the in-depth functionality of each state. Each state has its
 *  own actions and transitions and this is where all of the states actions are
 *  defined. It also has a number of helper functions to simplify the states
 *  actions.
 *
 *  RE: Ezra Boley, ...
 */

/* Includes */
#include <stdout.h>
#include "state_machine.h"
#include "data.h"


/* Defines */

/* Pressure sensor acceptable limits (in PSI) */
#define PS1_BOTTOM_LIMIT        1000
#define PS1_TOP_LIMIT           3000
#define PS2_BOTTOM_LIMIT        1000
#define PS2_TOP_LIMIT           3000
#define PS3_BOTTOM_LIMIT        1000
#define PS3_TOP_LIMIT           3000
#define PS4_BOTTOM_LIMIT        0
#define PS4_TOP_LIMIT           20
/**/

#define MAX_BATT_TEMP			60	/* Degrees Celcius */
#define MAX_STOPPED_ACCEL		0.3 /* Limit for qualifying the pod as stopped in m/s */


/* Imports/Externs */

extern stateMachine_t stateMachine;
extern data_t *data;
int timer;
static bool checkPrimPressures(void);
static bool checkStopped(void);


/***
 * checkPrimPressures - Compares the readings from the pressure sensors on
 * the primary brakes agaainst expected values. Applicable only for pre-braking
 * pressures
 *
 * RETURNS: true if everything is ok, false if there is an issue
 */

static bool checkPrimPressures(void) {
    bool noProblem = true;
    if (data->pressure->ps1 < PS1_BOTTOM_LIMIT || data->pressure->ps1 > PS1_TOP_LIMIT) {
        fprintf(stderr, "Tank pressure failing\n");
        noProblem = false;
    }
    if (data->pressure->ps2 < PS2_BOTTOM_LIMIT || data->pressure->ps2 > PS2_TOP_LIMIT) {
        fprint(stderr, "Line pressure failing\n");
        noProblem = false;
    }
    if (data->pressures->ps3 < PS3_BOTTOM_LIMIT || data->pressure->ps3 > PS3_TOP_LIMIT) {
        fprintf(stderr, "Line pressure failing\n");
        noProblem = false;
    }
    if (data->pressure->ps4 < PS4_BOTTOM_LIMIT || data->pressure->ps4 > PS4_TOP_LIMIT) {
        fprintf(stderr, "Line pressure failing\n");
        noProblem = false;
    }

    return noProblem;
}

/***
 * checkStopped - checks a variety of values to make sure the pod is stopped.
 *
 * RETURNS: true if stopped, false if moving
 */

static bool checkStopped(void) {
	if (data->motion->accel < MAX_STOPPED_ACCEL || timer == 0) {

	}
}

/***
 * Actions for all the states.
 * They perform transition and error condition
 * checking and perform the functionality of the state: e.g. brake if in
 * braking.
 *
 */

stateTransition_t * powerOffAction() {
    return findTransition(currState, IDLE_NAME);
}

stateTransition_t * idleAction() {
	if (!checkPrimPressures() || checkStopped()
    return NULL;
}

stateTransition_t * readyForPumpdownAction() {
   	/* Check error conditions */
    if (!checkPrimPressures() || data->bms->highTemp > MAX_BATT_TEMP) {
		return findTransition(currState, PRE_RUN_FAULT_NAME);
    }
	/* Check if we should transition */
    if () {

	}
    return NULL;
}

stateTransition_t * pumpDownAction() {
	return NULL;
}

stateTransition_t * readyForLaunchAction() {
	return NULL;
}

stateTransition_t * propulsionAction() {
	return NULL;
}

stateTransition_t * brakingAction() {
    if (/* 30s timer has hit */ || /* post_run button pushed */);
        /* transition to post run */
	if (/*No retro tape for 15s*/ && data->motion->accel < 0.1) {
        /* Start a timer to transition in 30 s */
    }

    return NULL;
}

stateTransition_t * stoppedAction() {
	return NULL;
}

stateTransition_t * crawlAction() {

	return NULL;
}

stateTransition_t * postRunAction() {
	return NULL;
}

stateTransition_t * safeToApproachAction() {
	return NULL;
}

stateTransition_t * secondaryBrakingAction() {
	return NULL;
}

stateTransition_t * preFaultAction() {
	return NULL;
}

stateTransition_t * runFaultAction() {
	return NULL;
}

stateTransition_t * postFaultAciton() {
	return NULL;
}
