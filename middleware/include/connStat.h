#ifndef __CONNSTAT_H__
#define __CONNSTAT_H__

#include <stdbool.h>

#define DASHBOARD_IP "192.168.0.15"
#define DASHBOARD_PORT 33333

#define LV_TELEM_PORT 33333
#define HV_TELEM_PORT 33333

#define LV_SERVER_IP "192.168.0.6"
#define HV_SERVER_IP "192.168.0.7"

#define LV_SERVER_PORT 9091
#define HV_SERVER_PORT 9094

#define HV_TELEM_RECV_PORT 9093
#define HV_TCP_PORT_RECV   9094

void *connStatTCPLoop(void *timestamp);
void *connStatUDPLoop(void *timestamp);

bool checkUDPStat(void);
bool checkTCPStat(void);

#endif
