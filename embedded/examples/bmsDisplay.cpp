#include <stdio.h>
#include <connStat.h>
#include <pthread.h>
#include <TelemetryLoop.h>
#include <hv_iox.h>
#include <can_devices.h>
#include "bms.h"
#include "rms.h"

extern "C" {
    #include "data.h"
    #include "NCD9830DBR2G.h"
    #include "imu.h"
    extern pthread_t CANThread;
}

HVIox hv_iox;
RMS rms;
int main() {
    initData();
    initPressureSensors();
    SetupCANDevices();
    SetupTelemetry((char *) DASHBOARD_IP, DASHBOARD_PORT);
    pthread_join(CANThread, NULL);
}
