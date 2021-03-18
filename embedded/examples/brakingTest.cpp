#include "connStat.h"
#include <TelemetryLoop.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "braking.h"
#include "hv_iox.h"

extern "C" {
#include "data.h"
#include "imu.h"
}
#define NUM_LIM_SWITCHES 2 /* In test */
#define FOREVER while (1)



int main(int argc, char* argv[])
{   
    printf("Run init\n");
    if (initComms() != 0) {
        printf("Failed to init comms\n");
    }

    sleep(2);
    printf("Run brake.\n");
    if (brake() != 0) {
        printf("Failed to brake.\n");
    }

    sleep(2);

    printf("Run unbrake.\n");
    if (unbrake() != 0) {
        printf("Failed to unbrake.\n");
    }
    printf("Braking test complete.\n");
    return 0;
}
