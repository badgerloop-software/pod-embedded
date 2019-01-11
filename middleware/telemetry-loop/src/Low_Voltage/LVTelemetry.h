#ifndef LVTELEMETRY_SENDER_H
#define LVTELEMETRY_SENDER_H

#define IPADDR "127.0.0.1"
#define PORT 33333

void *LVTelemetryLoop(void *arg);
void SetupLVTelemetry(char* ip, int port);

typedef struct Munch2Args{
	char *ipaddr;
	int port;
} LVTelemArgs;

#endif