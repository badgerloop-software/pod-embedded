#include <stdint.h>
#include <time.h>
#include <retro.h>

#define TIME_SINCE_LAST_RETRO 15000000


/* Functions for initializing the entire struct, and individual parts of it */
int initData(void);
int initMetaData(void);
int initPressureData(void);
int initMotionData(void);
int initBmsData(void);
int initRmsData(void);
int initFlagData(void);
int initTimerData(void);


/***
 *
 * Flags structure -
 *
 */

typedef struct flags_t {
    int readyPump;
    int pumpDown;
    int readyCommand;
    int propulse;
    int emergencyBrake;
} flags_t;


/***
 *
 * The Data Struct - Top of the great hierarchy of our descending data
 * tree
 *
 */

typedef struct data_t {
    struct pressure_t *pressure;
    struct motion_t   *motion;
    struct bms_t      *bms;
    struct rms_t      *rms;
    struct flags_t    *flags;
    struct timers_t   *timers;
    int state;
} data_t;


/***
 *
 * Timers struct -- For making sure that our run happens in a timely manner
 *
 */

typedef struct timers_t {
    uint64_t startTime;
    uint64_t lastRetro;
	uint64_t lastRetros[NUM_RETROS];
} timers_t;


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



/***
 * pressure_t - Pressure data from the braking system
 */
typedef struct pressure_t {
    uint32_t ps1;
    uint32_t ps2;
    uint32_t ps3;
    uint32_t ps4;
    uint32_t sec_ps1;
    uint32_t sec_ps2;
    uint32_t sec_ps3;
    uint32_t sec_ps4;
    uint32_t pv;
} pressure_t;


/***
 * motion_t - Where all movement data goes
 *
 * Fields are fairly self explanatory, assumed positive X direction (because the
 * pod only travels in one direction)
 *
 */
typedef struct motion_t {
    float pos;
    float vel;
    float accel;
    int retroCount;
} motion_t;


/***
 * bms_t - All of the data collected about the battery system
 *
 */
typedef struct bms_t {
    float packCurrent;
    float packVoltage;
    uint16_t packDCL;
    int16_t packCCL;
    uint16_t packResistance;
    uint8_t packHealth;
    float packOpenVoltage;
    uint16_t packCycles;
    uint16_t packAh;
    float inputVoltage;
    uint8_t Soc;
    uint16_t relayStatus;
    uint8_t highTemp;
    uint8_t lowTemp;
    uint16_t cellMaxVoltage;
    uint16_t cellMinVoltage;
    uint16_t cellAvgVoltage;
    uint8_t maxCells;
    uint8_t numCells;
} bms_t;

/***
 * rms_t - Collection of all of the RMS (motor controller) data
 *
 * Read in and filled in via CAN from the motor
 */
typedef struct rms_t {
    uint16_t igbtTemp;
    uint16_t gateDriverBoardTemp;
    uint16_t controlBoardTemp;
    uint16_t motorTemp;
    uint16_t motorSpeed;
    uint16_t phaseACurrent;
    uint16_t phaseBCurrent;
    uint16_t phaseCCurrent;
    uint16_t dcBusVoltage;
     //uint16_t output_voltage_peak;
    uint16_t lvVoltage;
    uint64_t canCode1;
    uint64_t canCode2;
    uint64_t faultCode1;
    uint64_t faultCode2;
    uint16_t commandedTorque;
    uint16_t actualTorque;
    uint16_t relayState;
    uint16_t  electricalFreq;
    uint16_t dcBusCurrent;
    uint16_t outputVoltageLn;
	uint16_t VSMCode;

	uint16_t keyMode;
} rms_t;


extern data_t *data;
