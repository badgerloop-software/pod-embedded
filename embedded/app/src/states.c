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
#include <math.h>
#include "bms_fault_checking.h"
#include "rms_fault_checking.h"
#include "pressure_fault_checking.h"
#define NO_FAULT
#define LV_BATT_SOC_CALC(x) (pow(-1.1142 * (x), 6) + \
        pow(78.334 * (x), 5) - \
        pow(2280.5 * (x), 4) + \
        pow(35181.0 * (x), 3) - \
        pow(303240.0 *(x),2) - (1000000.0 * (x)))

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

stateTransition_t * idleAction() {
    static firstCycle = true;
    static displayBatt = 0;
    if (firstCycle) {
        data->flags->brakeInit = true;
/*  Does this!      */
/*        brakePrimaryUnactuate();*/
/*        brakeSecondaryUnactuate();*/
        firstCycle = false;
    }

    if (displayBatt >= 100) {
        printf("LV SOC: %f\n", LV_BATT_SOC_CALC(getLVBattVoltage()));
        displayBatt = 0;
    } else 
        displayBatt += 1;
    data->state = 1;
    
    if (data->flags->emergencyBrake) {
        return findTransition(stateMachine.currState ,RUN_FAULT_NAME);
    }

    // CHECK PRESSURE
#ifndef NO_FAULT
    if(!checkPrerunPressures()){
        return stateMachine.currState->fault;
    }
#endif
    
    // TODO check LV Power
    

    // TODO check LV Temp
#ifndef NO_FAULT
    if(!checkPrerunBattery()){
        return stateMachine.currState->fault;
    }
    
    if(!checkPrerunRMS()){
        return stateMachine.currState->fault;
    }
#endif
    // TRANSITION CRITERIA
    if(data->flags->readyPump){
        return findTransition(stateMachine.currState, PUMPDOWN_NAME);
    }
    

    return NULL;
}

stateTransition_t * pumpdownAction() {
    // First check for nominal values?
    data->state = 2;
     /* Check IMD status */
    if (data->flags->emergencyBrake) {
        return findTransition(stateMachine.currState ,RUN_FAULT_NAME);
    } 
#ifndef NO_FAULT
    if (!getIMDStatus()) {
        return stateMachine.currState->fault;
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return stateMachine.currState->fault;
    }

    // CHECK PRESSURE
    if(!checkPrerunPressures()){
        return stateMachine.currState->fault;
    }
#endif    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkPrerunBattery()){
        return stateMachine.currState->fault;
    }
    
    if(!checkPrerunRMS()){
        return stateMachine.currState->fault;
    }
    
    
    if(data->flags->readyCommand){
	return findTransition(stateMachine.currState, PROPULSION_NAME);
    }

    return NULL;
}

stateTransition_t * propulsionAction() {
    static bool isInit = false;
    
    data->state = 3;
    if (!isInit) {
        data->flags->clrMotionData = true;
        data->timers->startTime = getuSTimestamp();
        isInit = true;
    }
    /* Check IMD status */
    if (data->flags->emergencyBrake) {
        printf("EMERG\n");
        return findTransition(stateMachine.currState ,RUN_FAULT_NAME);
    } 
/*    if (!getIMDStatus()) {*/
/*        return stateMachine.currState->fault;*/
/*    }*/

    /* Check HV Indicator light */
/*    if (!isHVIndicatorEnabled()) {*/
/*        return stateMachine.currState->fault;*/
/*    }*/


    
    // CHECK FAULT CRITERIA
    // CHECK PRESSURE -- PreRun function still valid here
/*    if(!checkRunPressures()){*/
/*        return stateMachine.currState->fault;*/
/*    }*/
    
    // TODO check LV Power
    // TODO check LV Temp
    
/*    if(!checkRunBattery()){*/
/*        return stateMachine.currState->fault;*/
/*    }*/
    
/*    if(!checkRunRMS()){*/
/*        return stateMachine.currState->fault;*/
/*    }*/
    if (getuSTimestamp() - data->timers->startTime > MAX_RUN_TIME){
        printf("here54\n");
        
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }

    if (data->motion->retroCount >= 3 && data->flags->readyToBrake) {
        printf("here3\n");
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }

    // CHECK TRANSITION CRITERIA
    if(data->flags->shouldStop){
        printf("HERE1\n");
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }
    // TODO will the IMU/timer logic be handled in the retro driver or do we do it here?

    return NULL;
}

