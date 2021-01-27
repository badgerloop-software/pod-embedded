/*   */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include "rms.h"
#include "can.h"

extern "C" {
#include <data_dump.h>
#include "data.h"
#include "connStat.h"

}

static pthread_t motorThreads[3];
static bool motorIsEnabled = false;
static bool noTorqueMode = false;
void motorHbs(void);
void testLoop(void);

data_t *data;
RMS rms;
CAN can;

void motorHbs() {
	while (1) {
		if (motorIsEnabled) {
			rms.rmsInvEn();
		} else if (noTorqueMode) {
			rms.rmsInvEnNoTorque();
		} else {
			rms.rmsIdleHb();
		}

		usleep(10000);
	}
}

void quitHandler(int, siginfo_t*, void*) {
	printf("\n\n\n\n\nSHUTDOWN \n\n\n");
	motorIsEnabled = false;
	noTorqueMode = false;
	sleep(1);
	rms.rmsCmdNoTorque();
	sleep(4);
	rms.rmsDischarge();
	sleep(1);
	rms.rmsInvDis();
	exit(0);
}

void testLoop() {
	//	usleepm,(100);
	rms.rmsEnHeartbeat();
	//  usleep(1000);
	rms.rmsClrFaults();
	//	usleep(1000);
	rms.rmsInvDis();
	sleep(3);

	//    rms.rmsEepromMsg(143, 0, WR_EEPROM);
	motorIsEnabled = true;
	noTorqueMode = true;
	usleep(100);
	//	rms.rmsEepromMsg(143, 0, RD_EEPROM);
	usleep(100);
	while(1);
	//	while (getRmsVSMCode() != 5) {
	//		sleep(1);
	//printf("setRmsVSMCode() == %d\n", getRmsVSMCode());
	//	}
	/*
	   printf("\n\n\n\n\nWE ARE SPINNING \n\n\n");
	   sleep(15);
	   printf("\n\n\n\n\nSHUTDOWN \n\n\n");
	   motorisenabled = false;
	   sleep(30);
	   notorquemode = false;
	   sleep(1);
	   rms.rmscmdnotorque();
	   sleep(4);
	   rms.rmsdischarge();
	   sleep(1);
	   rms.rmsinvdis(); */
}

void spinMotor() {
	pthread_create(&motorThreads[0], NULL, (void* (*)(void*)) motorHbs, NULL);
	pthread_create(&motorThreads[1], NULL, (void* (*)(void*)) testLoop, NULL);
	//				pthread_create(&motorThreads[2], NULL, (void*) canRX, NULL);
	printf("thread spawn\n");
	//	pthread_join(motorThreads[2], NULL);
	//   pthread_join(motorThreads[0], NULL);
	pthread_join(motorThreads[1], NULL);

}

int main() {
	initData();
	printf("initData\n");
	//	SetupDataDump();
	can.initCan();
    printf("canINit\n");
/*	SetupCANDevices();*/
    //	rms.rmsConfig();
	struct sigaction sig;
	sig.sa_sigaction = quitHandler;
	printf("SIGACTION %d\n",sigaction(SIGINT, &sig, NULL));
	spinMotor();
	//testLoop();
	return 0;
}

