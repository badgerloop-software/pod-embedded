/***
 * filename: state_machine.c
 *
 * summary: This is the collection of a lot of the pods data and
 * where it will check for faults. The pod will always be running this code and
 * the state machine that lives here is inherently global as it applies to the
 * whole pod. 
 * 
 * RE: Ezra Boley, ...
 *
 ***/

#include "badgerloop.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STATE_ERROR() ({                \
          printf("ERROR exiting...\n"); \
          exit(-1);                     \
        })

static void initState(state_t* state, char* name, stateTransition_t *(*action)(), int numTrans );
static void initTransition(stateTransition_t *transition, state_t *target, void (*action)() ); 

extern data_t pressures;



stateMachine_t stateMachine;

inline state_t *findState(char *stateName) {
    for (int i = 0; i < NUM_STATES; i++) {
        if (strcmp(stateMachine.allStates[i]->name, stateName) == 0) {
            return stateMachine.allStates[i];
        }
    }
    return NULL;
}

inline stateTransition_t *findTransition(state_t *srcState, char *targName) {
    for (int i = 0; i < srcState->numTransitions; i++) {
        if (strcmp(srcState->transitions[i]->target->name, targName) == 0)
            return srcState->transitions[i];
    }
    return NULL;
}

void runStateMachine() {
    /* The cmd receiver will populate this field if we get an override */
    if (stateMachine.overrideStateName != NULL) {
        state_t *tempState = findState(stateMachine.overrideStateName);
        /* TODO We also need to execute a transition if it exists here */
        if (tempState != NULL)
            stateMachine.currState = tempState;
        stateMachine.overrideStateName = NULL;
    }

    /* execute the state and check if we should be transitioning */
	stateTransition_t *transition = stateMachine.currState->action();
    if (transition != NULL) {
		transition->action();
		stateMachine.currState = transition->target;
	}
}


void buildStateMachine() {
	    /* Create all of the states*/
	state_t *powerOn, *idle, *readyForPumpdown, *pumpdown,
			*readyForLaunch, *propulsion, *braking, *secondaryBraking,
			*stopped, *crawl, *rebrake, *postRun, *safeToApproach, *preFault,
			*runFault, *postFault;
	state_t **allStates = malloc(NUM_STATES * sizeof(*state_t));

	stateMachine.allStates = allStates;

    /* Allocating space for each state */
	if (powerOn = malloc(sizeof(state_t))           == NULL) { STATE_ERROR() }
	if (idle = malloc(sizeof(state_t))              == NULL) { STATE_ERROR() }
	if (readyForPumpdown = malloc(sizeof(state_t))  == NULL) { STATE_ERROR() }
	if (pumpdown = malloc(sizeof(state_t))          == NULL) { STATE_ERROR() }
	if (readyForLaunch = malloc(sizeof(state_t))    == NULL) { STATE_ERROR() }
	if (propulsion = malloc(sizeof(state_t))        == NULL) { STATE_ERROR() }
	if (braking = malloc(sizeof(state_t))           == NULL) { STATE_ERROR() }
	if (secondaryBraking = malloc(sizeof(state_t))  == NULL) { STATE_ERROR() }
	if (stopped = malloc(sizeof(state_t))           == NULL) { STATE_ERROR() }
	if (crawl = malloc(sizeof(state_t))             == NULL) { STATE_ERROR() }
	if (rebrake = malloc(sizeof(state_t))           == NULL) { STATE_ERROR() }
	if (postRun = malloc(sizeof(state_t))           == NULL) { STATE_ERROR() }
	if (safeToApproach = malloc(sizeof(state_t))    == NULL) { STATE_ERROR() }
	if (preFault = malloc(sizeof(state_t))          == NULL) { STATE_ERROR() }
	if (runFault = malloc(sizeof(state_t))          == NULL) { STATE_ERROR() }
	if (postFault = malloc(sizeof(state_t))         == NULL) { STATE_ERROR() }

    /* Initializing states TODO add the num transition arguement to each of
     * these*/
	initState(powerOn, POWER_OFF_NAME, stateTransition_t *(*powerOnAction)());   // FIXME Not sure if this state should be poweroff or poweron
	initState(idle, IDLE_NAME, stateTransition_t *(*idleAction)());
	initState(readyForPumpdown, READY_FOR_PUMPDOWN_NAME, stateTransition_t *(*readyForPumpdownAction)());
	initState(pumpdown, PUMPDOWN_NAME, stateTransition_t *(*pumpdownAction)());
	initState(readyForLaunch, READY_NAME, stateTransition_t *(*readyForPumpdownAction)());
	initState(propulsion, PROPULSION_NAME, stateTransition_t *(*propulsionAction)());
	initState(braking, BRAKING_NAME, stateTransition_t *(*brakingAction)());
	initState(stopped, STOPPED_NAME, stateTransition_t *(*stoppedAction)());
	initState(crawl, CRAWL_NAME, stateTransition_t *(*crawlAction)());
	initState(postRun, POST_RUN_NAME, stateTransition_t *(*postRunAction)());
	initState(safeToApproach, SAFE_TO_APPROACH_NAME, stateTransition_t *(*safeToApproachAction)());
	initState(preFault, PRE_RUN_FAULT_NAME, stateTransition_t *(*preFaultAction)());
	initState(runFault, RUN_FAULT_NAME, stateTransition_t *(*runFaultAction)());
	initState(postFault, POST_RUN_FAULT_NAME, stateTransition_t *(*postFaultAction)());
    
    /* Create all of the transitions */
        
}


static void initState(state_t* state, char* name, stateTransition_t *(*action)(), int numTransitions ) {
    static int indexInAllStates = 0;
    strncpy(state->name, name, strlen(name) );
    state->action = action; 
    stateMachine.allStates[indexInAllStates++] = state;
}


static void initTransition(stateTransition_t *transition, state_t *target, bool (*action)() ) {

}

