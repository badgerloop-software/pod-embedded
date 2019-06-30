#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "braking.h"
#include "lv_iox.h"
#include "data.h"

#define NUM_LIM_SWITCHES 4  /* In test */
#define FOREVER while(1)

void showBrakingInfo() {
    int i = 0;
    showPressures();
    for (i = 0; i < NUM_LIM_SWITCHES; i++)
        printf("Limit switch %d: %d\n", i, limSwitchGet(i));
}


int main(int argc, char *argv[]) {
    bool isHard = false;
    if (argc > 1) {
        if (strcmp(argv[1], "-h")) isHard = true;
    }
    initData();
    initPressureMonitor();
    initLVIox(isHard);

    if (argc > 1) {
        if (strcmp(argv[1], "-p") == 0) {
            if (argc > 2) {
                if (strcmp(argv[2], "on") == 0)
                    brakePrimary();
                else if (strcmp(argv[2], "off") == 0)
                    brakePrimaryRelease();

            }
        }
        else if (strcmp(argv[1], "-s") == 0) {
            if (argc > 2) {
                if (strcmp(argv[2], "on") == 0)
                    brakeSecondary();
                else if (strcmp(argv[2], "off") == 0)
                    brakeSecondaryRelease();
            }
        }
    }

    FOREVER {
        showBrakingInfo();
        usleep(100000);
    }
}