stateTransition_t * brakingAction() {
    data->state = 4;
    // TODO Do we differenciate between primary and secondary braking systems?
    // TODO Add logic to handle switches / actuate both
 /* Check IMD status */
    if (data->flags->emergencyBrake) {
        return findTransition(stateMachine.currState ,RUN_FAULT_NAME);
    } 
#ifndef NO_FAULT
    if (!getIMDStatus()) {
        return stateMachine.currState->fault;
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return stateMachine.currState->fault;
    }


    // CHECK FAULT CRITERIA
/*    if(!checkBrakingPressures()){*/
/*        return stateMachine.currState->fault;*/
/*    }*/

    // TODO check LV Power
    // TODO check LV Temp

    if(!checkBrakingBattery()){
        return stateMachine.currState->fault;
    }

    if(!checkBrakingRMS()){
        return stateMachine.currState->fault;
    }
#endif
    if ((getuSTimestamp() - data->timers->startTime) > MAX_RUN_TIME){
        return stateMachine.currState->fault;
    }

    // CHECK TRANSITION CRITERIA
/*    if(checkStopped()){
        return findTransition(stateMachine.currState, STOPPED_NAME);
    }*/


    return NULL;
}

stateTransition_t * stoppedAction() {
	data->state = 5;
     /* Check IMD status */
    if (data->flags->emergencyBrake) {
        return findTransition(stateMachine.currState ,RUN_FAULT_NAME);
    } 
    if (!getIMDStatus()) {
        return stateMachine.currState->fault;
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return stateMachine.currState->fault;
    }

   
    // CHECK FAULT CRITERIA
    
/*    if(!checkBrakingPressures()){ // Still unchanged*/
/*        return stateMachine.currState->fault;*/
/*    }*/
    
    // TODO check LV Power
    // TODO check LV Temp
    
    if(!checkBrakingBattery()){ // Still unchanged
        return stateMachine.currState->fault;
    }
    
    if(!checkStoppedRMS()){ // Still unchanged
        return stateMachine.currState->fault;
    }
    
	// FIXME FIX ALL TIME CHECKS!
    if (getuSTimestamp() - data->timers->startTime > MAX_RUN_TIME){
        return stateMachine.currState->fault;
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

stateTransition_t * servPrechargeAction() {
    data->state = 6;
    
    if (data->flags->emergencyBrake) {
        return findTransition(stateMachine.currState ,RUN_FAULT_NAME);
    } 

    return NULL;
}

stateTransition_t * crawlAction() {
    data->state = 7;
 /* Check IMD status */
    if (!getIMDStatus()) {
        return stateMachine.currState->fault;
    }

    if (data->flags->emergencyBrake) {
        return findTransition(stateMachine.currState ,RUN_FAULT_NAME);
    } 
    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return stateMachine.currState->fault;
    }


/*    if(!checkCrawlPostrunPressures()){*/
/*        return stateMachine.currState->fault;*/
/*    }*/

    // TODO check LV Power
    // TODO check LV Temp

    if(!checkCrawlBattery()){
        return stateMachine.currState->fault;
    }

    if(!checkCrawlRMS()){ // Still unchanged
        return stateMachine.currState->fault;
    }

    // CHECK TRANSITION CRITERIA
    
    if (getuSTimestamp() - data->timers->crawlTimer > MAX_CRAWL_TIME){
        return findTransition(stateMachine.currState, BRAKING_NAME);
    }
    
    if(data->flags->shouldStop){

        return findTransition(stateMachine.currState, BRAKING_NAME);
    }
	return NULL;
}

stateTransition_t * postRunAction() {
    data->state = 8;
 /* Check IMD status */
    if (data->flags->emergencyBrake) {
        return findTransition(stateMachine.currState ,RUN_FAULT_NAME);
    } 
    if (!getIMDStatus()) {
        return stateMachine.currState->fault;
    }

    /* Check HV Indicator light */
    if (!isHVIndicatorEnabled()) {
        return stateMachine.currState->fault;
    }


/*    if(!checkCrawlPostrunPressures()){*/
/*        return stateMachine.currState->fault;*/
/*    }*/

    // TODO check LV Power
    // TODO check LV Temp

    if(!checkPostrunBattery()){
        return stateMachine.currState->fault;
    }

    if(!checkPostRMS()){
        return stateMachine.currState->fault;
    }

    // TODO CHECK TRANSITION CRITERIA
/*    if(data->flags->safeToApproach) {*/
/**/
/*    }*/

    return NULL;
}

stateTransition_t * safeToApproachAction() {
    if (isHVEnabled()) {
        return stateMachine.currState->fault;
    } 
/*    if (!checkIdlePressures()) {*/
/*       fprintf(stderr, "Pressures are out of the safe range\n");*/
/*       return stateMachine.currState->fault;*/
/*    }*/
    
    if (data->flags->emergencyBrake) {
        return findTransition(stateMachine.currState ,RUN_FAULT_NAME);
    } 
    data->state = 9;
	return NULL;
}
// 
//  We're removing pre and post faults and making them non run faults. 
// When you change this make 11 the non run fault action. Ty - EU
stateTransition_t * nonRunFaultAction() {
    //TODO
    data->state = 10;
	return NULL;
}

stateTransition_t * runFaultAction() {
    data->state = 11;
	
    return NULL;
}

stateTransition_t * brakingFault() {
    //TODO
    data->state = 12;
    return NULL;
}
