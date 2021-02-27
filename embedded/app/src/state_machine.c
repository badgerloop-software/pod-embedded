/***
 * filename: state_machine.c
 *
 * summary: This is the collection of a lot of the pods data and
 * where it will check for faults. The pod will always be running this code and
 * the state machine that lives here is inherently global as it applies to the
 * whole pod.
 *
 ***/

#include "state_machine.h"
#include "data.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <transitions.h>
static void initState(state_t* state, char* name, stateTransition_t* (*action)(), int numTrans);
static void initTransition(stateTransition_t* transition, state_t* target, int (*action)());

extern stateTransition_t* idleAction(void);
extern stateTransition_t* pumpdownAction(void);
extern stateTransition_t* propulsionAction(void);
extern stateTransition_t* brakingAction(void);
extern stateTransition_t* stoppedAction(void);
extern stateTransition_t* servPrechargeAction(void);
extern stateTransition_t* crawlAction(void);
extern stateTransition_t* postRunAction(void);
extern stateTransition_t* safeToApproachAction(void);
extern stateTransition_t* runFaultAction(void);
extern stateTransition_t* nonRunFaultAction(void);

volatile stateMachine_t stateMachine;

/**
 * findState - Searches all the created states and returns the one with a matching name
 *
 * ARGS: char *stateName - Name of the state we are searching for. Check out state_machine.h
 * 	for options.
 *
 * RETURNS: state_t *, the found state or NULL if that state doesnt exist
 */
state_t* findState(char* stateName)
{
    for (int i = 0; i < NUM_STATES; i++) {
        if (stateMachine.allStates[i] == NULL)
            continue;
        if (strcmp(stateMachine.allStates[i]->name, stateName) == 0) {
            return stateMachine.allStates[i];
        }
    }
    return NULL;
}

/**
 * getNumStates - Returns the number of created states
 * 
 * RETURNS: int, the number of created states
 */
int getNumStates(void)
{
    return sizeof(stateMachine.allStates) / sizeof(stateMachine.allStates[0]);
}

/***
 * initState - fills in the fields of the state_t struct.
 *
 * ARGS: state_t *state - allocated state (malloced most likely),
 *       stateTransition_t *(*action)() - a function pointer to a function that returns a state
 *			transition pointer. This is the meat of the state
 * 		 numTransitions - not implemented, fairly self explanatory
 */
static void initState(state_t* state, char* name, stateTransition_t* (*action)(), int numTransitions)
{
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
    state->transitions = malloc(numTransitions * (sizeof(stateTransition_t*)));
    for (i = 0; i < numTransitions; i++) {
        state->transitions[i] = malloc(sizeof(stateTransition_t));
    }
    stateMachine.allStates[indexInAllStates++] = state;
}

/***
 * initTransition - populates a transition struct
 *
 */
static void initTransition(stateTransition_t* transition, state_t* target, int (*action)())
{
    //    transition = malloc(sizeof(stateTransition_t));
    if (transition == NULL) {
        return;
    }
    transition->target = target;
    transition->action = action;
}

static int addTransition(char* stateName, stateTransition_t* trans)
{
    state_t* state = findState(stateName);
    if (state == NULL)
        return -1;
    if (state->transitionCounter >= state->numTransitions) {
        printf("WARN\n");
        return -1;
    }
    state->transitions[state->transitionCounter++] = trans;
    return 0;
}

static int initIdle(state_t* idle)
{
    initTransition(idle->transitions[0], findState(RUN_FAULT_NAME), genRunFault);
    addTransition(IDLE_NAME, idle->transitions[0]);
    idle->fault = idle->transitions[0];
    idle->next = NULL;
    idle->begin = genIdle;
    return 0;
}

static int initPumpdown(state_t* pumpdown)
{

    initTransition(pumpdown->transitions[0], findState(NON_RUN_FAULT_NAME), genNonRunFault);
    addTransition(PUMPDOWN_NAME, pumpdown->transitions[0]);
    pumpdown->fault = pumpdown->transitions[0];
    pumpdown->next = NULL;
    pumpdown->begin = genPumpdown;
    return 0;
}

