#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <rms.h>
#include <pthread.h>
#include <semaphore.h>
#include <motor.h>
#include <unistd.h>
#include <hv_iox.h>

/***
 * The high level interface for the motor
 */

#define HB_PERIOD 10000

static uint16_t TORQUE      = 0;
static bool     MOTOR_IS_ON = false;

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
int initMotor() {
    if (pthread_mutex_init(&torqueLock, NULL) != 0) {
        fprintf(stderr, "Failed to init torque mutex\n");
        return 1;
    }
    if (pthread_mutex_init(&motorLock, NULL) != 0) {
        fprintf(stderr, "Failed to init motor mutex\n");
        return 1;
    }
    if (sem_init(&hbSem, 0, 1) != 0) {
        fprintf(stderr, "Failed to init motor heartbeat semaphore\n");
        return 1;
    }

    setTorque(0);         /* Make sure torque is zero */
    setMotorIsOn(false);  /* To determine when we should send hbs */
    JOIN_HB_THREAD = false; /* Set true to stop the motor thread */

    /* Spawn a thread to do heartbeats here */
    if (pthread_create(&hbLoop, NULL, motorHeartbeatLoop, NULL) != 0) {
        fprintf(stderr, "Failed to init motor heartbeat thread\n");
        return 1;
    }
    return 0;
}

void joinMotorHbThread() {
    JOIN_HB_THREAD = true;
    if (pthread_join(hbLoop, NULL) != 0) {
        fprintf(stderr, "Failed to join thread\n");
    }
}

void setTorque(uint16_t val) {
    pthread_mutex_lock(&torqueLock);
    TORQUE = val;
    pthread_mutex_unlock(&torqueLock);
}

uint16_t getTorque() {
    uint16_t ret;
    pthread_mutex_lock(&torqueLock);
    ret = TORQUE;
    pthread_mutex_unlock(&torqueLock);
    return ret;
}

void setMotorIsOn(bool val) {
    pthread_mutex_lock(&motorLock);
    MOTOR_IS_ON = val;
    pthread_mutex_unlock(&motorLock);
}

bool getMotorIsOn() {
    bool ret;
    pthread_mutex_lock(&motorLock);
    ret = MOTOR_IS_ON;
    pthread_mutex_unlock(&motorLock);
    return ret;
}

int startMotor() {
    static bool isMCULatched = false;
    if (!isMCULatched) {
        setMCULatch(true);
/*        usleep(1000000);  //FIXME Not sure what to make this, .5 s for now*/
/*        setMCULatch(false);*/
/*        isMCULatched = true;*/
    }
    if (rmsEnHeartbeat() != 0) return 1;
	if (rmsClrFaults() != 0) return 1;
	if (rmsInvDis() != 0) return 1;
/*	idleMotor();*/
    sleep(3); /* Not ideal, but unless we have a way to check status this stays */
    rmsInvEnNoTorque();
	setMotorIsOn(true);
    return 0;
}

int stopMotor() {
    setTorque(0);
    
    /* we dont want to both command the inverter + torque
     * right after disabling it here because it's mid HB message */
    sem_wait(&hbSem);
    setMotorIsOn(false);
    sem_post(&hbSem);

    /* Unsure if there needs to be a delay inbetween these commands */
    if (rmsCmdNoTorque() != 0) return 1;
    if (rmsDischarge() != 0) return 1;        
    if (rmsInvDis() != 0) return 1;
    return 0;
}

/* Short hand for commanding no torque but leaving motor on */
void idleMotor() {
    setTorque(0);
}

/* Run in its own thread, this sends required hbs to the motor 
 *  every HB_PERIOD uS
 */
static void *motorHeartbeatLoop(void *unusedParam) {
    while(1) {
        //if (JOIN_HB_THREAD) return NULL;
		
		sem_wait(&hbSem);
        if (getMotorIsOn()) {
            printf("MOTOR IS ON\n");
			if (rmsSendHbMsg(getTorque()) != 0) {
                fprintf(stderr, "Failed to send heartbeat\n");
            }
        } else {
			printf("IDLE\n");
            if (rmsIdleHb() != 0) {
                fprintf(stderr, "Failed to send idle heartbeat\n");
            }
        }
        sem_post(&hbSem);
        usleep(HB_PERIOD);
    }
    return NULL;
}

