#ifndef HVTELEMETRY_SENDER_H
#define HVTELEMETRY_SENDER_H

typedef struct HVTelemArgs{
	char *ipaddr;
	int port;
} HVTelemArgs;

void *HVTelemetryLoop();
void SetupHVTelemetry(char* ip, int port);

#endif