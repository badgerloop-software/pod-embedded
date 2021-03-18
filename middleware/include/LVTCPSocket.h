#ifndef LVTCPSOCKET_H
#define LVTCPSOCKET_H

#ifndef MAX_COMMAND_SIZE
#define MAX_COMMAND_SIZE 1024
#endif

void SetupLVTCPServer();
void *LVTCPLoop(void *arg);


#endif
