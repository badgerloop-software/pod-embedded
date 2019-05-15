/***
 *  Filename: pressure_fault_checking.c
 *
 *  Summary: Checks to make sure recorded pressures are within limits
 *	at all points during the different states. Information about specific 
 *	values can be found in the "states.h" file
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "data.h"
#include "states.h"

#include "pressure_fault_checking.h"

extern data_t *data;

bool checkPrerunPressures(void) {
	if (data->pressure->ps1 < PS1_BOTTOM_LIMIT_PRE || data->pressure->ps1 > PS1_TOP_LIMIT_PRE) {
		fprintf(stderr, "PS1 pressure failing\n");
		return false;
	}
	if (data->pressure->ps2 < PS2_BOTTOM_LIMIT_PRE || data->pressure->ps2 > PS2_TOP_LIMIT_PRE) {
		fprintf(stderr, "PS2 pressure failing\n");
		return false;
	}
	if (data->pressure->ps3 < PS3_BOTTOM_LIMIT_PRE || data->pressure->ps3 > PS3_TOP_LIMIT_PRE) {
		fprintf(stderr, "PS3 pressure failing\n");
		return false;
	}
	if (data->pressure->sec_ps1 < SEC_PS1_BOTTOM_LIMIT_PRE || data->pressure->sec_ps1 > SEC_PS1_TOP_LIMIT_PRE) {
		fprintf(stderr, "Secondary PS1 pressure failing\n");
		return false;
	}
	if (data->pressure->sec_ps2 < SEC_PS2_BOTTOM_LIMIT_PRE || data->pressure->sec_ps2 > SEC_PS2_TOP_LIMIT_PRE) {
		fprintf(stderr, "Secondary PS2 pressure failing\n");
		return false;
	}
	if (data->pressure->sec_ps3 < SEC_PS3_BOTTOM_LIMIT || data->pressure->sec_ps3 > SEC_PS3_TOP_LIMIT) {
		fprintf(stderr, "Secondary PS3 pressure failing\n");
		return false;
	}
	

	return true;
}

bool checkBrakingPressures(void) {
	if (data->pressure->ps1 < PS1_BOTTOM_LIMIT_PRE || data->pressure->ps1 > PS1_TOP_LIMIT_PRE) {
		fprintf(stderr, "PS1 pressure failing\n");
		return false;
	}
	if (data->pressure->ps2 < PS2_BOTTOM_LIMIT_PRE || data->pressure->ps2 > PS2_TOP_LIMIT_PRE) {
		fprintf(stderr, "PS2 pressure failing\n");
		return false;
	}
	if (data->pressure->ps3 < PS3_BOTTOM_LIMIT_BRAKING || data->pressure->ps3 > PS3_TOP_LIMIT_BRAKING) {
		fprintf(stderr, "PS3 pressure failing\n");
		return false;
	}
	if (data->pressure->sec_ps1 < SEC_PS1_BOTTOM_LIMIT_PRE || data->pressure->sec_ps1 > SEC_PS1_TOP_LIMIT_PRE) {
		fprintf(stderr, "Secondary PS1 pressure failing\n");
		return false;
	}
	if (data->pressure->sec_ps2 < SEC_PS2_BOTTOM_LIMIT_PRE || data->pressure->sec_ps2 > SEC_PS2_TOP_LIMIT_PRE) {
		fprintf(stderr, "Secondary PS2 pressure failing\n");
		return false;
	}
	if (data->pressure->sec_ps3 < SEC_PS3_BOTTOM_LIMIT || data->pressure->sec_ps3 > SEC_PS3_TOP_LIMIT) {
		fprintf(stderr, "Secondary PS3 pressure failing\n");
		return false;
	}
	

	return true;
}

bool checkCrawlPostrunPressures(void) {
	if (data->pressure->ps1 < PS1_BOTTOM_LIMIT_CRAWLPOST || data->pressure->ps1 > PS1_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "PS1 pressure failing\n");
		return false;
	}
	if (data->pressure->ps2 < PS2_BOTTOM_LIMIT_CRAWLPOST || data->pressure->ps2 > PS2_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "PS2 pressure failing\n");
		return false;
	}
	if (data->pressure->ps3 < PS3_BOTTOM_LIMIT_CRAWLPOST || data->pressure->ps3 > PS3_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "PS3 pressure failing\n");
		return false;
	}
	if (data->pressure->sec_ps1 < SEC_PS1_BOTTOM_LIMIT_CRAWLPOST || data->pressure->sec_ps1 > SEC_PS1_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "Secondary PS1 pressure failing\n");
		return false;
	}
	if (data->pressure->sec_ps2 < SEC_PS2_BOTTOM_LIMIT_CRAWLPOST || data->pressure->sec_ps2 > SEC_PS2_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "Secondary PS2 pressure failing\n");
		return false;
	}
	if (data->pressure->sec_ps3 < SEC_PS3_BOTTOM_LIMIT || data->pressure->sec_ps3 > SEC_PS3_TOP_LIMIT) {
		fprintf(stderr, "Secondary PS3 pressure failing\n");
		return false;
	}
	

	return true;
}