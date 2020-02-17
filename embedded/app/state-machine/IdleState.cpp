#include "States.h"

States::IdleState::IdleState(State *previousState) : State(previousState) {
    this -> numberOfRuns = 0;
}

State* States::IdleState::run(std::ostream *logStream) {
    return (this->numberOfRuns++ < 5 ? reinterpret_cast<State *> (this) : new States::TestState(this));
}

char* States::IdleState::getName() {
    return "Idle State";
}