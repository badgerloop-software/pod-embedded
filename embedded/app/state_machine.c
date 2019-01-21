#include <stdio.h>
#include <string.h>


void runStateMachine() {
	stateTransition_t *transition = stateMachine->currState->action();
	if (transition != NULL) {
		transition->action();
		stateMachine->currState = transition->target;
	}
}


//What is the point of the initState function? 
//Does it simply create fill in the fields of the state_t pointer using the arguments passed in?
//That's my assumption for the following work, if not delete the body of the following function
		initState(powerOn, "powerOn", stateTransition_t *(*powerOnAction)());
void initState(state_t* state, char* name, stateTransition_t *(*action)() ){

   strncpy(state->name, name, strlen(name) );
   state->action = action; //TODO: not sure if I did this right, out of practice with function pointers in structs

   //TODO: Transitions need to be added 
}


//TODO: So all these function handles are representing what each state is actually doing?
//TODO: If so, I'm assuming the transitions still need to be created?
//TODO: Are you planning on creating a "tranisition in" and "transition out" function for each state?
//TODO: And the 'transitions' field in the state_t struct is an array holding function pointers to said functions?

//TODO: Please let me know the answers to the above^ Q's either in person, in a slack message, or if you really want you can answer them in the comments of your next commit lol
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


void buildStateMachine() {
	    /* Create all of the states*/
		state_t *powerOn, *idle, *readyForPumpdown, *pumpdown,
			*readyForLaunch, *propulsion, *braking, *secondaryBraking,
			*stopped, *crawl, *rebrake, *postRun, *safeToApproach, *preFault,
			*runFault, *postFault;
		state_t **allStates = malloc(NUM_STATES * sizeof(*state_t));

        //TODO: what are you planning to use the over-arching stateMachine for?
		stateMachine.allStates = allStates;

        /* Allocating space for each state */
		powerOn = malloc(sizeof(state_t));
		idle = malloc(sizeof(state_t));
		readyForPumpdown = malloc(sizeof(state_t));
		pumpdown = malloc(sizeof(state_t));
		readyForLaunch = malloc(sizeof(state_t));
		propulsion = malloc(sizeof(state_t));
		braking = malloc(sizeof(state_t));
		secondaryBraking = malloc(sizeof(state_t));
		stopped = malloc(sizeof(state_t));
		crawl = malloc(sizeof(state_t));
		rebrake = malloc(sizeof(state_t));
		postRun = malloc(sizeof(state_t));
		safeToApproach = malloc(sizeof(state_t));
		preFault = malloc(sizeof(state_t));
		runFault = malloc(sizeof(state_t));
		postFault = malloc(sizeof(state_t));

		int stateCount = 0;

        /* Initializing states */
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
