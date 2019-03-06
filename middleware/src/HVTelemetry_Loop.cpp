#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include "PracticalSocket.h"
#include "HVTelemetry_Loop.h"
#include "document.h"
#include "writer.h" 

#include "data.h"


using namespace rapidjson;
using namespace std;

pthread_t HVTelemThread;
extern data_t *data;

void SetupHVTelemetry(char* ip, int port){
	
	HVTelemArgs *args = (HVTelemArgs*) malloc(sizeof(HVTelemArgs));
	
	if(args == NULL){
		fprintf(stderr, "MALLOC ERROR\n");
		exit(1);
	}
		
	args->ipaddr = strdup(ip);
	args->port = port;
	
	if (pthread_create(&HVTelemThread, NULL, HVTelemetryLoop, args)){
		fprintf(stderr, "Error creating HV Telemetry thread\n");
	}
}


void *HVTelemetryLoop(void *arg){
	
	HVTelemArgs *sarg = (HVTelemArgs*) arg;
	
	uint64_t packetCount = 0;

	try {
		// Create socket
		UDPSocket sock;
		
		while(1){
		
			// Create document
			Document document;
			document.SetObject();
			
			
			/* SET DATA VALUES */

			// PACKET ID
			Value packet_id;
			packet_id.SetUint64(packetCount++);
						
			// PACK VOLTAGE
			Value packV;
			packV.SetFloat(data->bms->packVoltage);
			
			// PACK CURRENT
			Value packC;
			packC.SetFloat(data->bms->packCurrent);
			
			// PACK SOC
			Value packSOC;
			packSOC.SetUint(data->bms->Soc);
			
			// PACK AH
			Value packAH;
			packAH.SetUint(data->bms->packAh);
			
			// CELL MAX VOLTAGE
			Value cellMaxV;
			cellMaxV.SetUint(data->bms->cellMaxVoltage);
			
			// CELL MIN VOLTAGE
			Value cellMinV;
			cellMinV.SetUint(data->bms->cellMinVoltage);
			
			// SECONDARY TANK
			Value secondaryTank;
			secondaryTank.SetNull();
			
			// SECONDARY LINE
			Value secondaryLine;
			secondaryLine.SetNull();
			
			// SECONDARY ACTUATION
			Value secondaryActuation;
			secondaryActuation.SetNull();
			
			// PRIMARY TANK
			Value primaryTank;
			primaryTank.SetNull();
			
			// PRIMARY LINE
			Value primaryLine;
			primaryLine.SetNull();
			
			// PRIMARY ACTUATION
			Value primaryActuation;
			primaryActuation.SetNull();
			
			/* INSERT VALUES INTO JSON DOCUMENTS */
			
			document.AddMember("id", packet_id, document.GetAllocator());
			
			Document batteryDoc;
			batteryDoc.SetObject();
			batteryDoc.AddMember("packVoltage", packV, batteryDoc.GetAllocator());
			batteryDoc.AddMember("packCurrent", packC, batteryDoc.GetAllocator());
			batteryDoc.AddMember("packSOC", packSOC, batteryDoc.GetAllocator());
			batteryDoc.AddMember("packAH", packAH, batteryDoc.GetAllocator());
			batteryDoc.AddMember("cellMaxVoltage", cellMaxV, batteryDoc.GetAllocator());
			batteryDoc.AddMember("cellMinVoltage", cellMinV, batteryDoc.GetAllocator());
			
			Document brakingDoc;
			brakingDoc.SetObject();
			brakingDoc.AddMember("secondaryTank", secondaryTank, brakingDoc.GetAllocator());
			brakingDoc.AddMember("secondaryLine", secondaryLine, brakingDoc.GetAllocator());
			brakingDoc.AddMember("secondaryActuation", secondaryActuation, brakingDoc.GetAllocator());
			brakingDoc.AddMember("primaryTank", primaryTank, brakingDoc.GetAllocator());
			brakingDoc.AddMember("primaryLine", primaryLine, brakingDoc.GetAllocator());
			brakingDoc.AddMember("primaryActuation", primaryActuation, brakingDoc.GetAllocator());
			
			/* ADD DOCUMENTS TO MAIN JSON DOCUMENT */
			
			document.AddMember("battery", batteryDoc, document.GetAllocator());
			document.AddMember("braking", brakingDoc, document.GetAllocator());
			
			
			
			StringBuffer sb;
			Writer<StringBuffer> writer(sb); // PrettyWriter<StringBuffer> writer(sb); for debugging, don't forget to change header too
			document.Accept(writer);
			
			// Repeatedly send the string (not including \0) to the server
		
			sock.sendTo(sb.GetString(), strlen(sb.GetString()), sarg->ipaddr, sarg->port);
			usleep(30000);
		}
	} 
	catch (SocketException &e) {
		cerr << e.what() << endl;
		exit(1);
	}
}