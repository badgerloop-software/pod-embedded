#ifndef NETWORKING_OVERHAUL_TELEMETRYLOOP_H
#define NETWORKING_OVERHAUL_TELEMETRYLOOP_H

void *TelemetryLoop(void *arg);
void SetupTelemetry(char* ip, int port);

typedef struct TelemArgs{
    char *ipaddr;
    int port;
} TelemArgs;


#endif
