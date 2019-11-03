#ifndef PODEMBEDDED_TCPSERVER_H
#define PODEMBEDDED_TCPSERVER_H

#ifndef MAX_COMMAND_SIZE
#define MAX_COMMAND_SIZE 1024
#endif

#ifndef TCP_PORT
#define TCP_PORT 3001
#endif

#ifndef TIMEOUT_DURATION
#define TIMEOUT_DURATION 60*1000 // In milliseconds
#endif

#ifndef FUTURE_PAUSE
#define FUTURE_PAUSE 15 // In milliseconds
#endif


void SetupTCPServer();
void *TCPLoop(void *arg);
void *HandleConnection(void *arg);

#endif //PODEMBEDDED_TCPSERVER_HW
