#ifndef HV_TELEM_RECV_H
#define HV_TELEM_RECV_H

#ifndef MAX_TLM_HV_RECV
#define MAX_TLM_HV_RECV 4096
#endif

#ifndef TLM_RECV_PORT
#define TLM_RECV_PORT 9099
#endif

void SetupHVTelemRecv();
void *HVTelemRecv(void *arg);

#endif