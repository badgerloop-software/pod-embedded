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



stateMachine_t stateMachine;

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
    
    state = malloc(sizeof(state_t));
    if (state == NULL) {
        STATE_ERROR();
    }
    strncpy(state->name, name, strlen(name) );
    state->action = action;
    state->numTransitions = numTransitions;
    state->transitions = malloc(numTransitions * (sizeof(stateTransition_t *)));
    for (i = 0; i < numTransitions; i++) {
        state->transitions[i] = malloc(sizeof(stateTransition_t));
        state->transitions[i]->target = NULL;
    }
    stateMachine.allStates[indexInAllStates++] = state;
    
}

/***
 * initTransition - populates a transition struct
 *
 */
static void initTransition(stateTransition_t *transition, state_t *target, bool (*action)() ) {
    transition = malloc(sizeof(stateTransition_t));
    if (transition == NULL){
        return;
    } 
    transition->target = target;
    transition->action = action;
}

static int addTransition(state_t *state, stateTransition_t *trans) {
    int i = 0;
    for (i = 0; state->transitions[i]->target != NULL || i < state->numTransitions; i++);
    if (i >= state->numTransitions) {
        fprintf(stderr, "[WARN] adding too many transitions! Ignoring extra.\n");
        return -1;
    }
    state->transitions[i] = trans;
    return 0;
}

static int initPowerOff(state_t *powerOff) {
    stateTransition_t *toIdle = NULL, *toPreFault = NULL;
	
    initState(powerOff, POWER_OFF_NAME, powerOnAction, 2);   
    
    initTransition(toIdle, findState(IDLE_NAME), NULL);
    initTransition(toPreFault, findState(PRE_RUN_FAULT_NAME), NULL); 
    addTransition(powerOff, toIdle);
    addTransition(powerOff, toPreFault);
    return 0;
}

static int initIdle(state_t *idle) {
    stateTransition_t *toReadyForPumpdown = NULL, *toPreFault = NULL;

    initState(idle, IDLE_NAME, idleAction, 2);

    initTransition(toReadyForPumpdown, findState(READY_FOR_PUMPDOWN_NAME), NULL);
    initTransition(toPreFault, findState(PRE_RUN_FAULT_NAME), NULL);
    addTransition(idle, toReadyForPumpdown);
    addTransition(idle, toPreFault);
    return 0;
}

static int initReadyForPumpdown(state_t *readyForPumpdown) {
    stateTransition_t *toPumpdown = NULL, *toPreFault = NULL;

    initState(readyForPumpdown, READY_FOR_PUMPDOWN_NAME, readyForPumpdownAction, 2);
    
    initTransition(toPumpdown, findState(PUMPDOWN_NAME), NULL);
    initTransition(toPreFault, findState(PRE_RUN_FAULT_NAME), NULL);
    addTransition(readyForPumpdown, toPumpdown);
    addTransition(readyForPumpdown, toPreFault);
    return 0;
}

static int initPumpdown(state_t *pumpdown) {
    stateTransition_t *toReadyForLaunch = NULL, *toPreFault = NULL;

    initState(pumpdown, PUMPDOWN_NAME, pumpdownAction, 2);

    initTransition(toReadyForLaunch, findState(READY_NAME), NULL);
    initTransition(toPreFault, findState(PRE_RUN_FAULT_NAME), NULL);
    addTransition(pumpdown, toReadyForLaunch);
    addTransition(pumpdown, toPreFault);
    return 0;
}

static int initReadyForLaunch(state_t *ready) {
    stateTransition_t *toPropulsion = NULL, *toRunFault = NULL;

    initState(ready, READY_NAME, readyForLaunchAction, 2);

    initTransition(toPropulsion, findState(PROPULSION_NAME), NULL);
    initTransition(toRunFault, findState(RUN_FAULT_NAME), NULL);
    addTransition(ready, toPropulsion);
    addTransition(ready, toRunFault);
    return 0;
}

static int initPropulsion(state_t *propulsion) {
    stateTransition_t *toBraking = NULL, *toRunFault = NULL;

    initState(propulsion, PROPULSION_NAME, propulsionAction, 2);

    initTransition(toBraking, findState(BRAKING_NAME), NULL);
    initTransition(toRunFault, findState(RUN_FAULT_NAME), NULL);
    addTransition(propulsion, toBraking);
    addTransition(propulsion, toRunFault);

    return 0;
}

static int initBraking(state_t *braking) {
    stateTransition_t *toCrawl = NULL, *toStopped = NULL, *toRunFault = NULL;

    initState(braking, BRAKING_NAME, brakingAction, 3);

    initTransition(toCrawl, findState(CRAWL_NAME), NULL);
    initTransition(toStopped, findState(STOPPED_NAME), NULL);
    initTransition(toRunFault, findState(RUN_FAULT_NAME), NULL);
    addTransition(braking, toCrawl);
    addTransition(braking, toStopped);
    addTransition(braking, toRunFault);

    return 0;
}

