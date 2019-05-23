#ifndef __RETRO_H__
#define __RETRO_H__

#define NUM_RETROS		1		// TODO change to 3 when the rest are plugged in

#define RETRO_DELAY		2
#define WIDTH_TAPE_STRIP 4 /* 4 inches */
#define TOTAL_STRIPS	41

#define RETRO_1_PIN 	44 //66
#define RETRO_2_PIN		69
#define RETRO_3_PIN		68

/* We map them, so they are indexed in a somewhat funky way */
#define RETRO_1			0
#define RETRO_2			1
#define RETRO_3			2

int initRetros(void);
int joinRetroThreads(void);

#endif
