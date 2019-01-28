#ifndef HVTCPSOCKET_H
#define HVTCPSOCKET_H

#define HV_TCP_PORT_RECV 9090
#define MAX_COMMAND_SIZE 1024

void SetupHVTCPServer(int port);
void *TCPLoop(void *arg);


#endif