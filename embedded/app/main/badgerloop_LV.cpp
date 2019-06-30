// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "LVTelemetry_Loop.h"
#include "LVTCPSocket.h"
/* ADD SENSOR INCLUDES HERE */
extern "C"
{
    #include "lv_iox.h"
    #include "braking.h"
    #include "proc_iox.h"
	#include "imu.h"
    #include <data.h>
}

int init() {

    /* Init Data */
    initData();

	/* Init all peripherals */
	SetupIMU();
    initLVIox(true);
    initProcIox(true);

	/* Init telemetry services */
	SetupLVTelemetry((char *) "192.168.1.112", 33333);
	SetupLVTCPServer();

    return 0;
}

int main() {

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
