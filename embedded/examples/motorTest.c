#include <stdio.h>
#include <stdlib.h>
#include <can.h>
#include <motor.h>
#include <signal.h>
#include <data.h>
#include <string.h>
#include <hv_iox.h>

void quitHandler() {
    setTorque(0);
    stopMotor();
    exit(0);
}

int main(int argc, char *argv[]) {
    bool isHard = false;
	if (argc < 2) {
        printf("Enter a torque pls\n");
        return 0;
    }
    int torque = atoi(argv[1]);
    printf("torque= %d\n", torque);
    if (torque > 5 || torque < 0) {
        printf("invalid torque\n");
        return 0;
    }
    
    initData();
    initCan();
	if (argc > 1) {
		if (strcmp(argv[1], "-h") == 0) {
			isHard = true;
		}

	}

	
	initHVIox(isHard);
	if (isHard) return;
    initMotor();
	if (argc > 1) {
		if (strcmp(argv[1], "mcuoff") == 0) {
			setMCUHVEnabled(false);
		} else if (strcmp(argv[1], "mcuon") == 0) {
			printf("enable\n");
			setMCUHVEnabled(true);
		} else if (strcmp(argv[1], "latchoff") == 0) {
			printf("latchoff\n");
			setMCULatch(false);
		} else if (strcmp(argv[1], "latchon") == 0) {
			printf("latchon\n");
			setMCULatch(true);
		}
		return;
	}
/*	printf("HV EN\n");*/
/*	setMCULatch(true);*/
/*	printf("SET LATCH\n");*/
/*	usleep(2000000);*/
/*	setMCULatch(false);*/
/*	printf("CLR LATCH\n");*/
/*   return; */
    struct sigaction sig;
    sig.sa_sigaction = quitHandler;
    sigaction(SIGINT, &sig, NULL);
/*    sleep(1);*/
    
    startMotor();
    setTorque(torque);
    while(1) usleep(10000);
}
