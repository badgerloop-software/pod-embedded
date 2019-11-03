#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>
#include <unistd.h>

#include "data_dump.h"
#include "data.h"

pthread_t dataDumpThread;

extern data_t *data;

/* Setup PThread Loop */
void SetupDataDump(){
	
	if (pthread_create(&dataDumpThread, NULL, DataLoop, NULL)){
		fprintf(stderr, "Error creating HV Telemetry thread\n");
	}
	 
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
void currentDateTime(char *buf) {
	time_t     now = time(0);
	struct tm  tstruct;
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d%X", &tstruct);
}


/* Thread Loop */
void *DataLoop(void *arg){

	(void) arg;
	
	FILE *fp;
	
	char dir[128];
	char timestamp[80];
	currentDateTime(timestamp);
	
	sprintf(dir, "../../data_logs/LOG.csv", timestamp);
	
	fp = fopen (dir,"w");
	if (fp == NULL){
		printf("Unable to open file\n");
		return NULL;
	}

	
	
	fprintf(fp, "FRP,FPD,FRC,FP,FEB,TST,TLR,TLR1,TLR2,TLR3,Pps1,Pps2,Pps3,Sps1,Sps2,Sps3,pv,pos,vel,accel,retC,pCurr,pVol,pDCL,pCCL,pRes,pHealth,pOV,pC,pAH,iV,Soc,hiT,loT,cMax,cMin,cAvg,mCells,nCells,igbtTemp,GDBT,CBT,mT,mS,paC,pbC,pcC,dcbV,lvV,cc1,cc2,fC1,fC2,cT,aT,rS,eF,dcbC,oVL\n");

	while(1){
		
		// FLAGS
		fprintf(fp, "%i,%i,%i,%i,%i,", data->flags->readyPump, data->flags->pumpDown, data->flags->readyCommand, data->flags->propulse, data->flags->emergencyBrake);
		
		// TIMERS
		fprintf(fp, "%ld,%ld,%ld,%ld,%ld,", data->timers->startTime, data->timers->lastRetro, data->timers->lastRetros[0], data->timers->lastRetros[1], data->timers->lastRetros[2]);
		
		// PRESSURE
		fprintf(fp, "%f,%f,%f,%f,%f,%f,%f,", data->pressure->primTank, data->pressure->primLine, data->pressure->primAct, data->pressure->secTank, data->pressure->secLine, data->pressure->secAct, data->pressure->pv);
		
		// MOTION
		fprintf(fp, "%f,%f,%f,%i,", data->motion->pos, data->motion->vel, data->motion->accel, data->motion->retroCount);
		
		// BMS
		fprintf(fp, "%f,%f,%hu,%hx,%hu,%hu,%f,%hu,%hu,%f,%hu,%hu,%hu,%hu,%f,%f,%hu,%hu,%hu,", data->bms->packCurrent, data->bms->packVoltage, data->bms->packDCL, data->bms->packCCL, data->bms->packResistance, data->bms->packHealth, data->bms->packOpenVoltage, data->bms->packCycles, data->bms->packAh, data->bms->inputVoltage, data->bms->Soc, data->bms->relayStatus, data->bms->highTemp, data->bms->lowTemp, data->bms->cellMaxVoltage, data->bms->cellMinVoltage, data->bms->cellAvgVoltage, data->bms->maxCells, data->bms->numCells);
		
		// RMS
		fprintf(fp, "%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%lu,%lu,%lu,%lu,%hu,%hu,%hu,%hu,%hu,%hu\n", data->rms->igbtTemp, data->rms->gateDriverBoardTemp, data->rms->controlBoardTemp, data->rms->motorTemp, data->rms->motorSpeed, data->rms->phaseACurrent, data->rms->phaseBCurrent, data->rms->phaseCCurrent, data->rms->dcBusVoltage, data->rms->lvVoltage, data->rms->canCode1, data->rms->canCode2, data->rms->faultCode1, data->rms->faultCode2, data->rms->commandedTorque, data->rms->actualTorque, data->rms->relayState, data->rms->electricalFreq, data->rms->dcBusCurrent, data->rms->outputVoltageLn);
		
		fflush(fp);
		
		usleep(30000);
	}
		
}
