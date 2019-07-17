#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <netdb.h>
#include "HVTCPSocket.h"
#include "connStat.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#define SA struct sockaddr

extern "C"
{
#include "data.h"
#include "state_machine.h"
#include "hv_iox.h"
#include "braking.h"
}


pthread_t HVTCPThread, connStatThread;

static uint64_t *lastPacket;

extern void *connStatLoop(void *pTimestamp);
extern stateMachine_t stateMachine;

/* Setup PThread Loop */
void SetupHVTCPServer(){
    
    lastPacket = (uint64_t *) malloc(sizeof(uint64_t));

    if (pthread_create(&connStatThread, NULL, connStatLoop, lastPacket)) {
        fprintf(stderr, "Error creating connection watcher\n");
    }

	  if (pthread_create(&HVTCPThread, NULL, TCPLoop, NULL)){
		  fprintf(stderr, "Error creating HV Telemetry thread\n");
	  }
}

/* Thread Loop */
void *TCPLoop(void *arg)
{

	(void)arg;

	int server_fd, new_socket;
	int opt = 1;

	struct sockaddr_in address;
	int addrlen = sizeof(address);

	// Create Server fd
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		fprintf(stderr, "Error creating socket FD\n");
		exit(EXIT_FAILURE);
	}

	// Attach Socket
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
								 &opt, sizeof(opt)))
	{
		fprintf(stderr, "Error attaching socket\n");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(HV_TCP_PORT_RECV);

	// Bind Socket
	if (bind(server_fd, (struct sockaddr *)&address,
					 sizeof(address)) < 0)
	{
		fprintf(stderr, "Error binding port\n");
		exit(EXIT_FAILURE);
	}

	// Listen on the server
	if (listen(server_fd, 3) < 0)
	{
		fprintf(stderr, "Error listening\n");
		exit(EXIT_FAILURE);
	}

	// Loop
	while (1)
	{

		// Character Buffer
		char buffer[1024] = {0};

		// Read incoming message
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
														 (socklen_t *)&addrlen)) < 0)
		{
			fprintf(stderr, "Error accepting a packet\n");
			exit(EXIT_FAILURE);
		}
    
    *lastPacket = getuSTimestamp();

    read(new_socket, buffer, 1024); 
		
		printf("RECEIVED: %s\n",buffer);  
		
		// Do things
		if (!strncmp(buffer, "readyPump", MAX_COMMAND_SIZE))
		{
			data->flags->readyPump = 1;
		}

		if (!strncmp(buffer, "pumpDown", MAX_COMMAND_SIZE))
		{
			data->flags->readyPump = 1;
		}

		if (!strncmp(buffer, "readyCommand", MAX_COMMAND_SIZE))
		{
			data->flags->readyCommand = 1;
		}

		if (!strncmp(buffer, "propulse", MAX_COMMAND_SIZE))
		{
			data->flags->propulse = 1;
		}

		if (!strncmp(buffer, "emergencyBrake", MAX_COMMAND_SIZE))
		{
            data->flags->emergencyBrake = 1;
		}

		if (!strncmp(buffer, "hvEnable", MAX_COMMAND_SIZE))
		{
            /* Lets add a safety check here */
			setMCUHVEnabled(true);
		}

		if (!strncmp(buffer, "hvDisable", MAX_COMMAND_SIZE))
		{
			setMCUHVEnabled(false);
		}

		if(!strncmp(buffer,"override", 8)){
			fprintf(stderr, "Override received for state: %s\n", buffer+9);
		  sprintf(stateMachine.overrideStateName, "%s\0", buffer+9);
        }
		
		// HEARTBEAT
		if (!strncmp(buffer, "ping", MAX_COMMAND_SIZE))
		{
			// Send acknowledge packet back
			send(new_socket, (char *)"pong1", strlen("pong1"), 0);
		}

		close(new_socket);
	}
}

void signalLV(char *cmd)
{
	int srvFd;

	int opt = 1;

	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
    
	if ((srvFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		fprintf(stderr, "Error signalling\n");
		exit(1);
	}

	if (setsockopt(srvFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
								 &opt, sizeof(opt)))
	{
		fprintf(stderr, "Signal error\n");
		exit(1);
	}
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(LV_SERVER_IP);
    addr.sin_port = htons(LV_SERVER_PORT);
    
    if (connect(srvFd, (SA*)&addr,addrlen) != 0) {
        fprintf(stderr, "Failed to open port\n");
        return;
    }

	write(srvFd, cmd, strlen(cmd));

	close(srvFd);
}
