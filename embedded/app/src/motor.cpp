#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <motor.h>
#include <unistd.h>
#include <hv_iox.h>

extern "C" {
#include <data.h>
#include <rms.h>
}
/***
 * The high level interface for the motor
 */

#define HB_PERIOD 10000


/* Thread management variables */
extern int rmsInvEnNoTorque();

/* Create the semaphores and mutexes for thread sensitive operations
 *  and creates the HB loop that just waits for the motor to be started 
*/


static pthread_t hbThread;
static bool motorEnabled = false;
static bool lowTorqueMode = false;
static void *motorHbLoop(void *arg);

void setMotorEn() {
    motorEnabled = true;
}

void clrMotorEn() {
    motorEnabled = false;
}

void setMotorCrawl() {
    lowTorqueMode = true;
}

void clrMotorCrawl() {
    lowTorqueMode = false;
}

void SetupMotor() {
    pthread_create(&hbThread, NULL, (motorHbLoop), NULL);
}

static void *motorHbLoop(void *arg) {
    (void) arg;
    while(1) {
        if (motorEnabled)
            rms.rmsSendHbMsg(2);
        else if (lowTorqueMode)
            rms.rmsSendHbMsg(2);
        else
            rms.rmsIdleHb();
        usleep(10000);
    }
    
    return NULL;
}

