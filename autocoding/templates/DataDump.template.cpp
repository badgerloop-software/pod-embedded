#include <unistd.h>
#include <stdio.h>
#include <string>
#include <pthread.h>

#include <iostream>
#include <fstream>

#include "DataDump.h"
#include "data.h"

pthread_t dataDumpThread;

extern data_t *data;

/* Setup PThread Loop */
void SetupDataDump(){
    if (pthread_create(&dataDumpThread, NULL, DataLoop, NULL))
        std::cerr << "Error creating data dump thread.\n";
}

// Get current date/time, format is YYYY-MM-DD - HH:mm:ss
void currentDateTime(char *buf, int bufferLength) {
    time_t     now = time(0);
    struct tm  tstruct;
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, bufferLength, "%Y-%m-%d - %X", &tstruct);
}


/* Thread Loop */
void *DataLoop(void *arg){

    (void) arg;

    char timestamp[50];
    currentDateTime(timestamp, 50);

    std::string filePath = "/home/daniel/Desktop/Badgerloop/master-aligned/pod-embedded/cmake-build-debug/../data_logs/";

    std::ofstream logFile(filePath.append("telemetryDump - ").append(timestamp).append(".log"));
    if(!logFile.is_open()){
        std::cerr << "Error opening log file.\n";
        return 0;
    }

    while(true){
        currentDateTime(timestamp, 50);
        logFile << "\n" << timestamp << ":\n";

        /**!!AUTO-GENERATE HERE!!**/

        logFile << "\n\n";

        logFile.flush();
        usleep(300000);
    }

}
