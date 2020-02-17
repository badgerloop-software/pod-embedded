#include <iostream>
#include "../state-machine/States.h"
#include "../state-machine/StateMachine.h"
#include "ostream"

int main() {
    StateMachine *machine = new StateMachine(new States::IdleState(nullptr));

    while(!machine -> isHalted())
        machine -> crank();
}