static int initPropulsion(state_t* propulsion)
{

    initTransition(propulsion->transitions[0], findState(BRAKING_NAME), genBraking);
    initTransition(propulsion->transitions[1], findState(RUN_FAULT_NAME), genRunFault);
    addTransition(PROPULSION_NAME, propulsion->transitions[0]);
    addTransition(PROPULSION_NAME, propulsion->transitions[1]);
    propulsion->fault = propulsion->transitions[1];
    propulsion->next = propulsion->transitions[0];
    propulsion->begin = genPropulsion;
    return 0;
}

static int initBraking(state_t* braking)
{

    initTransition(braking->transitions[0], findState(STOPPED_NAME), genStopped);
    initTransition(braking->transitions[1], findState(RUN_FAULT_NAME), genRunFault);
    addTransition(BRAKING_NAME, braking->transitions[0]);
    addTransition(BRAKING_NAME, braking->transitions[1]);
    braking->fault = braking->transitions[1];

    braking->next = braking->transitions[0];
    braking->begin = genBraking;
    return 0;
}

static int initServPrecharge(state_t* servPrecharge)
{
    initTransition(servPrecharge->transitions[0], findState(RUN_FAULT_NAME), genRunFault);
    addTransition(SERV_PRECHARGE_NAME, servPrecharge->transitions[0]);
    servPrecharge->fault = servPrecharge->transitions[0];

    servPrecharge->next = NULL;
    servPrecharge->begin = genServPrecharge;
    return 0;
}

static int initCrawl(state_t* crawl)
{

    initTransition(crawl->transitions[0], findState(BRAKING_NAME), genBraking);
    initTransition(crawl->transitions[1], findState(RUN_FAULT_NAME), genRunFault);
    addTransition(CRAWL_NAME, crawl->transitions[0]);
    addTransition(CRAWL_NAME, crawl->transitions[1]);

    crawl->fault = crawl->transitions[1];
    crawl->next = crawl->transitions[0];
    crawl->begin = genCrawl;
    return 0;
}

static int initStopped(state_t* stopped)
{
    initTransition(stopped->transitions[0], findState(RUN_FAULT_NAME), genRunFault);
    addTransition(STOPPED_NAME, stopped->transitions[0]);
    stopped->fault = stopped->transitions[0];
    stopped->next = NULL;
    stopped->begin = genStopped;
    return 0;
}

static int initPostRun(state_t* postRun)
{
    initTransition(postRun->transitions[0], findState(RUN_FAULT_NAME), genRunFault);
    addTransition(POST_RUN_NAME, postRun->transitions[0]);
    postRun->fault = postRun->transitions[0];
    postRun->next = NULL;
    postRun->begin = genPostRun;
    return 0;
}

