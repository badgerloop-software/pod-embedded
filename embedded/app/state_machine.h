#include <stdbool.h>
#include <stdint.h>

#define NUM_STATES 17



void buildStateMachine();

void runStateMachine();

/*
* The state machine is a directed graph. Each edge is a transition
* The state_t handles the nodes and stateTransition_t is an edge
*
*
 */

typedef struct stateTransition_t {
	state_t *target;
	bool (*action)();
} stateTransition_t

/* 	struct: state_t
* 	fields:
*		action = The thing that the state does. If it should transition it returns
*			a pointer to the transiton, otherwise NULL
*		name = The name of the state
*		transitions = A list of all transitions exiting the state
*/

typedef struct state_t {
	stateTransition_t *(*action)();
	char *name;
	stateTransition_t **transitions;
} state_t

typedef struct stateMachine_t {
	state_t *currState;
	state_t **allStates;
} stateMachine_t;

stateMachine_t stateMachine;
