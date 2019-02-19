// Includes
#include "badgerloop.h"
#include "state_machine.h"
#include "HVTelemetry_Loop.h"
#include "HVTCPSocket.h"
#include "data.h"

data_t *data;

int init() {
	// 1. init all drivers
	
	// 2. init all peripherals
	
	
	
	SetupHVTelemetry("192.168.1.112", 33333);
	SetupHVTCPServer();
	
	
}

int main() {
	/* Create the big data structures to house pod data */
	Bms bms; 
	Rms rms;
	
	if (init() == 1) {
		printf("Error in initialization! Exiting...\r\n");
		exit();
	}
	
	while(1) {
	
		// Control Loop!		

	}

}
