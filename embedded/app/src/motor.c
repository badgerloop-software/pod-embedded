#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <rms.h>

/***
 * The high level interface for the motor
 */

#define HB_PERIOD 300000

static uint16_t TORQUE = 0;
static bool     MOTOR_IS_ON = false;
static pthread_mutex_t torqueLock, motorLock;
static pthread_t hbLoop;
/* TODO add a mutex for torque */


static void motorHeartbeatLoop(void);

int initMotor() {
    if (pthread_mutex_init(&torqueLock, NULL) != 0) {
        fprintf(stderr, "Failed to init torque mutex\n");
        return 1;
    }
    if (pthread_mutex_init(&motorLock, NULL) != 0) {
        fprintf(stderr, "Failed to init motor mutex\n");
        return 1;
    }

    setTorque(0);
    setMotorIsOn(false);

    /* Spawn a thread to do hbs here */
    if (pthread_create(&hbLoop, NULL, &motorHeartbeatLoop, NULL) != 0) {
        fprintf(stderr, "Failed to init motor heartbeat thread\n");
        return 1;
    }
    return 0;
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
    if (rmsEnHeartbeat() != 0) return 1;
    rmsClrFaults();
    rmsInvEn();
    idleMotor();
    sleep(3); /* Not ideal, but unless we have a way to check status this stays */
    setMotorIsOn(true);
}

int stopMotor() {
    setTorque(0);
    
    rmsCmdNoTorque();
    
}

void idleMotor() {
    setTorque(0);
}

static void motorHeartbeatLoop() {
    while(1) {
        if (getMotorIsOn()) {
            rmsSendHbMsg(getTorque());
        }
        usleep(HB_PERIOD);
    }
}
