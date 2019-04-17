/***
 * filename: state_machine.c
 *
 * summary: This is the collection of a lot of the pods data and
 * where it will check for faults. The pod will always be running this code and
 * the state machine that lives here is inherently global as it applies to the
 * whole pod.
 *
 ***/

#include "badgerloop.h"
#include "data.h"
#include "state_machine.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static void initState(state_t* state, char* name, stateTransition_t *(*action)(), int numTrans);
static void initTransition(stateTransition_t *transition, state_t *target, bool (*action)() );


extern data_t *data;

extern stateTransition_t * powerOnAction(void);
extern stateTransition_t * idleAction(void);
extern stateTransition_t * readyForPumpdownAction(void);
extern stateTransition_t * pumpdownAction(void);
extern stateTransition_t * readyForLaunchAction(void);
extern stateTransition_t * propulsionAction(void);
extern stateTransition_t * brakingAction(void);
extern stateTransition_t * stoppedAction(void);
extern stateTransition_t * crawlAction(void);
extern stateTransition_t * postRunAction(void);
extern stateTransition_t * safeToApproachAction(void);
extern stateTransition_t * secondaryBrakingAction(void);
extern stateTransition_t * preFaultAction(void);
extern stateTransition_t * runFaultAction(void);
extern stateTransition_t * postFaultAction(void);



volatile stateMachine_t stateMachine;

/***
 * findState - Searches all the created states and returns the one with a matching name
 *
 * ARGS: char *stateName - Name of the state we are searching for. Check out state_machine.h
 * 	for options.
 *
 * RETURNS: state_t, the found state or NULL if that state doesnt exist
 */
state_t *findState(char *stateName) {
    for (int i = 0; i < NUM_STATES; i++) {
        if (stateMachine.allStates[i] == NULL) continue;
        if (strcmp(stateMachine.allStates[i]->name, stateName) == 0) {
            return stateMachine.allStates[i];
        }
    }
    return NULL;
}

/***
 * initState - fills in the fields of the state_t struct.
 *
 * ARGS: state_t *state - allocated state (malloced most likely),
 *       stateTransition_t *(*action)() - a function pointer to a function that returns a state
 *			transition pointer. This is the meat of the state
 * 		 numTransitions - not implemented, fairly self explanatory
 */
static void initState(state_t* state, char* name, stateTransition_t *(*action)(), int numTransitions ) {
    static int indexInAllStates = 0;
    int i = 0;
    //state = malloc(sizeof(state_t));
    
    if (state == NULL) {
        STATE_ERROR();
    }
    
    state->name = malloc(1 + (strlen(name) * sizeof(char)));
    strcpy(state->name, name);
    state->action = action;
    state->transitionCounter = 0;
    state->numTransitions = numTransitions;
    state->transitions = malloc(numTransitions * (sizeof(stateTransition_t *)));
    for (i = 0; i < numTransitions; i++) {
        state->transitions[i] = malloc(sizeof(stateTransition_t));
    }
    stateMachine.allStates[indexInAllStates++] = state; 
}
bool genericAction() {
    return 1;
}
/***
 * initTransition - populates a transition struct
 *
 */
static void initTransition(stateTransition_t *transition, state_t *target, bool (*action)() ) {
//    transition = malloc(sizeof(stateTransition_t));
    if (transition == NULL){
        return;
    }
    transition->target = target;
    transition->action = genericAction;
    
    //transition->action = action;
}

static int addTransition(char *stateName, stateTransition_t *trans) {
    state_t *state = findState(stateName);
    if (state == NULL) return -1;
    if (state->transitionCounter >= state->numTransitions) {
        printf("WARN\n");
        return -1;
    }
    state->transitions[state->transitionCounter++] = trans;
    return 0;
}

static int initPowerOff(state_t *powerOff) {
    initTransition(powerOff->transitions[0], findState(IDLE_NAME), NULL);
    initTransition(powerOff->transitions[1], findState(PRE_RUN_FAULT_NAME), NULL); 
    addTransition(POWER_OFF_NAME, powerOff->transitions[0]);
    addTransition(POWER_OFF_NAME, powerOff->transitions[1]);
    return 0;
}

static int initIdle(state_t *idle) {

    initTransition(idle->transitions[0], findState(READY_FOR_PUMPDOWN_NAME), NULL);
    initTransition(idle->transitions[1], findState(PRE_RUN_FAULT_NAME), NULL);
    addTransition(IDLE_NAME, idle->transitions[0]);
    addTransition(IDLE_NAME, idle->transitions[1]);
    return 0;
}

