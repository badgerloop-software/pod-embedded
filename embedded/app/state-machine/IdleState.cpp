#include "States.h"

using namespace States;

IdleState::IdleState(State *previousState) : State(previousState) {
    this -> numberOfRuns = 0;
}

State* IdleState::run(std::ostream *logStream) {
    return (this->numberOfRuns++ < 5 ? reinterpret_cast<State *> (this) : new States::TestState(this));
}

char* IdleState::getName() {
    return "Idle State";
}