#ifndef TELEMETRY_SENDER_H
#define TELEMETRY_SENDER_H

#define IPADDR "127.0.0.1"
#define PORT 33333

void *TelemetryLoop();
void SetupTelemetry();

#endif