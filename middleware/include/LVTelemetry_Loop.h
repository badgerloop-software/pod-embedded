#ifndef LVTELEMETRY_SENDER_H
#define LVTELEMETRY_SENDER_H

void *LVTelemetryLoop(void *arg);
void SetupLVTelemetry(char* ip, int port);

typedef struct LVTelemArgs{
	char *ipaddr;
	int port;
} LVTelemArgs;

#ifndef HV_SERVER_IP 
#define HV_SERVER_IP "192.168.1.140"
#endif

#ifndef HV_SERVER_PORT
#define HV_SERVER_PORT 9099
#endif

#endif