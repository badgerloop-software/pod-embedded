#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/TelemetrySender.h"

int main() {
	SetupLVTelemetry();
	
	while(1){
		usleep(1000000);
	}
}