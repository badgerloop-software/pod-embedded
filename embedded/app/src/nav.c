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

static pthread_t navThread;

static void navLoop(void *arg);


void initNav() {

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
    setPosX(data->motion->pos = pos);
    data->motion->vel = vel;  /* Would be interesting to see diff w/IMU */
    data->motion->accel = accel;
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
