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
	SetupLVTelemetry((char *) DASHBOARD_IP, DASHBOARD_PORT);
	SetupLVTCPServer();

    return 0;
}

int main() {

	if (init() == 1) {
		printf("Error in initialization! Exiting...\r\n");
		exit(1);
	}
    flags_t *f = data->flags;
    while(1) {
		usleep(100000);
        if (data->flags->shouldBrake) {
            brake();
            data->flags->shouldBrake = false;
        }
        if (data->flags->brakePrimAct) {
            brakePrimaryActuate();
            data->flags->brakePrimAct = false;
        }
        if (data->flags->brakePrimRetr) {
            brakePrimaryUnactuate();
            data->flags->brakePrimRetr = false;
        }
        if (data->flags->brakeSecAct) {
            brakeSecondaryActuate();
            data->flags->brakeSecAct = false;
        }
        if (data->flags->brakeSecRetr) {
            brakeSecondaryUnactuate();
            data->flags->brakeSecRetr = false;
        }
		// Control loop
	}
    return 0;

}
