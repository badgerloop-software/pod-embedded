#ifndef __DATA_H__
#define __DATA_H__

#include <stdint.h>
#include <time.h>
#include <retro.h>
#include <stdbool.h>
#define TIME_SINCE_LAST_RETRO 15000000
#define FILTER_NONE     0
#define FILTER_ROLLING  1
#define FILTER_EXP      2

/* Filters */
float rollingAvgFloat(float *vals, int windowSize);
int rollingAvgInt(int *vals, int windowSize);
float expFilterFloat(float currVal, float prevVal, float weight);
int expFilterInt(int currVal, int prevVal, float weight);

int initMetaData(void);
int isEarlyInit(void);

/**!!AUTO-GENERATE HERE!!**/

extern data_t *data;
#endif