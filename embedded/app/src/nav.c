#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <data.h>
#include <imu.h>
#include <nav.h>
#include <connStat.h>

#define FEET_TO_METERS(x) ((x) * 0.3048)
#define USEC_TO_SEC(x)    ((x) / 1000000)
#define STRIP_DISTANCE FEET_TO_METERS(10) /*m*/
#define TOTAL_DISTANCE FEET_TO_METERS(100) /*m, not real TODO change*/
#define TOTAL_RUN_TIME 30   /* s */
#define EXPECTED_DECEL 9.8  /* m/s/s */

#define WINDOW_SIZE    2

static pthread_t navThread;
static pthread_mutex_t lock;

static void navLoop(void *arg);
static void insertRing(rawMotionNode_t *node, float val);

static rawMotion_t rawData;

void initNav() {

    rawData.pos.size    = WINDOW_SIZE;
    rawData.pos.head    = 0;
    rawData.pos.data    = (float *) malloc(sizeof(float) * rawData.pos.size);
    if (rawData.pos.data == NULL) fprintf(stderr, "Nav data malloc fail\n");

    rawData.vel.size    = WINDOW_SIZE;
    rawData.vel.head    = 0;
    rawData.vel.data    = (float *) malloc(sizeof(float) * rawData.vel.size);
    if (rawData.vel.data == NULL) fprintf(stderr, "Nav data malloc fail\n");

    rawData.accel.size  = WINDOW_SIZE;
    rawData.accel.head  = 0;
    rawData.accel.data  = (float *) malloc(sizeof(float) * rawData.accel.size);
    if (rawData.accel.data == NULL) fprintf(stderr, "Nav data malloc fail\n");


    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Error creating nav mutex\n");
    }

    if (pthread_create(&navThread, NULL, (void *)(navLoop), NULL) != 0) {
        fprintf(stderr, "Error creating nav thread\n");
    }
}

void showNavData() {
    printf("Pos: %0.5f ; Vel: %0.5f ; Accel%0.5f\n", 
            getMotionPos(), getMotionVel(), getMotionAccel());
}

void csvFormatHeader() {
    printf("accelx,accely,accelz,pos,vel,accel\n");   
}

void csvFormatShow() {
    printf("%0.5f,%0.5f,%0.5f,%0.5f,%0.5f,%0.5f\n", 
           getAccelX(), getAccelY(), getAccelZ(),
           getMotionPos(), getMotionVel(), getMotionAccel());
}

/* Returns m/s/s */
static inline float accelFromRetro(float currVel) {
   return (currVel - getMotionVel()) /
       USEC_TO_SEC((float)getTimersLastRetro() - (float)getTimersOldRetro()); 
}

/* Returns m/s */
static inline float velFromRetro(float currPos) {
   return (currPos - getMotionPos()) /
       USEC_TO_SEC((float)getTimersLastRetro() - (float)getTimersOldRetro()); 
}


void updateRawMotionData() {
    float vel, accel, pos;

    pos = getMotionRetroCount() * STRIP_DISTANCE;
    vel = velFromRetro(pos);
    accel = accelFromRetro(vel);
    /* note: we want to reset xsens pos. too, stop drift*/
    pthread_mutex_lock(&lock);
    insertRing(&rawData.pos, pos);
    insertRing(&rawData.vel, vel);
    insertRing(&rawData.accel, accel);  /* Would be interesting to see diff w/IMU */
    pthread_mutex_unlock(&lock);
}

/* Should be holding mutex already */
static void insertRing(rawMotionNode_t *node, float val) {
    node->data[node->head] = val;
    node->head = (node->head + 1) % node->size;
}

/* Only choose one of these two */
void filterMotion(int filterType) {
    float pos, vel, accel;
    pthread_mutex_lock(&lock);
    if (filterType == FILTER_ROLLING) {
        pos = rollingAvgFloat(rawData.pos.data, rawData.pos.size);
        vel = rollingAvgFloat(rawData.vel.data, rawData.vel.size);
        accel = rollingAvgFloat(rawData.accel.data, rawData.accel.size);
    } else if (filterType == FILTER_EXP) {
        pos = expFilterFloat(rawData.pos.data[rawData.pos.head], 
                rawData.pos.data[rawData.pos.head == 0 ? rawData.pos.size - 1 : rawData.pos.head - 1],
                .9);
        vel = expFilterFloat(rawData.vel.data[rawData.vel.head], 
                rawData.vel.data[rawData.vel.head == 0 ? rawData.vel.size - 1 : rawData.vel.head - 1],
                .9);
        accel = expFilterFloat(rawData.accel.data[rawData.accel.head], 
                    rawData.accel.data[rawData.accel.head == 0 ? rawData.accel.size - 1 : rawData.accel.head - 1],
                    .9);
    } else {
        pos = rawData.pos.data[rawData.pos.head];
        vel = rawData.vel.data[rawData.vel.head];
        accel = rawData.accel.data[rawData.accel.head];
    }
    pthread_mutex_unlock(&lock);
    setMotionPos(pos);
    setMotionVel(vel);
    setMotionAccel(accel);
}

void resetNav();
void resetNav() {
    setMotionPos(0);
    setMotionVel(0);
    setMotionAccel(0);
    setMotionRetroCount(0);

    /* reset rest */
}

/* TODO Open Qs
 *      1. Find out how bad drift is
 *      2. Are we going in X or Y plane
 */
void navLoop(void *unused) {
    (void) unused;
    
    int lastRetroCount = 0;
    setMotionMissedRetro(0);
    csvFormatHeader();
    while (1) {
        if (lastRetroCount != getMotionRetroCount()) {
            updateRawMotionData();
            filterMotion(FILTER_NONE);
        }
    
        lastRetroCount = getMotionRetroCount();
    /*    showNavData();   */
    /*    csvFormatShow(); */
        usleep(10000);  /* Runs at 10ms */
    }
}