static int initCrawl(state_t *crawl) {
    stateTransition_t *toStopped = NULL, *toRunFault = NULL, *toBraking = NULL;

    initState(crawl, CRAWL_NAME, crawlAction, 3);

    initTransition(toStopped, findState(STOPPED_NAME), NULL);
    initTransition(toBraking, findState(BRAKING_NAME), NULL);
    initTransition(toRunFault, findState(RUN_FAULT_NAME), NULL);
    addTransition(crawl, toStopped);
    addTransition(crawl, toRunFault);
    addTransition(crawl, toBraking);
    
    return 0;
}

static int initStopped(state_t *stopped) {
    stateTransition_t *toPostRun = NULL, *toCrawl = NULL, *toRunFault = NULL;

    initState(stopped, STOPPED_NAME, stoppedAction, 3);

    initTransition(toPostRun, findState(POST_RUN_NAME), NULL);
    initTransition(toRunFault, findState(RUN_FAULT_NAME), NULL);
    initTransition(toCrawl, findState(CRAWL_NAME), NULL);
    addTransition(stopped, toPostRun);
    addTransition(stopped, toCrawl);
    addTransition(stopped, toRunFault);

    return 0;
}

static int initPostRun(state_t *postRun) {
    stateTransition_t *toSafeToApproach = NULL, *toPostFault = NULL;
    
    initState(postRun, POST_RUN_NAME, postRunAction, 2);

    initTransition(toSafeToApproach, findState(SAFE_TO_APPROACH_NAME), NULL);
    initTransition(toPostFault, findState(POST_RUN_FAULT_NAME), NULL);
    addTransition(postRun, toSafeToApproach);
    addTransition(postRun, toPostFault);

    return 0;
}

static int initSafeToApproach(state_t *safeToApproach) {
    stateTransition_t *toPostFault = NULL;

    initState(safeToApproach, SAFE_TO_APPROACH_NAME, safeToApproachAction, 1);
    
    initTransition(toPostFault, findState(POST_RUN_FAULT_NAME), NULL);
    addTransition(safeToApproach, toPostFault);

    return 0;
}

static int initPreFault(state_t *preFault) {
    initState(preFault, PRE_RUN_FAULT_NAME, preFaultAction, 0);
    return 0;
}

static int initRunFault(state_t *runFault) {
    initState(runFault, RUN_FAULT_NAME, runFaultAction, 0);
    return 0;
}

static int initPostFault(state_t *postFault) {
    initState(postFault, POST_RUN_FAULT_NAME, postFaultAction, 0);
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
        if (strcmp(srcState->transitions[i]->target->name, targName) == 0)
            return srcState->transitions[i];
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
    if (stateMachine.overrideStateName != NULL) {
        state_t *tempState = findState(stateMachine.overrideStateName);
        /* TODO We also need to execute a transition if it exists here */
        if (tempState != NULL) {
            stateTransition_t *trans = findTransition(tempState, stateMachine.overrideStateName);
            if (trans != NULL) trans->action();
            stateMachine.currState = tempState;
        }
        stateMachine.overrideStateName = NULL;
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
	state_t *powerOff = NULL, *idle = NULL, *readyForPumpdown = NULL, *pumpdown = NULL,
			*readyForLaunch = NULL, *propulsion = NULL, *braking = NULL, *secondaryBraking = NULL,
			*stopped = NULL, *crawl = NULL, *postRun = NULL, *safeToApproach = NULL, *preFault = NULL,
			*runFault = NULL, *postFault = NULL;
	stateMachine.allStates[0] = powerOff;
    stateMachine.allStates[1] = idle; 
    stateMachine.allStates[2] = readyForPumpdown;
    stateMachine.allStates[3] = pumpdown,
    stateMachine.allStates[4] = readyForLaunch;
    stateMachine.allStates[5] = propulsion;
    stateMachine.allStates[6] = braking;
    stateMachine.allStates[7] = secondaryBraking;
    stateMachine.allStates[8] = stopped;
    stateMachine.allStates[9] = crawl;
    stateMachine.allStates[10] = postRun; 
    stateMachine.allStates[11] = safeToApproach;
    stateMachine.allStates[12] = preFault;
    stateMachine.allStates[13] = runFault; 
    stateMachine.allStates[14] = postFault;
    
    
    initPowerOff(powerOff);
    initIdle(idle);
    initReadyForPumpdown(readyForPumpdown);
    initPumpdown(pumpdown);
    initReadyForLaunch(readyForLaunch);
	initPropulsion(propulsion);
    initBraking(braking);
    initStopped(stopped);
    initCrawl(crawl);
    initPostRun(postRun);
    initSafeToApproach(safeToApproach);
    initPreFault(preFault);
    initRunFault(runFault);
    initPostFault(postFault);

    stateMachine.currState = powerOff;
    
    stateMachine.overrideStateName = malloc(21); // Longest state name is "readyForPropulsion" -- 18 char
    
    if(stateMachine.overrideStateName == NULL) {
        fprintf(stderr, "Malloc error -- state machine override state machine name\n");
        exit(1);
    }
}

