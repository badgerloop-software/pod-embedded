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
    if (data->pressure->ps1 < PS1_BOTTOM_LIMIT || data->pressure->ps1 > PS1_TOP_LIMIT) {
        fprintf(stderr, "PS1 pressure failing\n");
        return false;
    }
    if (data->pressure->ps2 < PS2_BOTTOM_LIMIT || data->pressure->ps2 > PS2_TOP_LIMIT) {
        fprintf(stderr, "PS2 pressure failing\n");
        return false;
    }
    if (data->pressure->ps3 < PS3_BOTTOM_LIMIT || data->pressure->ps3 > PS3_TOP_LIMIT) {
        fprintf(stderr, "PS3 pressure failing\n");
        return false;
    }
    if (data->pressure->ps4 < PS4_BOTTOM_LIMIT || data->pressure->ps4 > PS4_TOP_LIMIT) {
        fprintf(stderr, "PS4 pressure failing\n");
        return false;
    }

    return true;
}

static bool checkPrerunBattery(void){
    if(data->bms->highTemp > MAX_BATT_TEMP_PRERUN){
        printf("Temp too high: %i\n", data->bms->highTemp);
        return false;
    }
    if(data->bms->packCurrent > MAX_BATT_CURRENT_STILL){
        printf("Pack Current too high: %f\n", data->bms->packCurrent);
        return false;
    }
    if(data->bms->cellMaxVoltage > MAX_CELL_VOLTAGE || data->bms->cellMinVoltage < MIN_CELL_VOLTAGE){
        printf("Cell Voltage Error: %f, %f\n", data->bms->cellMinVoltage, data->bms->cellMaxVoltage);
        return false;
    }
    if(data->bms->packVoltage > MAX_PACK_VOLTAGE || data->bms->packVoltage < MIN_PACK_VOLTAGE_PRERUN){
        printf("Pack Voltage Error: %f\n", data->packVoltage);
        return false;
    }
    if(data->bms->Soc < MIN_SOC_PRERUN){
        printf("SOC is less than expected: %i", data->bms->Soc);
        return false;
    }
    
    return true;
}

static bool checkPrerunRMS(){
    if(data->rms->igbtTemp < MIN_IGBT_TEMP || data->rms->igbtTemp > MAX_IGBT_TEMP_PRERUN){
        printf("IGBT Prerun Temp Failure: %i\n", data->rms->igbtTemp);
        return false;
    }
    if(data->rms->controlBoardTemp < MIN_CONTROL_TEMP || data->rms->controlBoardTemp > MAX_CONTROL_TEMP_IDLE){
        printf("Control Temp Failure: %i\n", data->rms->controlBoardTemp);
        return false;
    }
    if(data->rms->motorTemp < MIN_MOTOR_TEMP || data->rms->motorTemp > MAX_MOTOR_TEMP_PRERUN){
        printf("Motor Temp Failure: %i\n", data->rms->motorTemp);
        return false;
    }
    if(data->rms->phaseACurrent < PHASE_A_MIN || data->rms->phaseACurrent > PHASE_A_MAX_PRE){
        printf("Phase A Current Failure: %i\n", data->rms->phaseACurrent);
        return false;
    }
    if(data->rms->dcBusVoltage < DC_BUS_VOLTAGE_MIN || data->rms->dcBusVoltage > DC_BUS_VOLTAGE_MAX){
        printf("DC Bus Voltage Failure: %i\n", data->rms->dcBusVoltage);
        return false;
    }
    if(data->rms->lvVoltage < LV_VOLTAGE_MIN || data->rms->lvVoltage > LV_VOLTAGE_MAX){
        printf("LV Voltage Failure: %i\n", data->rms->lvVoltage);
        return false;
    }
    if(data->rms->commandedTorque < CMD_TORQUE_MIN || data->rms->commandedTorque > CMD_TORQUE_MAX_PRE){
        printf("CMD Torque Failure: %i\n", data->rms->commandedTorque);
        return false;
    }
    if(data->rms->actualTorque < ACTUAL_TORQUE_MIN || data->rms->actualTorque > ACTUAL_TORQUE_MAX_PRE){
        printf("Actual Torque Failure: %i\n", data->rms->actualTorque);
        return false;
    }
    
    // IDLE
    if(data->state == 1 || data->state == 2){
        if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_IDLE){
            printf("DC Bus Current Idle Failure: %i\n", data->rms->dcBusCurrent);
            return false;
        }
        if(data->rms->motorSpeed < MIN_RPM_IDLE || data->rms->motorSpeed > MAX_RPM_IDLE){
            printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
            return false;
        }
        if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_PRERUN){
            printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
            return false;
        }
    }
    
    // READY PUMPDOWN SPECIFIC
    else if(data->state == 3){
        if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_PUMPDOWN){
            printf("DC Bus Current Pumpdown Failure: %i\n", data->rms->dcBusCurrent);
            return false;
        }
        if(data->rms->motorSpeed < MIN_RPM_IDLE || data->rms->motorSpeed > MAX_RPM_IDLE){
            printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
            return false;
        }
        if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_PRERUN){
            printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
            return false;
        }
    }
    // PUMPDOWN SPECIFIC
    else if(data->state == 4){
        if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_PUMPDOWN){
            printf("DC Bus Current Pumpdown Failure: %i\n", data->rms->dcBusCurrent);
            return false;
        }
        if(data->rms->motorSpeed < MIN_RPM_PUMPDOWN || data->rms->motorSpeed > MAX_RPM_PUMPDOWN){
            printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
            return false;
        }
        if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_PRERUN){
            printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
            return false;
        }
    }
    // PRE-PROPULSE SPECIFIC
    else if(data->state == 5){
        if(data->rms->dcBusCurrent < DC_BUS_CURRENT_MIN || data->rms->dcBusCurrent > DC_BUS_CURRENT_MAX_PUMPDOWN){
            printf("DC Bus Current Pumpdown Failure: %i\n", data->rms->dcBusCurrent);
            return false;
        }
        if(data->rms->motorSpeed < MIN_RPM_PUMPDOWN || data->rms->motorSpeed > MAX_RPM_PUMPDOWN){
            printf("Motor speed Failure: %i\n", data->rms->motorSpeed);
            return false;
        }
        if(data->rms->gateDriverBoardTemp < MIN_GATE_TEMP || data->rms->gateDriverBoardTemp > MAX_GATE_TEMP_RUN){
            printf("Gate Driver Temp Failure: %i\n", data->rms->gateDriverBoardTemp);
            return false;
        }
    }
    
    return true;
    
}

