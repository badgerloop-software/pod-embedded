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
    int pos;
    int vel;
    int accel;
    int retro_count;
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
    uint16_t igbt_temp;
    uint16_t gate_driver_board_temp;
    uint16_t control_board_temp;
    uint16_t motor_temp;
    uint16_t motor_speed;
    uint16_t phase_a_current;
    uint16_t phase_b_current;
    uint16_t phase_c_current;
    uint16_t dc_bus_voltage;
     //uint16_t output_voltage_peak;
    uint16_t lv_voltage;
    uint64_t can_code_1;
    uint64_t can_code_2;
    uint64_t fault_code_1;
    uint64_t fault_code_2;
    uint16_t commanded_torque;
    uint16_t actual_torque;
    uint16_t relay_state;
    uint16_t  electrical_freq;
    uint16_t dc_bus_current;
    uint16_t output_voltage_ln;
} rms_t;
