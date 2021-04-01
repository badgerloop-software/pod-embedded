#include "HVTCPSocket.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define SA struct sockaddr

extern "C" {
#include "braking.h"
#include "connStat.h"
#include "data.h"
#include "hv_iox.h"
#include "motor.h"
#include "rms.h"
#include "state_machine.h"
}

pthread_t HVTCPThread, connStatThread;

static uint64_t* lastPacket;

bool motorIsEnabled, noTorqueMode;
pthread_t hbT;
extern stateMachine_t stateMachine;
void* hbLoop(void* nul)
{

    (void)nul;

    while (1) {
        if (motorIsEnabled) {
            rmsInvEn();
        } else if (noTorqueMode) {
            rmsInvEnNoTorque();
        } else {
            rmsIdleHb();
        }

        usleep(10000);
    }
}
/* Setup PThread Loop */
void SetupHVTCPServer()
{

    lastPacket = (uint64_t*)malloc(sizeof(uint64_t));

    if (pthread_create(&connStatThread, NULL, connStatTCPLoop, lastPacket)) {
        fprintf(stderr, "Error creating connection watcher\n");
    }

    if (pthread_create(&HVTCPThread, NULL, TCPLoop, NULL)) {
        fprintf(stderr, "Error creating HV Telemetry thread\n");
    }
}

/* Thread Loop */
void* TCPLoop(void* arg)
{

    (void)arg;
    motorIsEnabled = false;
    noTorqueMode = false;
    int server_fd, new_socket;
    int opt = 1;

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create Server fd
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        fprintf(stderr, "Error creating socket FD\n");
        exit(EXIT_FAILURE);
    }

    // Attach Socket
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
            &opt, sizeof(opt))) {
        fprintf(stderr, "Error attaching socket\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(HV_TCP_PORT_RECV);

    // Bind Socket
    if (bind(server_fd, (struct sockaddr*)&address,
            sizeof(address))
        < 0) {
        fprintf(stderr, "Error binding port\n");
        exit(EXIT_FAILURE);
    }

    // Listen on the server
    if (listen(server_fd, 3) < 0) {
        fprintf(stderr, "Error listening\n");
        exit(EXIT_FAILURE);
    }

    // Loop
    while (1) {

        // Character Buffer
        char buffer[1024] = { 0 };

        // Read incoming message
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address,
                 (socklen_t*)&addrlen))
            < 0) {
            fprintf(stderr, "Error accepting a packet\n");
            exit(EXIT_FAILURE);
        }

        *lastPacket = getuSTimestamp();

        read(new_socket, buffer, 1024);

        printf("RECEIVED: %s\n", buffer);

        // Do things
        if (!strncmp(buffer, "readyPump", MAX_COMMAND_SIZE)) {
            setFlagsReadyPump(1);
        }

        if (!strncmp(buffer, "pumpDown", MAX_COMMAND_SIZE)) {
            setFlagsReadyPump(1);
        }

        if (!strncmp(buffer, "readyCommand", MAX_COMMAND_SIZE)) {
            setFlagsReadyCommand(1);
        }

        if (!strncmp(buffer, "propulse", MAX_COMMAND_SIZE)) {
            setFlagsPropulse(1);
        }

        if (!strncmp(buffer, "emergencyBrake", MAX_COMMAND_SIZE)) {
            setFlagsEmergencyBrake(1);
        }
        if (!strncmp(buffer, "mcuLatchOn", MAX_COMMAND_SIZE)) {
            setMCULatch(true);
        }
        if (!strncmp(buffer, "mcuLatchOff", MAX_COMMAND_SIZE)) {
            setMCULatch(false);
        }
        if (!strncmp(buffer, "enPrecharge", MAX_COMMAND_SIZE)) {
            /*            pthread_create(&hbT, NULL, hbLoop, NULL);*/
            rmsEnHeartbeat();
            rmsClrFaults();
            rmsInvDis();
            /*            noTorqueMode = true;*/
        }

        if (!strncmp(buffer, "cmdTorque", MAX_COMMAND_SIZE)) {
            setMotorEn();
        }

        if (!strncmp(buffer, "hvEnable", MAX_COMMAND_SIZE)) {
            /* Lets add a safety check here */
            setMCUHVEnabled(true);
        }

        if (!strncmp(buffer, "hvDisable", MAX_COMMAND_SIZE)) {
            setMCUHVEnabled(false);
        }

        if (!strncmp(buffer, "override", 8)) {
            fprintf(stderr, "Override received for state: %s\n", buffer + 9);
            sprintf(stateMachine.overrideStateName, "%s%c", buffer + 9, '\0');
        }

        // HEARTBEAT
        if (!strncmp(buffer, "ping", MAX_COMMAND_SIZE)) {
            printf("ping\n");
            // Send acknowledge packet back
            send(new_socket, (char*)"pong1", strlen("pong1"), 0);
        }

        close(new_socket);
    }
}

void signalLV(char* cmd)
{
	fprintf(stderr, "THERE IS NOTHING TO SIGNAL TO DO NOT SIGNAL LV\n");
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

    if (connect(srvFd, (SA*)&addr, addrlen) != 0) {
        //   fprintf(stderr, "Failed to open port\n");
        close(srvFd);
        return;
    }

    write(srvFd, cmd, strlen(cmd));

    close(srvFd);
}
