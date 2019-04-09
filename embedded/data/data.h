#include <stdint.h>

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
    int state;
} data_t;

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
} rms_t;
