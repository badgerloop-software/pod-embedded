#include <stdio.h>
#include <unistd.h>
#include <TelemetryLoop.h>
#include <string.h>
#include "connStat.h"
#include "imu.h"
extern "C" {
#include "braking.h"
#include "lv_iox.h"
#include "data.h"
}
#define NUM_LIM_SWITCHES 2  /* In test */
#define FOREVER while(1)

void showBrakingInfo() {
    int i = 0;
    //showPressures();
    fprintf(stderr,"%f,%f,%f,%f,%f,%f,%f\n", getPressurePrimTank(), getPressurePrimLine(), getPressurePrimAct(),
            getPressureSecTank(), getPressureSecLine(), getPressureSecAct(), getPressurePv());
    printf("%f,%f,%f,%f,%f,%f,%f,%f\n", getPressurePrimTank(), getPressurePrimLine(), getPressurePrimAct(),
           getPressureSecTank(), getPressureSecLine(), getPressureSecAct(), getPressurePv());
}


int main(int argc, char *argv[]) {
    bool isHard = false;
    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0) isHard = true;
    }
    initData();
    initPressureMonitor();
    initLVIox(isHard);
    SetupIMU();
    SetupTelemetry((char *) DASHBOARD_IP, DASHBOARD_PORT);
    if (argc > 1) {
        if (strcmp(argv[1], "-p") == 0) {
            if (argc > 2) {
                if (strcmp(argv[2], "on") == 0)
                    brakePrimaryActuate();
                else if (strcmp(argv[2], "off") == 0)
                    brakePrimaryUnactuate();

            }
        }
        else if (strcmp(argv[1], "-s") == 0) {
            if (argc > 2) {
                if (strcmp(argv[2], "on") == 0)
                    brakeSecondaryActuate();
                else if (strcmp(argv[2], "off") == 0)
                    brakeSecondaryUnactuate();
            }
        }
    }
    fprintf(stderr, "primTank,primLine,primAct,secTank,secLine,secAct,pv\n");
    printf("primTank,primLine,primAct,secTank,secLine,secAct,amb,pv\n");
    FOREVER {
        showBrakingInfo();
        usleep(100000);
    }
}
