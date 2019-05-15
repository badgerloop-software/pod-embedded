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
char* currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d%X", &tstruct);

	return buf.c_str();
}


/* Thread Loop */
void *DataLoop(void *arg){
	
	(void) arg;
	
	FILE *fp;
	
	char dir[64];
	char* timestamp = currentDateTime();
	
	sprintf(dir, "logs/%s.csv", timestamp);
	
	fp = fopen (dir,"w");
	if (fp == NULL){
		printf("Unable to open file\n");
		return;
	}
	
	fprintf(fp, "")
	
	
	while(1){
		fp = fopen (dir,"w");
	}
		
}
