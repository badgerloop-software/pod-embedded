/***
 * filename: state_machine.c
 *
 * summary: This is the collection of a lot of the pods data and
 * where it will check for faults. The pod will always be running this code and
 * the state machine that lives here is inherently global as it applies to the
 * whole pod.
 *
 ***/

#include "data.h"
#include "state_machine.h"
#include <transitions.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

static void initState(state_t* state, char* name, stateTransition_t *(*action)(), int numTrans);
static void initTransition(stateTransition_t *transition, state_t *target, int (*action)() );


extern stateTransition_t * idleAction(void);
extern stateTransition_t * pumpdownAction(void);
extern stateTransition_t * propulsionAction(void);
extern stateTransition_t * brakingAction(void);
extern stateTransition_t * stoppedAction(void);
extern stateTransition_t * servPrechargeAction(void);
extern stateTransition_t * crawlAction(void);
extern stateTransition_t * postRunAction(void);
extern stateTransition_t * safeToApproachAction(void);
extern stateTransition_t * runFaultAction(void);
extern stateTransition_t * nonRunFaultAction(void);



volatile stateMachine_t stateMachine;

/***
 * findState - Searches all the created states and returns the one with a matching name
 *
 * ARGS: char *stateName - Name of the state we are searching for. Check out state_machine.h
 * 	for options.
 *
 * RETURNS: state_t *, the found state or NULL if that state doesnt exist
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
 * findFaultState - Looks through a states transitions and finds its
 *  appropriate fault state
 *
 * ARGS state_t *state - The state whose transitions will be searched
 *
 * RETURNS state_t * - A pointer to the fault state
 */

