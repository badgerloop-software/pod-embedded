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
    #include "nav.h"
    #include "braking.h"
    extern void csvFormatShow();    
#include "connStat.h"
    #include "proc_iox.h"
	#include "imu.h"
    #include <data.h>
}

int init() {

    /* Init Data */
    initData();

    initPressureMonitor();
    initProcIox(true);
    initLVIox(true);
	
    /* Init all peripherals */
	SetupIMU();
    initRetros();
    initNav();
    
    /* Init telemetry services */
	
    SetupLVTelemetry((char *) DASHBOARD_IP, DASHBOARD_PORT);
	SetupLVTCPServer();
    data->state = 1;
    return 0;
}

int main() {

	if (init() == 1) {
		printf("Error in initialization! Exiting...\r\n");
		exit(1);
	}
    int errs = 0;
    while(1) {
		usleep(100000);
        if (errs > 50) brake();
        
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
		
        csvFormatShow();
        if (data->state != 1 && (!checkTCPStatHV() || !checkTCPStat())) {
            printf("ERRS: %d\n", errs);
            errs += 1;
        } else { 
            errs = 0;
        }
        

        // Control loop
	}
    return 0;

}
