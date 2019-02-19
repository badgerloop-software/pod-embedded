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
	
	if (data = malloc(sizeof(data_t))          				  == NULL) { STATE_ERROR() }
	if (data->pressure = malloc(sizeof(pressure_t))           == NULL) { STATE_ERROR() }
	if (data->motion = malloc(sizeof(motion_t))           	  == NULL) { STATE_ERROR() }
	if (data->bms = malloc(sizeof(bms_t))           		  == NULL) { STATE_ERROR() }
	if (data->rms = malloc(sizeof(rms_t))           		  == NULL) { STATE_ERROR() }
	if (data->flags = malloc(sizeof(flags_t))           	  == NULL) { STATE_ERROR() }
	
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
