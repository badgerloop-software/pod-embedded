#ifndef STATES_H
#define STATES_H


/* Pressure sensor acceptable limits (in PSI) */
#define PS1_BOTTOM_LIMIT        1000
#define PS1_TOP_LIMIT           3000
#define PS2_BOTTOM_LIMIT        150
#define PS2_TOP_LIMIT           275
#define PS3_BOTTOM_LIMIT        1000
#define PS3_TOP_LIMIT           3000
#define PS4_BOTTOM_LIMIT        0
#define PS4_TOP_LIMIT           20

#define PS1_BOTTOM_LIMIT_CRAWLPOSTRUN 0
#define PS2_BOTTOM_LIMIT_CRAWLPOSTRUN 0
#define PS3_BOTTOM_LIMIT_CRAWLPOSTRUN 0

/**/

#define MAX_BATT_TEMP			60	/* Degrees Celcius */
#define MAX_STOPPED_ACCEL		0.3 /* Limit for qualifying the pod as stopped in m/s */

/* In mV */
#define MAX_CELL_VOLTAGE        4200    
#define MIN_CELL_VOLTAGE        3000

#define TUBE_LENGTH             3800    /*in ft //FIXME I could be wrong on this on haha */

#define MAX_RUN_TIME            30  /* in seconds, TODO get the real number */



#endif