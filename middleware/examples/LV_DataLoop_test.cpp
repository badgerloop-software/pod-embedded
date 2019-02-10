#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "LVTelemetry_Loop.h"

int main() {
	SetupLVTelemetry((char*) "192.168.1.131", 33333);
	
	while(1){
		usleep(1000000);
	}
}