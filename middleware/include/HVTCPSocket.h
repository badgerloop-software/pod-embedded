#ifndef HVTCPSOCKET_H
#define HVTCPSOCKET_H

#define HV_TCP_PORT_RECV 9091
#define MAX_COMMAND_SIZE 1024

void SetupHVTCPServer();
void *TCPLoop(void *arg);


#endif