static int initReadyForPumpdown(state_t *readyForPumpdown) {

    initTransition(readyForPumpdown->transitions[0], findState(PUMPDOWN_NAME), NULL);
    initTransition(readyForPumpdown->transitions[1], findState(PRE_RUN_FAULT_NAME), NULL);
    addTransition(READY_FOR_PUMPDOWN_NAME, readyForPumpdown->transitions[0]);
    addTransition(READY_FOR_PUMPDOWN_NAME, readyForPumpdown->transitions[1]);
    return 0;
}

static int initPumpdown(state_t *pumpdown) {

    initTransition(pumpdown->transitions[0], findState(READY_NAME), NULL);
    initTransition(pumpdown->transitions[1], findState(PRE_RUN_FAULT_NAME), NULL);
    addTransition(PUMPDOWN_NAME, pumpdown->transitions[0]);
    addTransition(PUMPDOWN_NAME, pumpdown->transitions[1]);
    return 0;
}

static int initReadyForLaunch(state_t *ready) {

    initTransition(ready->transitions[0], findState(PROPULSION_NAME), NULL);
    initTransition(ready->transitions[1], findState(RUN_FAULT_NAME), NULL);
    addTransition(READY_NAME, ready->transitions[0]);
    addTransition(READY_NAME, ready->transitions[1]);
    return 0;
}

static int initPropulsion(state_t *propulsion) {

    initTransition(propulsion->transitions[0], findState(BRAKING_NAME), NULL);
    initTransition(propulsion->transitions[1], findState(RUN_FAULT_NAME), NULL);
    addTransition(PROPULSION_NAME, propulsion->transitions[0]);
    addTransition(PROPULSION_NAME, propulsion->transitions[1]);

    return 0;
}

static int initBraking(state_t *braking) {

    initTransition(braking->transitions[0], findState(CRAWL_NAME), NULL);
    initTransition(braking->transitions[1], findState(STOPPED_NAME), NULL);
    initTransition(braking->transitions[2], findState(RUN_FAULT_NAME), NULL);
    addTransition(BRAKING_NAME, braking->transitions[0]);
    addTransition(BRAKING_NAME, braking->transitions[1]);
    addTransition(BRAKING_NAME, braking->transitions[2]);

    return 0;
}

static int initCrawl(state_t *crawl) {

    initTransition(crawl->transitions[0], findState(STOPPED_NAME), NULL);
    initTransition(crawl->transitions[1], findState(BRAKING_NAME), NULL);
    initTransition(crawl->transitions[2], findState(RUN_FAULT_NAME), NULL);
    addTransition(CRAWL_NAME, crawl->transitions[0]);
    addTransition(CRAWL_NAME, crawl->transitions[1]);
    addTransition(CRAWL_NAME, crawl->transitions[2]);
    
    return 0;
}

static int initStopped(state_t *stopped) {


    initTransition(stopped->transitions[0], findState(POST_RUN_NAME), NULL);
    initTransition(stopped->transitions[1], findState(RUN_FAULT_NAME), NULL);
    initTransition(stopped->transitions[2], findState(CRAWL_NAME), NULL);
    addTransition(STOPPED_NAME, stopped->transitions[0]);
    addTransition(STOPPED_NAME, stopped->transitions[1]);
    addTransition(STOPPED_NAME, stopped->transitions[2]);

    return 0;
}

static int initPostRun(state_t *postRun) {
    initTransition(postRun->transitions[0], findState(SAFE_TO_APPROACH_NAME), NULL);
    initTransition(postRun->transitions[1], findState(POST_RUN_FAULT_NAME), NULL);
    addTransition(POST_RUN_NAME, postRun->transitions[0]);
    addTransition(POST_RUN_NAME, postRun->transitions[1]);
    return 0;
}

static int initSafeToApproach(state_t *safeToApproach) {
    
    initTransition(safeToApproach->transitions[0], findState(POST_RUN_FAULT_NAME), NULL);
    addTransition(SAFE_TO_APPROACH_NAME, safeToApproach->transitions[0]);

    return 0;
}
/***
 * findTransition - Looks through a passed in states list of transitions
 * 	and identifies the one that leads to a specified target
 *
 * ARGS: state_t srcState - The state whose transitions we are searching through
 * 		 char *targName	  - The name of the state we want a transition to
 *
 * RETURNS: stateTransition_t the transition to the state we want, or NULL
 * 	if no such transition exists.
 */
