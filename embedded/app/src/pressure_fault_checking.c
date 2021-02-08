/***
 *  Filename: pressure_fault_checking.c
 *
 *  Summary: Checks to make sure recorded pressures are within limits
 *	at all points during the different states. Information about specific 
 *	values can be found in the "states.h" file
 *
 */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "data.h"
#include "states.h"

#include "pressure_fault_checking.h"

// #define NOPRESSURE // AHHH NO PRESSURE NO PRESSURE

bool checkIdlePressures(void)
{
    #ifndef NOPRESSURE
    if (getPressurePrimTank() < PS1_BOTTOM_LIMIT_IDLE || getPressurePrimTank() > PS1_TOP_LIMIT_IDLE) {
        fprintf(stderr, "primTank pressure failing\n");
        return false;
    }
    if (getPressurePrimLine() < PS2_BOTTOM_LIMIT_IDLE || getPressurePrimLine() > PS2_TOP_LIMIT_IDLE) {
        fprintf(stderr, "primLine pressure failing\n");
        return false;
    }
    if (getPressurePrimAct() < PS3_BOTTOM_LIMIT_IDLE || getPressurePrimAct() > PS3_TOP_LIMIT_IDLE) {
        fprintf(stderr, "primAct pressure failing\n");
        return false;
    }
    if (getPressureSecTank() < SEC_PS1_BOTTOM_LIMIT_IDLE || getPressureSecTank() > SEC_PS1_TOP_LIMIT_IDLE) {
        fprintf(stderr, "Secondary primTank pressure failing\n");
        return false;
    }
    if (getPressureSecLine() < SEC_PS2_BOTTOM_LIMIT_IDLE || getPressureSecLine() > SEC_PS2_TOP_LIMIT_IDLE) {
        fprintf(stderr, "Secondary primLine pressure failing\n");
        return false;
    }
    if (getPressureSecAct() < SEC_PS3_BOTTOM_LIMIT_IDLE || getPressureSecAct() > SEC_PS3_TOP_LIMIT_IDLE) {
        fprintf(stderr, "Secondary primAct pressure failing\n");
        return false;
    }
    if (getPressurePv() < PV_BOTTOM_LIMIT || getPressurePv() > PV_TOP_LIMIT) {
        fprintf(stderr, "Pressure vessel depressurizing\n");
        return false;
    }
    #endif

    return true;
}

bool checkPrerunPressures(void)
{
    #ifndef NOPRESSURE
    if (getPressurePrimTank() < PS1_BOTTOM_LIMIT_PRE || getPressurePrimTank() > PS1_TOP_LIMIT_PRE) {
        fprintf(stderr, "primTank pressure failing\n");
        return false;
    }
    if (getPressurePrimLine() < PS2_BOTTOM_LIMIT_PRE || getPressurePrimLine() > PS2_TOP_LIMIT_PRE) {
        fprintf(stderr, "primLine pressure failing\n");
        return false;
    }
    if (getPressurePrimAct() < PS3_BOTTOM_LIMIT_PRE || getPressurePrimAct() > PS3_TOP_LIMIT_PRE) {
        fprintf(stderr, "primAct pressure failing\n");
        return false;
    }
    if (getPressureSecTank() < SEC_PS1_BOTTOM_LIMIT_PRE || getPressureSecTank() > SEC_PS1_TOP_LIMIT_PRE) {
        fprintf(stderr, "Secondary primTank pressure failing\n");
        return false;
    }
    if (getPressureSecLine() < SEC_PS2_BOTTOM_LIMIT_PRE || getPressureSecLine() > SEC_PS2_TOP_LIMIT_PRE) {
        fprintf(stderr, "Secondary primLine pressure failing\n");
        return false;
    }
    if (getPressureSecAct() < SEC_PS3_BOTTOM_LIMIT || getPressureSecAct() > SEC_PS3_TOP_LIMIT) {
        fprintf(stderr, "Secondary primAct pressure failing\n");
        return false;
    }
    if (getPressurePv() < PV_BOTTOM_LIMIT || getPressurePv() > PV_TOP_LIMIT) {
        fprintf(stderr, "Pressure vessel depressurizing\n");
        return false;
    }
    #endif

    return true;
}

