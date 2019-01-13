#include <stdio.h>


void runStateMachine() {
	stateTransition_t *transition = stateMachine->currState->action();
	if (transition != NULL) {
		transition->action();
		stateMachine->currState = transition->target;
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

		initState(powerOn);
		initState(idle);
		initState(readyForPumpdown, "readyForPumpdown", stateTransition_t *(*readyForPumpdownAction)());

/* Create all of the transitions */
}

void initState(state_t)

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
