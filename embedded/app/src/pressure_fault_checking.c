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

bool checkIdlePressures(void) {
	if (data->pressure->primTank < PS1_BOTTOM_LIMIT_IDLE || data->pressure->primTank > PS1_TOP_LIMIT_IDLE) {
		fprintf(stderr, "primTank pressure failing\n");
		return false;
	}
	if (data->pressure->primLine < PS2_BOTTOM_LIMIT_IDLE || data->pressure->primLine > PS2_TOP_LIMIT_IDLE) {
		fprintf(stderr, "primLine pressure failing\n");
		return false;
	}
	if (data->pressure->primAct < PS3_BOTTOM_LIMIT_IDLE || data->pressure->primAct > PS3_TOP_LIMIT_IDLE) {
		fprintf(stderr, "primAct pressure failing\n");
		return false;
	}
	if (data->pressure->secTank < SEC_PS1_BOTTOM_LIMIT_IDLE || data->pressure->secTank > SEC_PS1_TOP_LIMIT_IDLE) {
		fprintf(stderr, "Secondary primTank pressure failing\n");
		return false;
	}
	if (data->pressure->secLine < SEC_PS2_BOTTOM_LIMIT_IDLE || data->pressure->secLine > SEC_PS2_TOP_LIMIT_IDLE) {
		fprintf(stderr, "Secondary primLine pressure failing\n");
		return false;
	}
	if (data->pressure->secAct < SEC_PS3_BOTTOM_LIMIT_IDLE || data->pressure->secAct > SEC_PS3_TOP_LIMIT_IDLE) {
		fprintf(stderr, "Secondary primAct pressure failing\n");
		return false;
	}
	if (data->pressure->pv < PV_BOTTOM_LIMIT || data->pressure->pv > PV_TOP_LIMIT) {
        fprintf(stderr, "Pressure vessel depressurizing\n");
        return false;
    }
  
	return true;
}

bool checkPrerunPressures(void) {
	if (data->pressure->primTank < PS1_BOTTOM_LIMIT_PRE || data->pressure->primTank > PS1_TOP_LIMIT_PRE) {
		fprintf(stderr, "primTank pressure failing\n");
		return false;
	}
	if (data->pressure->primLine < PS2_BOTTOM_LIMIT_PRE || data->pressure->primLine > PS2_TOP_LIMIT_PRE) {
		fprintf(stderr, "primLine pressure failing\n");
		return false;
	}
	if (data->pressure->primAct < PS3_BOTTOM_LIMIT_PRE || data->pressure->primAct > PS3_TOP_LIMIT_PRE) {
		fprintf(stderr, "primAct pressure failing\n");
		return false;
	}
	if (data->pressure->secTank < SEC_PS1_BOTTOM_LIMIT_PRE || data->pressure->secTank > SEC_PS1_TOP_LIMIT_PRE) {
		fprintf(stderr, "Secondary primTank pressure failing\n");
		return false;
	}
	if (data->pressure->secLine < SEC_PS2_BOTTOM_LIMIT_PRE || data->pressure->secLine > SEC_PS2_TOP_LIMIT_PRE) {
		fprintf(stderr, "Secondary primLine pressure failing\n");
		return false;
	}
	if (data->pressure->secAct < SEC_PS3_BOTTOM_LIMIT || data->pressure->secAct > SEC_PS3_TOP_LIMIT) {
		fprintf(stderr, "Secondary primAct pressure failing\n");
		return false;
	}
	if (data->pressure->pv < PV_BOTTOM_LIMIT || data->pressure->pv > PV_TOP_LIMIT) {
        fprintf(stderr, "Pressure vessel depressurizing\n");
        return false;
    }
    
	return true;
}

bool checkBrakingPressures(void) {
	if (data->pressure->primTank < PS1_BOTTOM_LIMIT_PRE || data->pressure->primTank > PS1_TOP_LIMIT_PRE) {
		fprintf(stderr, "primTank pressure failing\n");
		return false;
	}
	if (data->pressure->primLine < PS2_BOTTOM_LIMIT_PRE || data->pressure->primLine > PS2_TOP_LIMIT_PRE) {
		fprintf(stderr, "primLine pressure failing\n");
		return false;
	}
	if (data->pressure->primAct < PS3_BOTTOM_LIMIT_BRAKING || data->pressure->primAct > PS3_TOP_LIMIT_BRAKING) {
		fprintf(stderr, "primAct pressure failing\n");
		return false;
	}
	if (data->pressure->secTank < SEC_PS1_BOTTOM_LIMIT_PRE || data->pressure->secTank > SEC_PS1_TOP_LIMIT_PRE) {
		fprintf(stderr, "Secondary primTank pressure failing\n");
		return false;
	}
	if (data->pressure->secLine < SEC_PS2_BOTTOM_LIMIT_PRE || data->pressure->secLine > SEC_PS2_TOP_LIMIT_PRE) {
		fprintf(stderr, "Secondary primLine pressure failing\n");
		return false;
	}
	if (data->pressure->secAct < SEC_PS3_BOTTOM_LIMIT || data->pressure->secAct > SEC_PS3_TOP_LIMIT) {
		fprintf(stderr, "Secondary primAct pressure failing\n");
		return false;
	}
	if (data->pressure->pv < PV_BOTTOM_LIMIT || data->pressure->pv > PV_TOP_LIMIT) {
        fprintf(stderr, "Pressure vessel depressurizing\n");
        return false;
    }
    

	return true;
}

bool checkCrawlPostrunPressures(void) {
	if (data->pressure->primTank < PS1_BOTTOM_LIMIT_CRAWLPOST || data->pressure->primTank > PS1_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "primTank pressure failing\n");
		return false;
	}
	if (data->pressure->primLine < PS2_BOTTOM_LIMIT_CRAWLPOST || data->pressure->primLine > PS2_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "primLine pressure failing\n");
		return false;
	}
	if (data->pressure->primAct < PS3_BOTTOM_LIMIT_CRAWLPOST || data->pressure->primAct > PS3_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "primAct pressure failing\n");
		return false;
	}
	if (data->pressure->secTank < SEC_PS1_BOTTOM_LIMIT_CRAWLPOST || data->pressure->secTank > SEC_PS1_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "Secondary primTank pressure failing\n");
		return false;
	}
	if (data->pressure->secLine < SEC_PS2_BOTTOM_LIMIT_CRAWLPOST || data->pressure->secLine > SEC_PS2_TOP_LIMIT_CRAWLPOST) {
		fprintf(stderr, "Secondary primLine pressure failing\n");
		return false;
	}
	if (data->pressure->secAct < SEC_PS3_BOTTOM_LIMIT || data->pressure->secAct > SEC_PS3_TOP_LIMIT) {
		fprintf(stderr, "Secondary primAct pressure failing\n");
		return false;
	}
	if (data->pressure->pv < PV_BOTTOM_LIMIT || data->pressure->pv > PV_TOP_LIMIT) {
        fprintf(stderr, "Pressure vessel depressurizing\n");
        return false;
    }
    

	return true;
}
