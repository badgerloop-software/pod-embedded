#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>
#include <unistd.h>

#include "data.h"
#include "HVTCPSocket.h"
#include "state_machine.h"


pthread_t HVTCPThread;

extern data_t *data;
extern stateMachine_t stateMachine;

/* Setup PThread Loop */
void SetupHVTCPServer(){
	
	if (pthread_create(&HVTCPThread, NULL, TCPLoop, NULL)){
		fprintf(stderr, "Error creating HV Telemetry thread\n");
	}
	 
}


/* Thread Loop */
void *TCPLoop(void *arg){
	
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
	address.sin_port = htons(HV_TCP_PORT_RECV); 
		
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
		
		printf("RECEIVED: %s\n",buffer);  
		
		// Do things
		if(!strncmp(buffer, "readyPump", MAX_COMMAND_SIZE)){
			data->flags->readyPump = 1;
		}
		
		if(!strncmp(buffer, "pumpDown", MAX_COMMAND_SIZE)){
			data->flags->readyPump = 1;
		}
		
		if(!strncmp(buffer, "readyCommand", MAX_COMMAND_SIZE)){
			data->flags->readyCommand = 1;
		}
		
		if(!strncmp(buffer, "propulse", MAX_COMMAND_SIZE)){
			data->flags->propulse = 1;
		}
		
		if(!strncmp(buffer, "emergencyBrake", MAX_COMMAND_SIZE)){
			data->flags->emergencyBrake = 1;
		}

		if(!strncmp(buffer, "primBrakeOff", MAX_COMMAND_SIZE)){
			// TODO Turn primary brake off
		}
		if(!strncmp(buffer, "primBrakeOn", MAX_COMMAND_SIZE)){
			// TODO Turn primary brake on
		}
		if(!strncmp(buffer, "secBrakeOff", MAX_COMMAND_SIZE)){
			// TODO Turn sec brake off
		}
		if(!strncmp(buffer, "secBrakeOn", MAX_COMMAND_SIZE)){
			// TODO turn sec brake on
		}

		if(!strncmp(buffer, "hvEnable", MAX_COMMAND_SIZE)){
			// TODO turn hv on
		}

		if(!strncmp(buffer, "hvDisable", MAX_COMMAND_SIZE)){
			// TODO turn hv off
		}

		if(!strncmp(buffer,"override", 8)){
			fprintf(stderr, "Override received for state: %s\n", buffer+9);
		    sprintf(stateMachine.overrideStateName, "%s\0", buffer+9);
        }
		
		// HEARTBEAT
		if(!strncmp(buffer, "ping", MAX_COMMAND_SIZE)){
			// Send acknowledge packet back
			send(new_socket, (char*) "pong1" , strlen("pong1") , 0 ); 
		}

	}
	
}
