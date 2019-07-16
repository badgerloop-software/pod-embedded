// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "connStat.h"
#include "LVTelemetry_Loop.h"
#include "LVTCPSocket.h"
/* ADD SENSOR INCLUDES HERE */
extern "C"
{
    #include "lv_iox.h"
    #include "nav.h"
    #include "braking.h"
    #include "proc_iox.h"
	#include "imu.h"
    #include <data.h>
}

int init() {

    /* Init Data */
    initData();

    initProcIox(true);
    initLVIox(true);
	/* Init all peripherals */
	SetupIMU();
    initRetros();
    initPressureMonitor();
    initNav();
    /* Init telemetry services */
	SetupLVTelemetry((char *) DASHBOARD_IP, LV_TELEM_PORT);
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
