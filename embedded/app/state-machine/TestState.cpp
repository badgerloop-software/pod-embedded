#include "TestState.h"

TestState::TestState(State *previousState) : State(previousState) {
    this -> numberOfRuns = 0;
}

State* TestState::run(std::ostream *logStream) {
    return this->numberOfRuns++ < 5 ? this : nullptr;
}

char* TestState::getName() {
    return "Test State";
}
