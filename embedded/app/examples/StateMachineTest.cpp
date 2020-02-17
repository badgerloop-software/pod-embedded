#include <iostream>
#include "../state-machine/State.h"
#include "../state-machine/StateMachine.h"
#include "ostream"
#include "../state-machine/IdleState.h"

int main() {
    StateMachine *machine = new StateMachine(new IdleState(nullptr));

    while(!machine -> isHalted())
        machine -> crank();
}