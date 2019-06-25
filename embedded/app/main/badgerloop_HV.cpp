// Includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "HVTelemetry_Loop.h"
#include "HVTCPSocket.h"
#include "HV_Telem_Recv.h"
#include "data_dump.h"
#include "signal.h"

extern "C" 
{
    #include "motor.h"
    #include "hv_iox.h"
    #include "motor.h"
    #include "proc_iox.h"
    #include "data.h"
    #include "can_devices.h"
    #include "state_machine.h"
}

int init() {
    /* Init Data struct */
    initData();

    /* Init all drivers */
    SetupCANDevices();
    initProcIox(true);
    initHVIox(true);
    initMotor();   
    
    /* Allocate needed memory for state machine and create graph */
	buildStateMachine();

    /* Init telemetry */
    SetupHVTelemetry((char *) "192.168.1.126", 33333);
	SetupHVTCPServer();
	SetupHVTelemRecv();	
	
    /* Start 'black box' data saving */
    SetupDataDump();
	
    return 0;	
}

void shutdown() {
    setTorque(0);   /* Try our best to de-escalate while not overstaying our welcome */
    data->flags->shutdown = true;
    return;
}

int main() {
	/* Create the big data structures to house pod data */
	
	if (init() == 1) {
		fprintf(stderr, "Error in initialization! Exiting...\r\n");
		exit(1);
	}
    signal(SIGINT, shutdown);
    printf("Here\n");

	while(1) {
	    if (data->flags->shutdown) {
            if (getMotorIsOn())
                stopMotor();
            exit(0);
	    }
	    runStateMachine();
        usleep(10000);

		// Control loop
	}
    return 0;
}