bool checkBrakingPressures(void)
{
    #ifndef NOPRESSURE
    if (getPressurePrimTank() < PS1_BOTTOM_LIMIT_PRE || getPressurePrimTank() > PS1_TOP_LIMIT_PRE) {
        fprintf(stderr, "primTank pressure failing\n");
        return false;
    }
    if (getPressurePrimLine() < PS2_BOTTOM_LIMIT_PRE || getPressurePrimLine() > PS2_TOP_LIMIT_PRE) {
        fprintf(stderr, "primLine pressure failing\n");
        return false;
    }
    if (getPressurePrimAct() < PS3_BOTTOM_LIMIT_BRAKING || getPressurePrimAct() > PS3_TOP_LIMIT_BRAKING) {
        fprintf(stderr, "primAct pressure failing\n");
        return false;
    }
    if (getPressureSecTank() < SEC_PS1_BOTTOM_LIMIT_PRE || getPressureSecTank() > SEC_PS1_TOP_LIMIT_PRE) {
        fprintf(stderr, "Secondary primTank pressure failing\n");
        return false;
    }
    if (getPressureSecLine() < SEC_PS2_BOTTOM_LIMIT_PRE || getPressureSecLine() > SEC_PS2_TOP_LIMIT_PRE) {
        fprintf(stderr, "Secondary primLine pressure failing\n");
        return false;
    }
    if (getPressureSecAct() < SEC_PS3_BOTTOM_LIMIT || getPressureSecAct() > SEC_PS3_TOP_LIMIT) {
        fprintf(stderr, "Secondary primAct pressure failing\n");
        return false;
    }
    if (getPressurePv() < PV_BOTTOM_LIMIT || getPressurePv() > PV_TOP_LIMIT) {
        fprintf(stderr, "Pressure vessel depressurizing\n");
        return false;
    }
    #endif

    return true;
}

bool checkCrawlPostrunPressures(void)
{
    #ifndef NOPRESSURE
    if (getPressurePrimTank() < PS1_BOTTOM_LIMIT_CRAWLPOST || getPressurePrimTank() > PS1_TOP_LIMIT_CRAWLPOST) {
        fprintf(stderr, "primTank pressure failing\n");
        fprintf(stderr, "\t pressure is %d\n", getPressurePrimTank());
        return false;
    }
    if (getPressurePrimLine() < PS2_BOTTOM_LIMIT_CRAWLPOST || getPressurePrimLine() > PS2_TOP_LIMIT_CRAWLPOST) {
        fprintf(stderr, "primLine pressure failing\n");
        return false;
    }
    if (getPressurePrimAct() < PS3_BOTTOM_LIMIT_CRAWLPOST || getPressurePrimAct() > PS3_TOP_LIMIT_CRAWLPOST) {
        fprintf(stderr, "primAct pressure failing\n");
        return false;
    }
    if (getPressureSecTank() < SEC_PS1_BOTTOM_LIMIT_CRAWLPOST || getPressureSecTank() > SEC_PS1_TOP_LIMIT_CRAWLPOST) {
        fprintf(stderr, "Secondary primTank pressure failing\n");
        return false;
    }
    if (getPressureSecLine() < SEC_PS2_BOTTOM_LIMIT_CRAWLPOST || getPressureSecLine() > SEC_PS2_TOP_LIMIT_CRAWLPOST) {
        fprintf(stderr, "Secondary primLine pressure failing\n");
        return false;
    }
    if (getPressureSecAct() < SEC_PS3_BOTTOM_LIMIT || getPressureSecAct() > SEC_PS3_TOP_LIMIT) {
        fprintf(stderr, "Secondary primAct pressure failing\n");
        return false;
    }
    if (getPressurePv() < PV_BOTTOM_LIMIT || getPressurePv() > PV_TOP_LIMIT) {
        fprintf(stderr, "Pressure vessel depressurizing\n");
        return false;
    }
    #endif

    return true;
}
