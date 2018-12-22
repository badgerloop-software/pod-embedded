#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/TelemetrySender.h"

int main() {
	SetupTelemetry();
	
	while(1){
		usleep(1000000);
	}
}