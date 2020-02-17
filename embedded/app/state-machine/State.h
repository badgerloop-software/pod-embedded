#ifndef PODEMBEDDED_STATE_H
#define PODEMBEDDED_STATE_H

#include <ostream>
#include <string>

class State {
public:
    explicit State(State *previousState) {}
    virtual State* run(std::ostream *logStream) = 0;
    virtual char* getName() = 0;
};

#endif //PODEMBEDDED_STATE_H
