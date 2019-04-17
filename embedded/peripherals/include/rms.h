#ifndef _RMS__H__
#define _RMS__H__

/* RMS Init function */
void rms_init();

/* Handles parsing of all recieved CAN messages */
int rms_parser(uint32_t id, uint8_t *data);

#endif 