static state_t * findFaultState(state_t *state) {
    int i = 0;
    for (i = 0; i < state->numTransitions; i++) {
        if (strncmp(state->transitions[i]->target->name, FAULT, 5)) {
            return state->transitions[i]->target;
        }
    }
    fprintf(stderr, "This state has no fault? Needs to be fixed\n");
    return NULL;    /* This should never happen, every state can fault */
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


/***
 * initTransition - populates a transition struct
 *
 */
static void initTransition(stateTransition_t *transition, state_t *target, int (*action)() ) {
//    transition = malloc(sizeof(stateTransition_t));
    if (transition == NULL){
        return;
    }
    transition->target = target;
    transition->action = action;
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

static int initIdle(state_t *idle) {

    initTransition(idle->transitions[0], findState(PUMPDOWN_NAME), genTranAction);
    initTransition(idle->transitions[1], findState(NON_RUN_FAULT_NAME), genTranAction);
    addTransition(IDLE_NAME, idle->transitions[0]);
    addTransition(IDLE_NAME, idle->transitions[1]);
    
    idle->fault = idle->transitions[1];

    return 0;
}

static int initPumpdown(state_t *pumpdown) {

    initTransition(pumpdown->transitions[0], findState(PROPULSION_NAME), genTranAction);
    initTransition(pumpdown->transitions[1], findState(RUN_FAULT_NAME), genTranAction);
    addTransition(PUMPDOWN_NAME, pumpdown->transitions[0]);
    addTransition(PUMPDOWN_NAME, 
            pumpdown->fault = pumpdown->transitions[1]);


    return 0;
}

static int initPropulsion(state_t *propulsion) {

    initTransition(propulsion->transitions[0], findState(BRAKING_NAME), toBraking);
    initTransition(propulsion->transitions[1], findState(RUN_FAULT_NAME), genTranAction);
    addTransition(PROPULSION_NAME, propulsion->transitions[0]);
    addTransition(PROPULSION_NAME, 
            propulsion->fault = propulsion->transitions[1]);

    return 0;
}

static int initBraking(state_t *braking) {

    initTransition(braking->transitions[0], findState(CRAWL_NAME), toCrawl);
    initTransition(braking->transitions[1], findState(STOPPED_NAME), genTranAction);
    initTransition(braking->transitions[2], findState(RUN_FAULT_NAME), genTranAction);
    addTransition(BRAKING_NAME, braking->transitions[0]);
    addTransition(BRAKING_NAME, braking->transitions[1]);
    addTransition(BRAKING_NAME, 
            braking->fault = braking->transitions[2]);

    return 0;
}

static int initServPrecharge(state_t *servPrecharge) {
    initTransition(servPrecharge->transitions[0], findState(CRAWL_NAME), toCrawl);
    initTransition(servPrecharge->transitions[1], findState(RUN_FAULT_NAME), genTranAction);
    addTransition(SERV_PRECHARGE_NAME, servPrecharge->transitions[0]);
    addTransition(SERV_PRECHARGE_NAME,
            servPrecharge->fault = servPrecharge->transitions[1]);
}

static int initCrawl(state_t *crawl) {

    initTransition(crawl->transitions[1], findState(POST_RUN_NAME), toBraking);
    initTransition(crawl->transitions[2], findState(RUN_FAULT_NAME), genTranAction);
    addTransition(CRAWL_NAME, crawl->transitions[0]);
    addTransition(CRAWL_NAME, 
            crawl->fault = crawl->transitions[1]);
    
    return 0;
}

static int initStopped(state_t *stopped) {


    initTransition(stopped->transitions[0], findState(POST_RUN_NAME), genTranAction);
    initTransition(stopped->transitions[1], findState(CRAWL_NAME), toCrawl);
    initTransition(stopped->transitions[2], findState(RUN_FAULT_NAME), genTranAction);
    addTransition(STOPPED_NAME, stopped->transitions[0]);
    addTransition(STOPPED_NAME, stopped->transitions[1]);
    addTransition(STOPPED_NAME, 
            stopped->fault = stopped->transitions[2]);

    return 0;
}

static int initPostRun(state_t *postRun) {
    initTransition(postRun->transitions[0], findState(SAFE_TO_APPROACH_NAME), genTranAction);
    initTransition(postRun->transitions[1], findState(RUN_FAULT_NAME), genTranAction);
    addTransition(POST_RUN_NAME, postRun->transitions[0]);
    addTransition(POST_RUN_NAME, 
            postRun->fault = postRun->transitions[1]);
    return 0;
}

static int initSafeToApproach(state_t *safeToApproach) {
    initTransition(safeToApproach->transitions[0], findState(NON_RUN_FAULT_NAME), genTranAction);
    addTransition(SAFE_TO_APPROACH_NAME, 
            safeToApproach->fault = safeToApproach->transitions[0]);
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

state_t *getCurrState(void) {
    return stateMachine.currState;
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
        if (transition->action() == 0) 
            stateMachine.currState = transition->target;
        else 
            stateMachine.currState = findFaultState(stateMachine.currState);
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
    
    initState(stateMachine.allStates[0], IDLE_NAME, idleAction, 2);
    initState(stateMachine.allStates[1], PUMPDOWN_NAME, pumpdownAction, 2);
    initState(stateMachine.allStates[2], PROPULSION_NAME, propulsionAction, 2);
    initState(stateMachine.allStates[3], BRAKING_NAME, brakingAction, 3);
    initState(stateMachine.allStates[4], SERV_PRECHARGE_NAME, servPrechargeAction, 2);
    initState(stateMachine.allStates[5], CRAWL_NAME, crawlAction, 3);
    initState(stateMachine.allStates[6], STOPPED_NAME, stoppedAction, 3);
    initState(stateMachine.allStates[7], POST_RUN_NAME, postRunAction, 2);
    initState(stateMachine.allStates[8], SAFE_TO_APPROACH_NAME, safeToApproachAction, 1);
    initState(stateMachine.allStates[9], NON_RUN_FAULT_NAME, nonRunFaultAction, 0);
    initState(stateMachine.allStates[10], RUN_FAULT_NAME, runFaultAction, 0);
    
    initIdle( stateMachine.allStates[0]);
    initPumpdown(stateMachine.allStates[1]);
	initPropulsion(stateMachine.allStates[2]);
    initBraking(stateMachine.allStates[3]);
    initStopped(stateMachine.allStates[4]);
    initServPrecharge(stateMachine.allStates[5]);
    initCrawl(stateMachine.allStates[6]);
    initPostRun(stateMachine.allStates[7]);
    initSafeToApproach(stateMachine.allStates[8]);
 
    stateMachine.currState = stateMachine.allStates[0];

    stateMachine.overrideStateName = malloc(21); // Longest state name is "readyForPropulsion" -- 18 char
    strcpy(stateMachine.overrideStateName, BLANK_NAME);
    if(stateMachine.overrideStateName == NULL) {
        fprintf(stderr, "Malloc error -- state machine override state machine name\n");
        exit(1);
    }
}

