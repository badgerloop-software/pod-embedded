#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/Low_Voltage/LVTCPSocket.h"

int main() {
	SetupLVTCPServer();
	
	while(1){
		usleep(1000000);
	}
}