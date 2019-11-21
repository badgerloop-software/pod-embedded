// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <TelemetryLoop.h>
#include "LVTCPSocket.h"


/* ADD SENSOR INCLUDES HERE */
extern "C"
{
    #include "lv_iox.h"
    #include "nav.h"
    #include "braking.h"
    #include "connStat.h"
    #include "proc_iox.h"
	#include "imu.h"
    #include "retro.h"
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
	
    SetupTelemetry((char *) DASHBOARD_IP, DASHBOARD_PORT);
	SetupLVTCPServer();
    setDataState(1);
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
        
        if (getFlagsShouldBrake()) {
            brake();
            setFlagsShouldBrake(false);
        }
        if (getFlagsBrakePrimAct()) {
            brakePrimaryActuate();
            setFlagsBrakePrimAct(false);
        }
        if (getFlagsBrakePrimRetr()) {
            brakePrimaryUnactuate();
            setFlagsBrakePrimRetr(false);
        }
        if (getFlagsBrakeSecAct()) {
            brakeSecondaryActuate();
            setFlagsBrakeSecAct(false);
        }
        if (getFlagsBrakeSecRetr()) {
            brakeSecondaryUnactuate();
            setFlagsBrakeSecRetr(false);
        }

        if (getDataState() != 1 && (!checkTCPStatHV() || !checkTCPStat())) {
            printf("ERRS: %d\n", errs);
            errs += 1;
        } else { 
            errs = 0;
        }
        

        // Control loop
	}
    return 0;

}
