//
// Created by kouchekinia on 2/16/20.
//

#ifndef PODEMBEDDED_IDLESTATE_H
#define PODEMBEDDED_IDLESTATE_H

#include "State.h"

class IdleState: public State {
    char* getName() override;
    State *run(std::ostream *logStream) override;
    int numberOfRuns;
public:
    explicit IdleState(State *previousState);
};

#endif //PODEMBEDDED_IDLESTATE_H
