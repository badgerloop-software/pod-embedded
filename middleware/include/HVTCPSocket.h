#ifndef HVTCPSOCKET_H
#define HVTCPSOCKET_H

#define HV_TCP_PORT_RECV 9091

#ifndef MAX_COMMAND_SIZE
#define MAX_COMMAND_SIZE 1024
#endif

void SetupHVTCPServer();
void *TCPLoop(void *arg);
void signalLV(char *cmd);

#endif
