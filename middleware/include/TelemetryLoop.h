#ifndef TELEMETRYLOOP_H
#define TELEMETRYLOOP_H

void* TelemetryLoop(void* arg);
void SetupTelemetry(char* ip, int port);

typedef struct TelemArgs {
    char* ipaddr;
    int port;
} TelemArgs;

#endif
