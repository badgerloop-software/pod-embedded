#include <stdio.h>
#include <stdlib.h>
#include <data.h>
#include <fcntl.h>
#include <i2c.h>
#include <proc_iox.h>
#include "semaphore.h"


/* Checks whether or not the hardware has been initialized,
 * and therefor needs to be set to default values. It should
 * only be set to default values if the system hasn't been initialized
 *
 * Returns:
 *      1 if it has been hard initialized since boot,
 *      0 if this is the first time running since boot,
 *      -1 if there is an error checking its status
 */
int isEarlyInit() {
    if (initProcIox(false) != 0) return -1;
    if (earlyInitPinGet() == 0) {
        if (earlyInitPinSet(true) != 0) return -1;
        return 0;
    }
    return 1;
}

data_t *data;

/**!!AUTO-GENERATE HERE!!**/