stateTransition_t *findTransition(state_t *srcState, char *targName) {
    for (int i = 0; i < srcState->numTransitions; i++) {
        if (strcmp(srcState->transitions[i]->target->name, targName) == 0) {
            return srcState->transitions[i];
        }
    }
    return NULL;
}

/***
 * runStateMachine -
 *		Executes the current states action. A mini control loop
 * 	that will be called on every iteration of the overarching control loop. It
 *  will be able to then check if the current state should transition, and if so
 *  it should be able to execute that transition action as well and change the
 *  current state.
 *		It also is responsible for dealing with incoming state commands
 *  from the dashboard, after they have been parsed and appropriate flags have been
 *  set elsewhere.
 *
 */
void runStateMachine(void) {
    /* The cmd receiver will populate this field if we get an override */
    if (strcmp(stateMachine.overrideStateName, BLANK_NAME) != 0) {
    
        state_t *tempState = findState(stateMachine.overrideStateName);
        /* TODO We also need to execute a transition if it exists here */
        if (tempState != NULL) {
            stateTransition_t *trans = findTransition(tempState, stateMachine.overrideStateName);
            if (trans != NULL) trans->action();
            stateMachine.currState = tempState;
        }
        strcpy(stateMachine.overrideStateName, BLANK_NAME);
    }
    /* execute the state and check if we should be transitioning */
	stateTransition_t *transition = stateMachine.currState->action();
    if (transition != NULL) {
        transition->action();
		stateMachine.currState = transition->target;
	}
}

/***
 * buildStateMachine - allocates memory for each state and creates the
 * 	state machine struct that will contain the meta data for the whole
 *  state machine. Its a globally available because it will be so commonly
 *  used.
 *
 */
void buildStateMachine(void) {
	    /* Create all of the states*/
    
    stateMachine.allStates = malloc(sizeof(state_t *) * NUM_STATES);
    
    for (int i = 0; i < NUM_STATES; i++) {
        stateMachine.allStates[i] = malloc(sizeof(state_t));
    }
    
    initState(stateMachine.allStates[0], POWER_OFF_NAME, powerOnAction, 2);   
    initState(stateMachine.allStates[1], IDLE_NAME, idleAction, 2);
    initState(stateMachine.allStates[2], READY_FOR_PUMPDOWN_NAME, readyForPumpdownAction, 2);
    initState(stateMachine.allStates[3], PUMPDOWN_NAME, pumpdownAction, 2);
    initState(stateMachine.allStates[4], READY_NAME, readyForLaunchAction, 2);
    initState(stateMachine.allStates[5], PROPULSION_NAME, propulsionAction, 2);
    initState(stateMachine.allStates[6], BRAKING_NAME, brakingAction, 3);
    initState(stateMachine.allStates[7], CRAWL_NAME, crawlAction, 3);
    initState(stateMachine.allStates[8], STOPPED_NAME, stoppedAction, 3);
    initState(stateMachine.allStates[9], POST_RUN_NAME, postRunAction, 2);
    initState(stateMachine.allStates[10], SAFE_TO_APPROACH_NAME, safeToApproachAction, 1);
    initState(stateMachine.allStates[11], PRE_RUN_FAULT_NAME, preFaultAction, 0);
    initState(stateMachine.allStates[12], RUN_FAULT_NAME, runFaultAction, 0);
    initState(stateMachine.allStates[13], POST_RUN_FAULT_NAME, postFaultAction, 0);
    
    initPowerOff(stateMachine.allStates[0]);
    initIdle( stateMachine.allStates[1]);
    initReadyForPumpdown(stateMachine.allStates[2]);
    initPumpdown(stateMachine.allStates[3]);
    initReadyForLaunch( stateMachine.allStates[4]);
	initPropulsion(stateMachine.allStates[5]);
    initBraking(stateMachine.allStates[6]);
    initStopped(stateMachine.allStates[7]);
    initCrawl(stateMachine.allStates[8]);
    initPostRun(stateMachine.allStates[9]);
    initSafeToApproach(stateMachine.allStates[10]);
 
    stateMachine.currState = stateMachine.allStates[0];

    stateMachine.overrideStateName = malloc(21); // Longest state name is "readyForPropulsion" -- 18 char
    strcpy(stateMachine.overrideStateName, BLANK_NAME);
    if(stateMachine.overrideStateName == NULL) {
        fprintf(stderr, "Malloc error -- state machine override state machine name\n");
        exit(1);
    }
}

