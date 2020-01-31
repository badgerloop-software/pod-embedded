/*   */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include "connStat.h"
#include "DataDump.h"
#include "data.h"
#include "rms.h"
#include "can.h"

static pthread_t motorThreads[3];
static bool motorIsEnabled = false;
static bool noTorqueMode = false;
void motorHbs(void);
void testLoop(void);

data_t *data;

void motorHbs() {
	while (1) {
		if (motorIsEnabled) {
			rmsInvEn();
		} else if (noTorqueMode) {
			rmsInvEnNoTorque();
		} else {
			rmsIdleHb();
		}

		usleep(10000);
	}
}

void quitHandler() {
	printf("\n\n\n\n\nSHUTDOWN \n\n\n");
	motorIsEnabled = false;
	noTorqueMode = false;
	sleep(1);
	rmsCmdNoTorque();
	sleep(4);
	rmsDischarge();
	sleep(1);
	rmsInvDis();
	exit(0);
}

void testLoop() {
	//	usleepm,(100);
	rmsEnHeartbeat();
	//  usleep(1000);
	rmsClrFaults();
	//	usleep(1000);
	rmsInvDis();
	sleep(3);

	//    rmsEepromMsg(143, 0, WR_EEPROM);
	motorIsEnabled = true;
	noTorqueMode = true;
	usleep(100);
	//	rmsEepromMsg(143, 0, RD_EEPROM);
	usleep(100);
	while(1);
	//	while (data->rms->VSMCode != 5) {
	//		sleep(1);
	//printf("data->rms->VSMCode == %d\n", data->rms->VSMCode);
	//	}
	/*
	   printf("\n\n\n\n\nWE ARE SPINNING \n\n\n");
	   sleep(15);
	   printf("\n\n\n\n\nSHUTDOWN \n\n\n");
	   motorisenabled = false;
	   sleep(30);
	   notorquemode = false;
	   sleep(1);
	   rmscmdnotorque();
	   sleep(4);
	   rmsdischarge();
	   sleep(1);
	   rmsinvdis(); */
}

void spinMotor() {
	pthread_create(&motorThreads[0], NULL, (void*) motorHbs, NULL);
	pthread_create(&motorThreads[1], NULL, (void*) testLoop, NULL);
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
	initCan();
    printf("canINit\n");
/*	SetupCANDevices();*/
    //	rmsConfig();
	struct sigaction sig;
	sig.sa_sigaction = quitHandler;
	printf("SIGACTION %d\n",sigaction(SIGINT, &sig, NULL));
	spinMotor();
	//testLoop();
	return 0;
}

