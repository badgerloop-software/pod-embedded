#ifndef _BMS_H_
#define _BMS_H_

int bmsClearFaults();
int bmsParseMsg(uint32_t id, uint8_t *data);
void bmsDump();

#endif
