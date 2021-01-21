#include <stdio.h>
#include <connStat.h>
#include <pthread.h>
#include <TelemetryLoop.h>
#include <hv_iox.h>
#include <can_devices.h>
extern "C" {
    #include "data.h"
                #include "NCD9830DBR2G.h"
    #include "imu.h"
    #include "bms.h"
    extern pthread_t CANThread;
}

HVIox hv_iox;
int main() {
    initData();
    initPressureSensors();
    SetupCANDevices();
    SetupTelemetry((char *) DASHBOARD_IP, DASHBOARD_PORT);
    pthread_join(CANThread, NULL);
}
