#ifndef PODEMBEDDED_STATES_H
#define PODEMBEDDED_STATES_H

#include <ostream>
#include <string>

class State {
public:
    explicit State(State *previousState) {}
    virtual ~State() = default;
    virtual State* run(std::ostream *logStream) = 0;
    virtual char* getName() = 0;
};

// Define a macro for declaring state classes
#define DECLARE_STATE(STATE_TYPE) \
class STATE_TYPE: public State { \
    int numberOfRuns; \
public: \
    explicit STATE_TYPE(State *previousState); \
    State *run(std::ostream *logStream) override; \
    char* getName() override; \
};

namespace States {
    DECLARE_STATE(IdleState);
    DECLARE_STATE(TestState);
}

#endif //PODEMBEDDED_STATES_H
