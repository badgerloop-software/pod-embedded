#ifndef PODEMBEDDED_STATEMACHINE_H
#define PODEMBEDDED_STATEMACHINE_H

#include "States.h"
#include <ostream>
#include <iostream>

class StateMachine {
private:
    State* currentState;
    std::ostream* logStream;
public:
    explicit StateMachine(State* initialState, std::ostream* logStream = &std::cout);
    ~StateMachine();
    bool crank();
    State* getState();
    void overwriteState(State *state);
    bool isHalted();
};


#endif //PODEMBEDDED_STATEMACHINE_H
