#include <stdio.h>
#include <stdlib.h>
#include <can.h>
#include <motor.h>
#include <signal.h>
#include <data.h>

void quitHandler() {
    setTorque(0);
    stopMotor();
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Enter a torque pls\n");
        return 0;
    }
    int torque = atoi(argv[1]);
    
    if (torque > 5 || torque < 0) {
        printf("invalid torque\n");
        return 0;
    }
    
    initData();
    initCan();
    initMotor();
    
    struct sigaction sig;
    sig.sa_sigaction = quitHandler;
    sigaction(SIGINT, &sig, NULL);
    sleep(1);
    
    startMotor();
    setTorque(torque);
    while(1) usleep(10000);
}
