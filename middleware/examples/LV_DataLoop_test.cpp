#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "LVTelemetry_Loop.h"


/* ADD SENSOR INCLUDES HERE */
extern "C" 
{
	#include "imu.h"
}

int main() {
	SetupIMU();
	SetupLVTelemetry((char*) "192.168.1.126", 33333);
	
	while(1){
		usleep(1000000);
	}
}