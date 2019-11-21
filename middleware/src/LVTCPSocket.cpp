#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>
#include <unistd.h>

#include "LVTCPSocket.h"
#include "data.h"

#define DASH 0
#define HV 1

extern  "C" {
    extern void resetNav();
#include "connStat.h"
#include <braking.h>
}

pthread_t LVTCPThread, lvTcpConT, lvTcpConT2;
uint64_t *lastPacket[2];

/* Setup PThread Loop */
void SetupLVTCPServer(){
    lastPacket[DASH] = (uint64_t *) malloc(sizeof(uint64_t));
    lastPacket[HV] = (uint64_t *) malloc(sizeof(uint64_t));
	    if (pthread_create(&lvTcpConT, NULL, connStatTCPLoop, lastPacket[DASH])) {
        fprintf(stderr, "Error creating connection watcher\n");
    }
    if (pthread_create(&lvTcpConT2, NULL, connStatTCPLoopHV, lastPacket[HV])) {
        fprintf(stderr, "Error creating connection watcher\n");
    }
	if (pthread_create(&LVTCPThread, NULL, LVTCPLoop, NULL)){
		fprintf(stderr, "Error creating LV Telemetry thread\n");
	}
	 
}

/* Thread Loop */
void *LVTCPLoop(void *arg){
	
	(void) arg;

	int server_fd, new_socket;
	int opt = 1;
		
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	
	// Create Server fd	
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
		fprintf(stderr, "Error creating socket FD\n");
		exit(EXIT_FAILURE); 
	} 
		
	// Attach Socket
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												  &opt, sizeof(opt))){ 
		fprintf(stderr, "Error attaching socket\n");
		exit(EXIT_FAILURE); 
	}
		
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(LV_SERVER_PORT); 

	// Bind Socket
	if (bind(server_fd, (struct sockaddr *)&address,
									 sizeof(address))<0) {
		fprintf(stderr, "Error binding port\n");
		exit(EXIT_FAILURE); 
	}

	// Listen on the server
	if (listen(server_fd, 3) < 0){ 
		fprintf(stderr, "Error listening\n");
		exit(EXIT_FAILURE); 
	}
	
	// Loop
	while(1){
		
		// Character Buffer
		char buffer[1024] = {0}; 
		
		// Read incoming message
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
								   (socklen_t*)&addrlen))<0){ 
			fprintf(stderr, "Error accepting a packet\n"); 
			exit(EXIT_FAILURE); 
		}
		read(new_socket, buffer, 1024); 
		
	//	printf("RECEIVED: %s\n",buffer);  
		
		// Do things
		if(!strncmp(buffer, "power off", MAX_COMMAND_SIZE)){
			// DO POWER OFF
		}
        if (!strncmp(buffer, "state", 5)) {
            *lastPacket[HV] = getuSTimestamp();
            setDataState(buffer[5] - 0x30);
        }
	    
        if (!strncmp(buffer, "clrMotion", MAX_COMMAND_SIZE)) {
            resetNav();
            setFlagsReadyToBrake(true);
        }

        if (!strncmp(buffer, "brake", MAX_COMMAND_SIZE)) {
/*            setFlagsShouldBrake(true);*/
            brake();
        }

		if (!strncmp(buffer, "primBrakeOff", MAX_COMMAND_SIZE))
		{
/*			setFlagsBrakePrimRetr(true);*/
                brakePrimaryUnactuate(); 
		}
		if (!strncmp(buffer, "primBrakeOn", MAX_COMMAND_SIZE))
		{
/*            setFlagsBrakePrimAct(true); */
	    brakePrimaryActuate();
		}
		if (!strncmp(buffer, "secBrakeOff", MAX_COMMAND_SIZE))
		{
/*            setFlagsBrakeSecRetr(true);*/
		brakeSecondaryUnactuate();
		}
		if (!strncmp(buffer, "secBrakeOn", MAX_COMMAND_SIZE))
		{
/*            setFlagsBrakeSecAct(true)*/
		brakeSecondaryActuate();
		}

		// HEARTBEAT
		if(!strncmp(buffer, "ping", MAX_COMMAND_SIZE)){
			// Send acknowledge packet back
			*lastPacket[DASH] = getuSTimestamp();
            send(new_socket, (char*) "pong2" , strlen("pong2") , 0 ); 
		}
		
		close(new_socket);
	}
}


