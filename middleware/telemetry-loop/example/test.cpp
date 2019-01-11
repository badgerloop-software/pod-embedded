#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/Low_Voltage/LVTelemetry.h"

int main() {
	SetupLVTelemetry("192.168.1.131", 33333);
	
	while(1){
		usleep(1000000);
	}
}