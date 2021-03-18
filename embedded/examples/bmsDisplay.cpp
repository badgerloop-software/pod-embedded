#include <TelemetryLoop.h>
#include <connStat.h>
#include <hv_iox.h>
#include <pthread.h>
extern "C" {
#include "bms.h"
#include "can_devices.h"
#include "data.h"
#include "imu.h"
extern pthread_t CANThread;
}

HVIox hv_iox;
int main()
{
    initData();
    // initPressureSensors();
    SetupCANDevices();
    SetupTelemetry((char*)DASHBOARD_IP, DASHBOARD_PORT);
    pthread_join(CANThread, NULL);
}
