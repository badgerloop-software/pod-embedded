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
		fprintf(fp, "%i,%i,%i,%i,%i,", getFlagsReadyPump(), getFlagsPumpDown(), getFlagsReadyCommand(), getFlagsPropulse(), getFlagsEmergencyBrake());
		
		// TIMERS
		fprintf(fp, "%ld,%ld,%ld,%ld,%ld,", getTimersStartTime(), getTimersLastRetro(), getTimersLastRetros(0), getTimersLastRetros(1), getTimersLastRetros(2));
		
		// PRESSURE
		fprintf(fp, "%f,%f,%f,%f,%f,%f,%f,", getPressurePrimTank(), getPressurePrimLine(), getPressurePrimAct(), getPressureSecTank(), getPressureSecLine(), getPressureSecAct(), getPressurePv());
		
		// MOTION
		fprintf(fp, "%f,%f,%f,%i,", getMotionPos(), getMotionVel(), getMotionAccel(), getMotionRetroCount());
		
		// BMS
		fprintf(fp, "%f,%f,%hu,%hx,%hu,%hu,%f,%hu,%hu,%f,%hu,%hu,%hu,%hu,%f,%f,%hu,%hu,%hu,", getBmsPackCurrent(), getBmsPackVoltage(), getBmsPackDCL(), getBmsPackCCL(), getBmsPackResistance(), getBmsPackHealth(), getBmsPackOpenVoltage(), getBmsPackCycles(), getBmsPackAh(), getBmsInputVoltage(), getBmsSoc(), getBmsRelayStatus(), getBmsHighTemp(), getBmsLowTemp(), getBmsCellMaxVoltage(), getBmsCellMinVoltage(), getBmsCellAvgVoltage(), getBmsMaxCells(), getBmsNumCells());
		
		// RMS
		fprintf(fp, "%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%hu,%lu,%lu,%lu,%lu,%hu,%hu,%hu,%hu,%hu,%hu\n", getRmsIgbtTemp(), getRmsGateDriverBoardTemp(), getRmsControlBoardTemp(), getRmsMotorTemp(), getRmsMotorSpeed(), getRmsPhaseACurrent(), getRmsPhaseBCurrent(), getRmsPhaseCCurrent(), getRmsDcBusVoltage(), getRmsLvVoltage(), getRmsCanCode1(), getRmsCanCode2(), getRmsFaultCode1(), getRmsFaultCode2(), getRmsCommandedTorque(), getRmsActualTorque(), getRmsRelayState(), getRmsElectricalFreq(), getRmsDcBusCurrent(), getRmsOutputVoltageLn());
		
		fflush(fp);
		
		usleep(30000);
	}
		
}
