#include <iostream>
#include "StateMachine.h"
#include "States.h"
#include "ostream"


/**
 * Initializes a new StateMachine
 * @param initialState The first state in the machine
 * @param logStream (optional) Where log messages should be sent. logStream is set to std::cout by default.
 */
StateMachine::StateMachine(State *initialState, std::ostream* logStream) {
    this -> logStream = logStream;
    this -> currentState = initialState;

    *logStream << "States initialized with initial state '" << initialState -> getName() << "'.\n";
}

/**
 * Runs the state machine once, changing the previouslyExecutedState if necessary
 * @return true if the previouslyExecutedState changed
 */
bool StateMachine::crank() {
    if(this -> isHalted())
        return false;

    // Keep track of the previous state
    State *lastState = this -> currentState;

    // Run the current state and update the current one
    this->currentState = this ->currentState->run(this->logStream);

    // Check for null state to halt machine
    if(this -> currentState == nullptr) {
        *this->logStream << "StateMachine halted by '" << lastState->getName() << "'.\n";
        return true;
    }

    *this->logStream << lastState->getName() << " run.\n";

    // Determine if the state has changed by comparing the pointers
    bool stateChanged = lastState != this->currentState;

    // Print a message when the state has been changed
    if(stateChanged) {
        *this->logStream << "States changed from '" << lastState->getName() << "' to '"
                         << this->currentState->getName() << ".\n";
        // De-allocate previous state
        delete(lastState);
        lastState = nullptr;
    }

    return stateChanged;
}

/**
 * Retrieve a pointer to the current state, or null if the machine is halted
 * @return a pointer to the current state, or null if the machine is halted
 */
State* StateMachine::getState() {
 return this->currentState;
}

/**
 * Overwrites the current state or resumes a halted machine
 * @param state The new state of the machine
 */
void StateMachine::overwriteState(State *state) {
    *this->logStream << (this->isHalted() ? "Halted state resumed with new state '" : "States overridden by '") << *state->getName() << "'.\n";
    this->currentState = state;
}
/**
 * Returns whether or not the state machine is halted
 * @return true if the state machine is halted
 */
bool StateMachine::isHalted() {
    return this->currentState == nullptr;
}

/**
 * Destructor
 */
StateMachine::~StateMachine() {
    delete(this->currentState);
}