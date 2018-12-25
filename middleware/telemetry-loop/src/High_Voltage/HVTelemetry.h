#ifndef HVTELEMETRY_SENDER_H
#define HVTELEMETRY_SENDER_H

#define IPADDR "127.0.0.1"
#define PORT 33333

void *HVTelemetryLoop();
void SetupHVTelemetry();

#endif