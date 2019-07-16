#ifndef __CONNSTAT_H__
#define __CONNSTAT_H__

#define DASHBOARD_IP "192.168.1.109"
#define DASHBOARD_PORT 33333

#define LV_TELEM_PORT 33333
#define HV_TELEM_PORT 33334

#define LV_SERVER_IP "192.168.1.126"
#define HV_SERVER_IP "192.168.1.147"

#define LV_SERVER_PORT 9091
#define HV_SERVER_PORT 9094

#define HV_TELEM_RECV_PORT 9093
#define HV_TCP_PORT_RECV   9094

void *connStatLoop(void *timestamp);

#endif
