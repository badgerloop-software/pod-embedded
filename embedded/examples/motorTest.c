#include <stdio.h>
#include <stdlib.h>
#include <can.h>
#include <motor.h>
#include <signal.h>
#include <data.h>

void quitHandler() {
    stopMotor();
}

int main(int argc, char *argv[]) {
    initData();
    initCan();
    initMotor();
    struct sigaction sig;
    sig.sa_sigaction = quitHandler;
    sigaction(SIGINT, &sig, NULL);
    sleep(1);

    startMotor();
    setTorque(argc);
    while(1);
}
