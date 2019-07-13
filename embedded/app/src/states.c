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
#include <math.h>

#include "state_machine.h"
#include "data.h"
#include "states.h"
#include "hv_iox.h"

#include "bms_fault_checking.h"
#include "rms_fault_checking.h"
#include "pressure_fault_checking.h"

/* Imports/Externs */

extern stateMachine_t stateMachine;

extern stateTransition_t *findTransition(state_t *currState, char *name);


/***
 * checkStopped - checks a variety of values to make sure the pod is stopped.
 *
 * RETURNS: true if stopped, false if moving
 */

static bool checkStopped(void) {
	return fabs(data->motion->vel) < MAX_STOPPED_VEL &&  (getuSTimestamp() - data->timers->lastRetro) > TIME_SINCE_LAST_RETRO;
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
    if(!checkPrerunPressures()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    // CHECK STOPPED (MOTION)
    if(!checkStopped()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkPrerunBattery()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    if(!checkPrerunRMS()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    // TRANSITION CRITERIA
    if(data->flags->readyPump){
        return findTransition(stateMachine.currState, READY_FOR_PUMPDOWN_NAME);
    }
    

    return NULL;
}

stateTransition_t * readyForPumpdownAction() {
    data->state = 2;

    /* Check IMD status */
    if (!getIMDStatus()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    // CHECK PRESSURE
    if(!checkPrerunPressures()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    // CHECK STOPPED (MOTION)
    if(!checkStopped()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkPrerunBattery()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    if(!checkPrerunRMS()){
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
     /* Check IMD status */
    if (!getIMDStatus()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }


    // CHECK PRESSURE
    if(!checkPrerunPressures()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    // CHECK STOPPED (MOTION)
    if(!checkStopped()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkPrerunBattery()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    if(!checkPrerunRMS()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    
    if(data->flags->readyCommand){
	return findTransition(stateMachine.currState, READY_NAME);
    }

    return NULL;
}

stateTransition_t * readyForLaunchAction() {
    data->state = 4;
     /* Check IMD status */
    if (!getIMDStatus()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }


     // CHECK PRESSURE
    if(!checkPrerunPressures())
    {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    // CHECK STOPPED (MOTION)
    if(!checkStopped()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }
    
    // TODO check LV Power
    // TODO check LV Temp

	if(!checkPrerunBattery()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    if(!checkPrerunRMS()){
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    if(data->flags->propulse){
        // Init initial timer
		data->timers->startTime = getuSTimestamp();
        return findTransition(stateMachine.currState, PROPULSION_NAME);
    }

    return NULL;
}

stateTransition_t * propulsionAction() {
    data->state = 5;
     /* Check IMD status */
    if (!getIMDStatus()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }


    // CHECK FOR EMERGENCY BRAKE
    if(data->flags->emergencyBrake){
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }
    
    // CHECK FAULT CRITERIA
    // CHECK PRESSURE -- PreRun function still valid here
    if(!checkPrerunPressures()){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkRunBattery()){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    
    if(!checkRunRMS()){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    if (getuSTimestamp() - data->timers->startTime > MAX_RUN_TIME){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    // CHECK TRANSITION CRITERIA
    if(data->flags->shouldStop){
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }
    // TODO will the IMU/timer logic be handled in the retro driver or do we do it here?

    return NULL;
}

stateTransition_t * brakingAction() {
    data->state = 6;
    // TODO Do we differenciate between primary and secondary braking systems?
    // TODO Add logic to handle switches / actuate both
 /* Check IMD status */
    if (!getIMDStatus()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }


    // CHECK FAULT CRITERIA
    if(!checkBrakingPressures()){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    // TODO check LV Power
    // TODO check LV Temp

    if(!checkBrakingBattery()){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    if(!checkBrakingRMS()){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    if ((getuSTimestamp() - data->timers->startTime) > MAX_RUN_TIME){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    // CHECK TRANSITION CRITERIA
    if(checkStopped()){
        return findTransition(stateMachine.currState, STOPPED_NAME);
    }


    return NULL;
}

stateTransition_t * stoppedAction() {
	data->state = 7;
     /* Check IMD status */
    if (!getIMDStatus()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

   
    // CHECK FAULT CRITERIA
    
    if(!checkBrakingPressures()){ // Still unchanged
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkBrakingBattery()){ // Still unchanged
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    
    if(!checkStoppedRMS()){ // Still unchanged
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    
	// FIXME FIX ALL TIME CHECKS!
    if (getuSTimestamp() - data->timers->startTime > MAX_RUN_TIME){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }
    
    // CHECK TRANSITION CRITERIA
    if(data->motion->pos <= TUBE_LENGTH - MIN_DISTANCE_TO_END){
        return findTransition(stateMachine.currState, POST_RUN_NAME);
    }
    else{
        return findTransition(stateMachine.currState, CRAWL_NAME);
    }
	return NULL;
}

stateTransition_t * crawlAction() {
    data->state = 8;
 /* Check IMD status */
    if (!getIMDStatus()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }


    if(!checkCrawlPostrunPressures()){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    // TODO check LV Power
    // TODO check LV Temp

    if(!checkCrawlBattery()){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    if(!checkCrawlRMS()){ // Still unchanged
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    if (getuSTimestamp() - data->timers->startTime > MAX_RUN_TIME){
        return findTransition(stateMachine.currState, RUN_FAULT_NAME);
    }

    // CHECK TRANSITION CRITERIA
    if(data->flags->shouldStop){
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }
	return NULL;
}

stateTransition_t * postRunAction() {
    data->state = 9;
 /* Check IMD status */
    if (!getIMDStatus()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return findTransition(stateMachine.currState, PRE_RUN_FAULT_NAME);
    }


    if(!checkCrawlPostrunPressures()){
        return findTransition(stateMachine.currState, POST_RUN_FAULT_NAME);
    }

    // TODO check LV Power
    // TODO check LV Temp

    if(!checkPostrunBattery()){
        return findTransition(stateMachine.currState, POST_RUN_FAULT_NAME);
    }

    if(!checkPostRMS()){
        return findTransition(stateMachine.currState, POST_RUN_FAULT_NAME);
    }

    // TODO CHECK TRANSITION CRITERIA
    

    return NULL;
}

stateTransition_t * safeToApproachAction() {
    if (isHVEnabled()) {
        return findTransition(stateMachine.currState, POST_RUN_FAULT_NAME);
    }
    data->state = 10;
	return NULL;
}

stateTransition_t * preFaultAction() {
    //TODO
    data->state = 11;
	return NULL;
}

stateTransition_t * runFaultAction() {
    //TODO
    data->state = 12;
	return NULL;
}

stateTransition_t * postFaultAction() {
    //TODO
    data->state = 13;
	return NULL;
}
