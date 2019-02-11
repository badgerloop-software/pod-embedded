#ifndef HVTELEMETRY_SENDER_H
#define HVTELEMETRY_SENDER_H

void *HVTelemetryLoop(void *arg);
void SetupHVTelemetry(char* ip, int port);

typedef struct HVTelemArgs{
	char *ipaddr;
	int port;
} HVTelemArgs;

#endif