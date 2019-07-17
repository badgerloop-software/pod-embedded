#include <NCD9830DBR2G.h>
#include <braking.h>
#include <data.h>
#include <stdio.h>
#include <lv_iox.h>
#include <pthread.h>

#define VOLTAGE_2000_SCALING(x) ((((((x/256.0)*5.0)-0.5)/4.0)*2000.0))
#define CURRENT_500_SCALING(x)  ( ((((x / 256.0) * 5.0) - 0.6) / 2.4) * 500.0)
#define CURRENT_50_SCALING(x)   ( ((((x / 256.0) * 5.0) - 0.6) / 2.4) * 50.0)

#define RING_SIZE  50
#define LOOP_PERIOD 100000

static double avgDouble(double *arr, int size);
double readPressureVessel();

double readPressureVessel(); 
static pthread_t presMonThread;

int initPressureMonitor() {

    if (initPressureSensors() != 0) {
        fprintf(stderr, "Failed to init ADCs\n");
        return (-1);
    }
    if (pthread_create(&presMonThread, NULL, (void *)(pressureMonitor), NULL) != 0) {
        fprintf(stderr, "Failed to init pressure monitor\n");
        return (-1);
    }
    return 0;
}

void *pressureMonitor() {
    double primTankRing[RING_SIZE];
    double primLineRing[RING_SIZE];
    double primActRing[RING_SIZE];

    double secTankRing[RING_SIZE];
    double secLineRing[RING_SIZE];
    double secActRing[RING_SIZE];

    double pvRing[RING_SIZE];

    uint64_t i = 0;
    while(1) {
        primTankRing[i % RING_SIZE] = readPrimaryTank() + 10.76;
        primLineRing[i % RING_SIZE] = readPrimaryLine();
        primActRing[i % RING_SIZE]  = readPrimaryActuator();

        secTankRing[i % RING_SIZE]  = readSecTank() + 11.83;
        secLineRing[i % RING_SIZE]  = readSecLine();
        secActRing[i % RING_SIZE]   = readSecActuator();

        pvRing[i % RING_SIZE]       = readPressureVessel();

        data->pressure->primTank = avgDouble(primTankRing, RING_SIZE);
        data->pressure->primLine = avgDouble(primLineRing, RING_SIZE);
        data->pressure->primAct  = avgDouble(primActRing,  RING_SIZE);
        data->pressure->secTank  = avgDouble(secTankRing,  RING_SIZE);
        data->pressure->secLine  = avgDouble(secLineRing,  RING_SIZE);
        data->pressure->secAct   = avgDouble(secActRing,   RING_SIZE);
        data->pressure->pv       = avgDouble(pvRing,       RING_SIZE);
#ifdef DEBUG_PRES
        showPressures();
#endif
        i += 1;
        usleep(LOOP_PERIOD);
    }

    return NULL;
}

int joinPressureMonitor() {
    return pthread_join(presMonThread, NULL);
}

static double avgDouble(double *arr, int size) {
    int i = 0;
    double sum = 0;
    for (i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / (double) size;
}

int brake() {
    brakePrimaryActuate();
    usleep(500000);
    if (!limSwitchGet(PRIM_LIM_SWITCH))
        brakeSecondaryActuate();
    return 0;
}

void brakeHV() {
    data->flags->shouldBrake = true;
}

int brakePrimaryUnactuate() {
    if (solenoidSet(SOLENOID_0, 1) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_0\n");
        return 1;
    }
    if (solenoidSet(SOLENOID_2, 1) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_1\n");
        return 1;
    }

    return 0;
}

int brakeSecondaryUnactuate() {
    if (solenoidSet(SOLENOID_4, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_4\n");
        return 1;
    }
    if (solenoidSet(SOLENOID_5, 1) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_5\n");
        return 1;
    }
    if (solenoidSet(SOLENOID_6, 1) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_6\n");
        return 1;
    }

    return 0;
}

int brakePrimaryActuate() {
    if (solenoidSet(SOLENOID_0, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_0\n");
        return 1;
    }
    if (solenoidSet(SOLENOID_2, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_1\n");
        return 1;
    }

    return 0;
}

int brakeSecondaryActuate() {
    if (solenoidSet(SOLENOID_4, 1) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_2\n");
        return 1;
    }
    if (solenoidSet(SOLENOID_5, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_3\n");
        return 1;
    }
    if (solenoidSet(SOLENOID_6, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_4\n");
        return 1;
    }

    return 0;
}

int brakePrimaryVent() {
    if (solenoidSet(SOLENOID_0, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_0\n");
        return 1;
    }
    if (solenoidSet(SOLENOID_2, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_1\n");
        return 1;
    }

    return 0;
}
  
int brakeSecondaryVent() {
    if (solenoidSet(SOLENOID_4, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_2\n");
        return 1;
    }
    if (solenoidSet(SOLENOID_5, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_3\n");
        return 1;
    }
    if (solenoidSet(SOLENOID_6, 0) != 0) {
        fprintf(stderr, "Failed to set SOLENOID_4\n");
        return 1;
    }

    return 0;
}
 

//Voltage
double readPrimaryTank() {
    uint8_t data[2];
    if (readPressureSensor(ADC_0, PS_TANK, data) != 0)
        return -1;
    return (VOLTAGE_2000_SCALING((double) data[0]));
}

//Current
double readPrimaryLine() {
    uint8_t data[2];
    if (readPressureSensor(ADC_0, PS_LINE, data) != 0)
        return -1;
    return ( CURRENT_500_SCALING((double) data[0]) );
}
//Current
double readPrimaryActuator() {
    uint8_t data[2];
    if (readPressureSensor(ADC_0, PS_ACTUATE, data) != 0) {
        return -1;
    }
    return ( CURRENT_500_SCALING((double) data[0]) );
}


//Voltage
double readSecTank() {
    uint8_t data[2];
    if (readPressureSensor(ADC_0, BS_TANK, data) != 0)
        return -1;
    return (VOLTAGE_2000_SCALING((double) data[0]));
}

//Current
double readSecLine() {
    uint8_t data[2];
    if (readPressureSensor(ADC_0, BS_LINE, data) != 0)
        return -1;
    return ( CURRENT_500_SCALING((double) data[0]) );
}

//Current
double readSecActuator() {
    uint8_t data[2];
    if (readPressureSensor(ADC_0, BS_ACTUATE, data) != 0) {
        return -1;
    }
  
    return ( CURRENT_500_SCALING((double) data[0]) );
}


/* Damn I dont know how to spell vessel */
double readPressureVessel() {
    uint8_t data[2];
    if (readPressureSensor(ADC_0, PRES_VESL, data) != 0) {
        return -1;
    }
    return ( CURRENT_50_SCALING( (double) data[0]) );
}

void showPressures() {
    printf("======PRESSURE READINGS======\n");
    printf("---Primary System---\n");
    printf("Primary Tank: %f psi\n", data->pressure->primTank);
    printf("Primary Line: %f psi\n", data->pressure->primLine);
    printf("Primary Actuator: %f psi\n", data->pressure->primAct);
    printf("---Secondary System---\n");
    printf("Secondary Tank: %f psi\n", data->pressure->secTank);
    printf("Secondary Line: %f psi\n", data->pressure->secLine);
    printf("Secondary Actuator: %f psi\n\n", data->pressure->secAct);
    printf("Pressure Vessel: %f psi\n", data->pressure->pv);
}
