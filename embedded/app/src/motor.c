#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <rms.h>
#include <pthread.h>
#include <semaphore.h>
#include <motor.h>
#include <unistd.h>
#include <hv_iox.h>
#include <data.h>
/***
 * The high level interface for the motor
 */

#define HB_PERIOD 10000


/* Thread management variables */
static pthread_mutex_t torqueLock, motorLock;
static sem_t hbSem;
static pthread_t hbLoop;
static bool JOIN_HB_THREAD = false;

static void *motorHeartbeatLoop(void *);
extern int rmsInvEnNoTorque();
/* Create the semaphores and mutexes for thread sensitive operations
 *  and creates the HB loop that just waits for the motor to be started 
*/


static pthread_t hbThread;
static bool motorEnabled = false;
static bool lowTorqueMode = false;
static void motorHbLoop();

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

static void motorHbLoop() {
    while(1) {
        if (motorEnabled)
            rmsInvEn();
        else if (lowTorqueMode)
            rmsInvEnNoTorque();
        else
            rmsIdleHb();
        usleep(10000);
    }
}

