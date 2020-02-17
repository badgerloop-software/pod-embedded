#include "States.h"

States::TestState::TestState(State *previousState) : State(previousState) {
    this -> numberOfRuns = 0;
}

State* States::TestState::run(std::ostream *logStream) {
    return this->numberOfRuns++ < 5 ? this : nullptr;
}

char* States::TestState::getName() {
    return "Test State";
}
