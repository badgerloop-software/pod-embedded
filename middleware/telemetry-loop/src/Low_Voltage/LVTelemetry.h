#ifndef LVTELEMETRY_SENDER_H
#define LVTELEMETRY_SENDER_H

void *LVTelemetryLoop(void *arg);
void SetupLVTelemetry(char* ip, int port);

typedef struct LVTelemArgs{
	char *ipaddr;
	int port;
} LVTelemArgs;

#endif