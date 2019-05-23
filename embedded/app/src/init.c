#include <stdio.h>
#include <stdlib.h>
#include <data.h>

extern data_t *data;

int initData() {
    if (initMetaData() != 0) {
        fprintf(stderr, "Failed to init meta data\n");
        return -1;
    }
    if (initPressureData() != 0) {
        fprintf(stderr, "Failed to init pressure data\n");
        return -1;
    }
    if (initMotionData() != 0) {
        fprintf(stderr, "Failed to init motion data\n");
        return -1;
    }
    if (initBmsData() != 0) {
        fprintf(stderr, "Failed to init BMS data\n");
        return -1;
    }
    if (initRmsData() != 0) {
        fprintf(stderr, "Failed to init RMS data\n");
        return -1;
    }
    if (initFlagData() != 0) {
        fprintf(stderr, "Failed to init flag data\n");
        return -1;
    }
    if (initTimerData() != 0) {
        fprintf(stderr, "Failed to timer data\n");
        return -1;
    }
    return 0;
}

int initMetaData() {
    if ((data = (data_t *) malloc(sizeof(data_t)))                      == NULL) { return 1; }
	if ((data->pressure = (pressure_t *) malloc(sizeof(pressure_t)))    == NULL) { return 1; }
	if ((data->motion = (motion_t *)malloc(sizeof(motion_t)))           == NULL) { return 1; }
	if ((data->bms = (bms_t *) malloc(sizeof(bms_t)))     	          	== NULL) { return 1; }
	if ((data->rms = (rms_t *) malloc(sizeof(rms_t)))      		       	== NULL) { return 1; }
	if ((data->flags = (flags_t *) malloc(sizeof(flags_t)))             == NULL) { return 1; }
	if ((data->timers = (timers_t *) malloc(sizeof(timers_t)))          == NULL) { return 1; }
    data->state = 0;
    return 0;
}

int initPressureData() {
    data->pressure->ps1 = 0;
	data->pressure->ps2 = 0;
	data->pressure->ps3 = 0;
	data->pressure->ps4 = 0;
	data->pressure->sec_ps1 = 0;
	data->pressure->sec_ps2 = 0;
	data->pressure->sec_ps3 = 0;
	data->pressure->sec_ps4 = 0;
	data->pressure->pv = 0;
    return 0;
}

int initMotionData() {
	data->motion->pos = 0;
	data->motion->vel = 0;
	data->motion->accel = 0;
	data->motion->retroCount = 0;
    return 0;
}

int initBmsData() {
	data->bms->packCurrent = 0;
	data->bms->packVoltage = 0;
	data->bms->packDCL = 0;
	data->bms->packCCL = 0;
	data->bms->packResistance = 0;
	data->bms->packHealth = 0;
	data->bms->packOpenVoltage = 0;
	data->bms->packCycles = 0;
	data->bms->packAh = 0;
	data->bms->inputVoltage = 0;
	data->bms->Soc = 0;
	data->bms->relayStatus = 0;
	data->bms->highTemp = 0;
	data->bms->lowTemp = 0;
	data->bms->cellMaxVoltage = 0;
	data->bms->cellMinVoltage = 0;
	data->bms->cellAvgVoltage = 0;
	data->bms->maxCells = 0;
	data->bms->numCells = 0;
    return 0;
}

int initRmsData() {
	data->rms->igbtTemp = 0;
	data->rms->gateDriverBoardTemp = 0;
	data->rms->controlBoardTemp = 0;
	data->rms->motorTemp = 0;
	data->rms->motorSpeed = 0;
	data->rms->phaseACurrent = 0;
	data->rms->phaseBCurrent = 0;
	data->rms->phaseCCurrent = 0;
	data->rms->dcBusVoltage = 0;
	//data->rms->output_voltage_peak = 0;
	data->rms->lvVoltage = 0;
	data->rms->canCode1 = 0;
	data->rms->canCode2 = 0;
	data->rms->faultCode1 = 0;
	data->rms->faultCode2 = 0;
	data->rms->commandedTorque = 0;
	data->rms->actualTorque = 0;
	data->rms->relayState = 0;
	data->rms->electricalFreq = 0;
	data->rms->dcBusCurrent = 0;
	data->rms->outputVoltageLn = 0;
    data->rms->VSMCode = 0;
	return 0;
}

int initFlagData() {
	// Init flags values to 0
	data->flags->readyPump = 0;
	data->flags->pumpDown = 0;
	data->flags->readyCommand = 0;
	data->flags->propulse = 0;
	data->flags->emergencyBrake = 0;
	return 0;
}

int initTimerData() {
    int i = 0;
    data->timers->startTime = 0;
    data->timers->lastRetro = 0;
    for (i = 0; i < NUM_RETROS; i++) data->timers->lastRetros[i] = 0;
    return 0;
}
