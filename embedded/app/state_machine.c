#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STATE_ERROR() ({\
          printf("ERROR exiting...\n");\
          exit(-1);\
            })

static void initState(state_t* state, char* name, stateTransition_t *(*action)(), int numTrans );
static void initTransition(stateTransition_t *transition, state_t *target, void (*action)() ); 

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
	initState(powerOn, "powerOn", stateTransition_t *(*powerOnAction)());
	initState(idle, "idle", stateTransition_t *(*idleAction)());
	initState(readyForPumpdown, "readyForPumpdown", stateTransition_t *(*readyForPumpdownAction)());
	initState(pumpdown, "pumpdown", stateTransition_t *(*pumpdownAction)());
	initState(readyForLaunch, "readyForLaunch", stateTransition_t *(*readyForPumpdownAction)());
	initState(propulsion, "propulsion", stateTransition_t *(*propulsionAction)());
	initState(braking, "braking", stateTransition_t *(*brakingAction)());
	initState(secondaryBraking, "secondaryBraking", stateTransition_t *(*secondaryBrakingAction)());
	initState(stopped, "stopped", stateTransition_t *(*stoppedAction)());
	initState(crawl, "crawl", stateTransition_t *(*crawlAction)());
	initState(rebrake, "rebrake", stateTransition_t *(*rebrakeAction)());
	initState(postRun, "postRun", stateTransition_t *(*postRunAction)());
	initState(safeToApproach, "safeToApproach", stateTransition_t *(*safeToApproachAction)());
	initState(preFault, "preFault", stateTransition_t *(*preFaultAction)());
	initState(runFault, "runFault", stateTransition_t *(*runFaultAction)());
	initState(postFault, "postFault", stateTransition_t *(*postFaultAction)());
    
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


// Thanks -Ethan <3
stateTransition_t * powerOnAction() {
            	
    return NULL;
}

stateTransition_t * idleAction() {
	
    return NULL;
}

stateTransition_t * readyForPumpdownAction() {
	
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

