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
#include "state_machine.h"

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


/* Imports/Externs */
extern stateMachine_t stateMachine;
extern data_t pressures;

static bool checkNonBrakingPres() {
    bool noProblem = true;
    if (pressures.ps1 < PS1_BOTTOM_LIMIT || pressures.ps1 > PS1_TOP_LIMIT) {
        fprintf(stderr, "Tank pressure failing\n");
        noProblem = false;
    }
    if (pressures.ps2 < PS2_BOTTOM_LIMIT || pressures.ps2 > PS2_TOP_LIMIT) {
        fprint(stderr, "Line pressure failing\n");
        noProblem = false;
    }
    if (pressures.ps3 < PS3_BOTTOM_LIMIT || pressures.ps3 > PS3_TOP_LIMIT) {
        fprintf(stderr, "Line pressure failing\n");
        noProblem = false;
    }
    if (pressures.ps4 < PS4_BOTTOM_LIMIT || pressures.ps4 > PS4_TOP_LIMIT) {
        fprintf(stderr, "Line pressure failing\n");
        noProblem = false;
    }

    return noProblem;
}


// Thanks -Ethan <3
stateTransition_t * powerOnAction() {
            	
    return NULL;
}

stateTransition_t * idleAction() {
	
    return NULL;
}

stateTransition_t * readyForPumpdownAction() {
    if (!check_pressure() /*|| battery temp > 60C*/) {    /* Check error conditions */
        return findTransition(currState, PRE_RUN_FAULT_NAME);
    }
    
    if () /* Check if we should transition */
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
	if (/*No retro tape for 15s*/ && accel < 0.1) {
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

stateTransition_t * rebrakeAction() {
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
