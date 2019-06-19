// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#include "HVTelemetry_Loop.h"
#include "HVTCPSocket.h"
#include "HV_Telem_Recv.h"


extern "C"
{
    #include "can_devices.h"
    #include "data.h"
}

data_t *data;

int init() {
	// 1. init all drivers

    initData();
    SetupCANDevices();
    // Init initial timer
	data->timers->startTime = getuSTimestamp();

	data->state = 0;

    SetupHVTelemetry((char *) "192.168.1.126", 33333);
	SetupHVTCPServer();
	SetupHVTelemRecv();
    return 0;
}

int main() {
	/* Create the big data structures to house pod data */

	if (init() == 1) {
		printf("Error in initialization! Exiting...\r\n");
		exit(1);
	}

	while(1) {
        usleep(100000);
		// Control loop
	}
    return 0;
}
