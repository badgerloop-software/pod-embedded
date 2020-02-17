//
// Created by kouchekinia on 2/16/20.
//

#ifndef PODEMBEDDED_TESTSTATE_H
#define PODEMBEDDED_TESTSTATE_H

#include "State.h"

class TestState: public State {
    char* getName() override;
    State *run(std::ostream *logStream) override;
    int numberOfRuns;
public:
    explicit TestState(State *previousState);
};


#endif //PODEMBEDDED_TESTSTATE_H
