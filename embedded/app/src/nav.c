#include <stdio.h>
#include <pthread.h>
#include <data.h>


#define STRIP_DISTANCE 100 /*ft*/
#define TOTAL_DISTANCE 1000 /*ft, not real TODO change*/
#define TOTAL_RUN_TIME 30   /* s */

static pthread_t navThread;




void initNav() {

    if (pthread_create(&navThread, NULL, navLoop, NULL) != 0) {
        fprintf(stderr, "Error creating nav thread\n");
    }
}


/* TODO Open Qs
 *      1. Find out how bad drift is
 *      2. Are we going in X or Y plane
 */

void navLoop() {
    static int lastRetroCount = data->motion->retroCount;
    if (lastRetroCount != data->motion->retroCount) {
        /* Reset accel pos.*/
        data->motion->position = imuData->position = data->motion->retroCount * STRIP_DISTANCE;
        
    }
    
    if (imuData->position - data->motion->position > STRIP_DISTANCE * 2) {
            /* If it is that different, we likely missed a strip */
            fprintf(stderr, "MISSED A TAPESTRIP\n"); //FIXME All we do rn is notify, to be adjusted in test
    }
    
    if (getUsTimestamp() - data->timers->startTime > GLOBAL_RUN_TIME) {
        shouldStop = true;
    }

    


    lastRetroCount = data->motion->retroCount;
}
