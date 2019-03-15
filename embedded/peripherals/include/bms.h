#ifndef _BMS_H_
#define _BMS_H_

#include <stdint.h>

void bmsInit();
int bmsClearFaults();
int bmsParseMsg(uint32_t id, uint8_t *data);
void bmsDump();
#endif
