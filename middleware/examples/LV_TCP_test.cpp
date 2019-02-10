#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "LVTCPSocket.h"

int main() {
	SetupLVTCPServer();
	
	while(1){
		usleep(1000000);
	}
}