#ifndef LVTCPSOCKET_H
#define LVTCPSOCKET_H

#define LV_TCP_PORT_RECV 9090
#define MAX_COMMAND_SIZE 1024

void SetupLVTCPServer(int port);
void *TCPLoop(void *arg);


#endif