/***
 * checkStopped - checks a variety of values to make sure the pod is stopped.
 *
 * RETURNS: true if stopped, false if moving
 */

static bool checkStopped(void) {
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
    data->state = 1;
    
    // CHECK PRESSURE
    if(!checkPrimPressures(){
        STATE_ERROR();
    }
    
    // CHECK STOPPED (MOTION)
    if(!checkStopped()){
        STATE_ERROR();
    }
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkPrerunBattery()){
        STATE_ERROR();
    }
    
    if(!checkPrerunRMS()){
        STATE_ERROR();
    }
    
    // TRANSITION CRITERIA
    if(data->flags->readyPump){
        return findTransition(stateMachine.currState, READY_FOR_PUMPDOWN_NAME);
    }
    

    return NULL;
}

stateTransition_t * readyForPumpdownAction() {
    data->state = 2;

    // CHECK PRESSURE
    if(!checkPrimPressures(){
        STATE_ERROR();
    }
    
    // CHECK STOPPED (MOTION)
    if(!checkStopped()){
        STATE_ERROR();
    }
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkPrerunBattery()){
        STATE_ERROR();
    }
    
    if(!checkPrerunRMS()){
        STATE_ERROR();
    }

    if (data->flags->pumpDown){
        return findTransition(stateMachine.currState, PUMPDOWN_NAME);
    }

    return NULL;
}

stateTransition_t * pumpdownAction() {
    // First check for nominal values?
    data->state = 3;
    
    // CHECK PRESSURE
    if(!checkPrimPressures(){
        STATE_ERROR();
    }
    
    // CHECK STOPPED (MOTION)
    if(!checkStopped()){
        STATE_ERROR();
    }
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkPrerunBattery()){
        STATE_ERROR();
    }
    
    if(!checkPrerunRMS()){
        STATE_ERROR();
    }
    
    
    if(data->flags->readyCommand){
	return findTransition(stateMachine.currState, READY_NAME);
    }

    return NULL;
}

stateTransition_t * readyForLaunchAction() {
    data->state = 4;
    
     // CHECK PRESSURE
    if(!checkPrimPressures(){
        STATE_ERROR();
    }
    
    // CHECK STOPPED (MOTION)
    if(!checkStopped()){
        STATE_ERROR();
    }
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkPrerunBattery()){
        STATE_ERROR();
    }
    
    if(!checkPrerunRMS()){
        STATE_ERROR();
    }

    if(data->flags->propulse){
        return findTransition(stateMachine.currState, PROPULSION_NAME);
    }

    return NULL;
}

stateTransition_t * propulsionAction() {
    if(data->flags->emergencyBrake){
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
