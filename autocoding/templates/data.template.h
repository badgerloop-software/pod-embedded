#ifndef __DATA_H__
#define __DATA_H__

#include <stdint.h>
#include <time.h>
#include <semaphore.h>
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

static inline uint64_t convertTouS(struct timespec *currTime) {
    return (uint64_t)((currTime->tv_sec * 1000000) + (currTime->tv_nsec / 1000));
}

static inline uint64_t getuSTimestamp() {
    struct timespec _temp;
    clock_gettime(CLOCK_MONOTONIC, &_temp);
    uint64_t _tempTs = convertTouS(&_temp);
    return _tempTs;
}

static inline uint64_t getSTimestamp() {
    struct timespec temp;
    clock_gettime(CLOCK_MONOTONIC, &temp);
    return (uint64_t) (temp.tv_sec);
}

extern data_t *data;
#endif