static int initSafeToApproach(state_t* safeToApproach)
{
    initTransition(safeToApproach->transitions[0], findState(NON_RUN_FAULT_NAME), genNonRunFault);
    addTransition(SAFE_TO_APPROACH_NAME, safeToApproach->transitions[0]);
    safeToApproach->fault = safeToApproach->transitions[0];
    safeToApproach->next = NULL;
    safeToApproach->begin = genTranAction;
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
stateTransition_t* findTransition(state_t* srcState, char* targName)
{
    for (int i = 0; i < srcState->numTransitions; i++) {
        if (strcmp(srcState->transitions[i]->target->name, targName) == 0) {
            return srcState->transitions[i];
        }
    }
    return NULL;
}
state_t* getCurrState(void);
state_t* getCurrState()
{
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
void runStateMachine(void)
{
    /* The cmd receiver will populate this field if we get an override */
    if (strcmp(stateMachine.overrideStateName, BLANK_NAME) != 0) {
        state_t* tempState = findState(stateMachine.overrideStateName);
        if (tempState != NULL) {
            stateTransition_t* trans = findTransition(stateMachine.currState, stateMachine.overrideStateName);
            if (trans != NULL) {
                trans->action();
            } else {
                tempState->begin();
            }
            stateMachine.currState = tempState;
        }
        strcpy(stateMachine.overrideStateName, BLANK_NAME);
        return;
    }
    /* execute the state and check if we should be transitioning */
    stateTransition_t* transition = stateMachine.currState->action();
    if (transition != NULL) {
        /*        printf("transAction->%s\n", transition->target->name);*/
        if (transition->action() == 0) {
            stateMachine.currState = transition->target;
        } else {
            stateMachine.currState = stateMachine.currState->fault->target;
        }
        if (transition->target->begin != NULL) {
            transition->target->begin();
        }
    }
}

/***
 * buildStateMachine - allocates memory for each state and creates the
 * 	state machine struct that will contain the meta data for the whole
 *  state machine. Its a globally available because it will be so commonly
 *  used.
 *
 */
void buildStateMachine(void)
{
    /* Create all of the states*/
    // printf("Begin Allocation of states\n");
    stateMachine.allStates = malloc(sizeof(state_t*) * NUM_STATES);

    for (int i = 0; i < NUM_STATES; i++) {
        stateMachine.allStates[i] = malloc(sizeof(state_t));
    }
    // printf("Begin Initalization of states\n");
    initState(stateMachine.allStates[0], IDLE_NAME, idleAction, 1);
    initState(stateMachine.allStates[1], PUMPDOWN_NAME, pumpdownAction, 1);
    initState(stateMachine.allStates[2], PROPULSION_NAME, propulsionAction, 2);
    initState(stateMachine.allStates[3], BRAKING_NAME, brakingAction, 2);
    initState(stateMachine.allStates[4], SERV_PRECHARGE_NAME, servPrechargeAction, 1);
    initState(stateMachine.allStates[5], CRAWL_NAME, crawlAction, 2);
    initState(stateMachine.allStates[6], STOPPED_NAME, stoppedAction, 1);
    initState(stateMachine.allStates[7], POST_RUN_NAME, postRunAction, 1);
    initState(stateMachine.allStates[8], SAFE_TO_APPROACH_NAME, safeToApproachAction, 1);
    initState(stateMachine.allStates[9], NON_RUN_FAULT_NAME, nonRunFaultAction, 0);
    initState(stateMachine.allStates[10], RUN_FAULT_NAME, runFaultAction, 0);

    // printf("Begin Initalization of indiv. states\n");
    initIdle(stateMachine.allStates[0]);
    initPumpdown(stateMachine.allStates[1]);
    initPropulsion(stateMachine.allStates[2]);
    initBraking(stateMachine.allStates[3]);
    initServPrecharge(stateMachine.allStates[4]);
    initCrawl(stateMachine.allStates[5]);
    initStopped(stateMachine.allStates[6]);
    initPostRun(stateMachine.allStates[7]);
    initSafeToApproach(stateMachine.allStates[8]);

    // printf("Set current state\n");
    stateMachine.currState = stateMachine.allStates[0];
    // printf("begin\n");
    stateMachine.currState->begin();

    // printf("Allocate overide state name\n");
    stateMachine.overrideStateName = malloc(21); // Longest state name is "readyForPropulsion" -- 18 char
    strcpy(stateMachine.overrideStateName, BLANK_NAME);
    if (stateMachine.overrideStateName == NULL) {
        fprintf(stderr, "Malloc error -- state machine override state machine name\n");
        exit(1);
    }
}

void destroyStateMachine(void)
{

    /* Free all of the states */
    /* Unrelated to the events of 1861 - 1865 */
    // printf("DESTROYING STATE MACHINE\n");
    for (int i = 0; i < NUM_STATES; i++) {
        free(stateMachine.allStates[i]);
    }
    free(stateMachine.allStates);

    stateMachine.currState = NULL;
    free(stateMachine.overrideStateName);
}
