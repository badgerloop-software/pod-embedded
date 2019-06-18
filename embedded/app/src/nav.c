#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <data.h>
#include <imu.h>

#define FEET_TO_METERS(x) ((x) * 0.3048)
#define USEC_TO_SEC(x)    ((x) / 100000)
#define STRIP_DISTANCE FEET_TO_METERS(100) /*m*/
#define TOTAL_DISTANCE FEET_TO_METERS(1000) /*m, not real TODO change*/
#define TOTAL_RUN_TIME 30   /* s */
#define EXPECTED_DECEL 9.8  /* m/s/s */

#define WINDOW_SIZE    10

static pthread_t navThread;
static pthread_mutex_t lock;
static void navLoop(void *arg);

static rawMotion_t rawData;

void initNav() {

    rawData.pos.size    = WINDOW_SIZE;
    rawData.pos.head    = 0;
    rawData.pos.data    = (float) malloc(sizeof(float) * rawData.pos.size);
    if (rawData.pos.data == NULL) fprintf(stderr, "Nav data malloc fail\n");

    rawData.vel.size    = WINDOW_SIZE;
    rawData.vel.head    = 0;
    rawData.vel.data    = (float) malloc(sizeof(float) * rawData.vel.size);
    if (rawData.vel.data == NULL) fprintf(stderr, "Nav data malloc fail\n");

    rawData.accel.size  = WINDOW_SIZE;
    rawData.accel.head  = 0;
    rawData.accel.data  = (float) malloc(sizeof(float) * rawData.accel.size);
    if (rawData.accel.data == NULL) fprintf(stderr, "Nav data malloc fail\n");


    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Error creating nav mutex\n");
    }
    
    if (pthread_create(&navThread, NULL, navLoop, NULL) != 0) {
        fprintf(stderr, "Error creating nav thread\n");
    }
}

void showNavData() {
    printf("Pos: %0.4f ; Vel: %0.4f ; Accel%0.4f\n", 
            data->motion->pos, data->motion->vel, data->motion->accel);
}

static inline float accelFromRetro(float currVel) {
   return (currVel - data->motion->vel) / 
       USEC_TO_SEC(data->timers->lastRetro - USEC_TO_SEC(data->timers->oldRetro)); 
}


static inline float velFromRetro(float currPos) {
   return (currPos - data->motion->pos) / 
       USEC_TO_SEC(data->timers->lastRetro - data->timers->oldRetro); // ft/uS
}


void updateMotionData() {
    float vel, accel, pos;

    pos = data->motion->retroCount * STRIP_DISTANCE;
    vel = velFromRetro(pos);
    accel = accelFromRetro(vel);
    /* note: we want to reset xsens pos. too, stop drift*/
    pthread_mutex_lock(&lock);
    insertRing(&rawData.pos, pos);
    insertRing(&rawData.vel, vel);
    insertRing(&rawData.accel, accel);  /* Would be interesting to see diff w/IMU */
    pthread_mutex_unlock(&lock);
}

static void insertRing(rawMotionNode_t *node, float val) {
    node.data[node.head] = val;
    node.head = (node.head + 1) % node.size
}

/* TODO Open Qs
 *      1. Find out how bad drift is
 *      2. Are we going in X or Y plane
 */
void navLoop(void *unused) {
    static int lastRetroCount = 0;
   
    if (lastRetroCount != data->motion->retroCount) {
        updateMotionData();
    }
    
    if ((getPosX() - data->motion->pos) > STRIP_DISTANCE * 2) {
            /* If it is that different, we likely missed a strip */
            fprintf(stderr, "MISSED A TAPESTRIP\n"); //FIXME All we do rn is notify, to be adjusted in test
    }
    
    if ((getuSTimestamp() - data->timers->startTime) > TOTAL_RUN_TIME) {
        printf("Hit time limit\n");
        data->flags->shouldStop = true;
    }


    lastRetroCount = data->motion->retroCount;
/*    showNavData(); */
    usleep(10000);  /* Runs at 10ms */
}
