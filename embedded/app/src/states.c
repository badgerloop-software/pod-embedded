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
#include <stdio.h>
#include <stdbool.h>
#include <time.h>  

#include "state_machine.h"
#include "data.h"
#include "states.h"

/* Imports/Externs */

extern stateMachine_t stateMachine;
extern data_t *data;
static bool checkPrimPressures(void);
static bool checkStopped(void);

extern stateTransition_t *findTransition(state_t *currState, char *name);

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
        fprintf(stderr, "Line pressure failing\n");
        noProblem = false;
    }
    if (data->pressure->ps3 < PS3_BOTTOM_LIMIT || data->pressure->ps3 > PS3_TOP_LIMIT) {
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
    // TODO Check this with all three retro timers?
	return data->motion->accel < MAX_STOPPED_ACCEL &&  (time(NULL) - data->timers->lastRetro1) > 15;
}


static bool checkBattTemp(void) {
    return data->bms->highTemp < MAX_BATT_TEMP; 
}
/***
 * Actions for all the states.
 * They perform transition and error condition
 * checking and perform the functionality of the state: e.g. brake if in
 * braking.
 *
 */

stateTransition_t * powerOnAction() {
    printf("Power on action\n");
    return findTransition(stateMachine.currState, IDLE_NAME);
}

stateTransition_t * idleAction() {
    // First check for nominal values?
    data->state = 1;
   // if(!checkPrimPressures() || !checkStopped()){
   //     STATE_ERROR();
   // }

    if(data->flags->readyPump){
        return findTransition(stateMachine.currState, READY_FOR_PUMPDOWN_NAME);
    }

    return NULL;
}

stateTransition_t * readyForPumpdownAction() {
    // First check for nominal values?
    data->state = 2;
    if (!checkPrimPressures() || !checkBattTemp()) {
	return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    if (data->flags->pumpDown){
        return findTransition(stateMachine.currState, PUMPDOWN_NAME);
    }

    return NULL;
}

stateTransition_t * pumpdownAction() {
    // First check for nominal values?
    data->state = 3;
    if(data->flags->readyCommand){
	return findTransition(stateMachine.currState, READY_NAME);
    }

    return NULL;
}

stateTransition_t * readyForLaunchAction() {
    // First check for nominal values?
    data->state = 4;
    if(data->flags->propulse){
        return findTransition(stateMachine.currState, PROPULSION_NAME);
    }

    return NULL;
}

stateTransition_t * propulsionAction() {
    if(data->flags->emergencyBrake){
	// If it's an emergency, shold we trigger braking first then go through the formalities of doing a state transition?
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }
    data->state = 5;
    // Check for nominal values?
    

    if (time(NULL) - data->timers->startTime > MAX_RUN_TIME){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    return NULL;
}

stateTransition_t * brakingAction() {
    data->state = 6;
    /* transition to post run */
    if (!checkBattTemp()) 
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    if (checkStopped()) {
        return findTransition(stateMachine.currState, POST_RUN_NAME);
    }
    return NULL;
}

stateTransition_t * stoppedAction() {
    data->state = 7;
    if (!checkBattTemp()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    if (data->bms->cellMaxVoltage > MAX_CELL_VOLTAGE || data->bms->cellMinVoltage < MIN_CELL_VOLTAGE) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
	return NULL;
}

stateTransition_t * crawlAction() {
    data->state = 8;
    if (!checkBattTemp()){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    if (data->bms->cellMaxVoltage > MAX_CELL_VOLTAGE || data->bms->cellMinVoltage < MIN_CELL_VOLTAGE) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    if (data->pressure->pv < 13) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    if (data->motion->pos >= (TUBE_LENGTH - 100)) {
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }
    
	return NULL;
}

stateTransition_t * postRunAction() {
    data->state = 9;
	if (!checkBattTemp()) {
        return findTransition(stateMachine.currState, POST_RUN_FAULT_NAME);
    }
    
    if (data->timers->timeInState >= 30 && data->bms->packVoltage > 0) {
        return findTransition(stateMachine.currState, POST_RUN_FAULT_NAME);
    }

    if (data->bms->packVoltage == 0 && data->pressure->ps1 < 30) {
        return findTransition(stateMachine.currState, SAFE_TO_APPROACH_NAME);
    }

    return NULL;
}

stateTransition_t * safeToApproachAction() {
    data->state = 10;
	return NULL;
}

stateTransition_t * secondaryBrakingAction() {
    data->state = 14;
	return NULL;
}

stateTransition_t * preFaultAction() {
    data->state = 11;
	return NULL;
}

stateTransition_t * runFaultAction() {
    data->state = 12;
	return NULL;
}

stateTransition_t * postFaultAction() {
    data->state = 13;
	return NULL;
}
