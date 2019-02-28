#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include <stdint.h>
#include "PracticalSocket.h"
#include "LVTelemetry_Loop.h"
#include "document.h"     // rapidjson's DOM-style API
#include "prettywriter.h" // for stringify JSON

/* ADD SENSOR INCLUDES HERE */
extern "C" 
{
	#include "imu.h"
}


using namespace rapidjson;
using namespace std;

pthread_t LVTelemThread;

void SetupLVTelemetry(char* ip, int port){
	
	LVTelemArgs *args = (LVTelemArgs*) malloc(sizeof(LVTelemArgs));
	
	args->ipaddr = strdup(ip);
	args->port = port;
	
	if (pthread_create(&LVTelemThread, NULL, LVTelemetryLoop, args)){
		fprintf(stderr, "Error creating LV Telemetry thread\n");
	}
}


void *LVTelemetryLoop(void *arg)
{
	LVTelemArgs *sarg = (LVTelemArgs*) arg;
	try {
		// Create socket
		UDPSocket sock;
		
		uint64_t packetCount = 0;
		
		while(1){
		
			// Create document
			Document document;
			document.SetObject();
			
			
			/* SET DATA VALUES */

			// TIME
			Value packet_id;
			packet_id.SetUint64(packetCount++);
			
			// STOPPING DISTANCE
			Value stopDistance;
			stopDistance.SetNull();
			
			// POSITION
			Value pos;
			pos.SetNull();
			
			// RETRO
			Value retro;
			retro.SetNull();
			
			// VELOCITY - Change "X" to "Y" if need be
			Value vel;
			vel.SetFloat(getDeltaVX());
			
			// ACCELERATION - Change "X" to "Y" if need be
			Value accel;
			accel.SetFloat(getAccelX());
				
			// HIGH TEMP
			Value tempH;
			tempH.SetNull();
			
			// LOW TEMP
			Value tempL;
			tempL.SetNull();
						
			// PRESSURE VESSEL PRESSURE
			Value pressureV;
			pressureV.SetNull();
			
			// CURRENT PRESSURE
			Value currP;
			currP.SetNull();
			
			
			/* INSERT VALUES INTO JSON DOCUMENTS */
			
			document.AddMember("id", packet_id, document.GetAllocator());
			
			Document motionDoc;
			motionDoc.SetObject();
			motionDoc.AddMember("stoppingDistance", stopDistance, motionDoc.GetAllocator());
			motionDoc.AddMember("position", pos, motionDoc.GetAllocator());
			motionDoc.AddMember("retro", retro, motionDoc.GetAllocator());
			motionDoc.AddMember("velocity", vel, motionDoc.GetAllocator());
			motionDoc.AddMember("acceleration", accel, motionDoc.GetAllocator());
			
			Document batteryDoc;
			batteryDoc.SetObject();
			batteryDoc.AddMember("highTemp", tempH, batteryDoc.GetAllocator());
			batteryDoc.AddMember("lowTemp", tempL, batteryDoc.GetAllocator());
			
			Document brakingDoc;
			brakingDoc.SetObject();
			brakingDoc.AddMember("pressureVesselPressure", pressureV, brakingDoc.GetAllocator());
			brakingDoc.AddMember("currentPressure", currP, brakingDoc.GetAllocator());
			
			/* ADD DOCUMENTS TO MAIN JSON DOCUMENT */
			
			document.AddMember("motion", motionDoc, document.GetAllocator());
			document.AddMember("battery", batteryDoc, document.GetAllocator());
			document.AddMember("braking", brakingDoc, document.GetAllocator());
			
			StringBuffer sb;
			Writer<StringBuffer> writer(sb); // PrettyWriter<StringBuffer> writer(sb); for debugging
			document.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
			
			// Repeatedly send the string (not including \0) to the servers
			
			if(strlen(sb.GetString()) >= 4096){
				fprintf(stderr, "STRING SIZE EXCEEDS 4096. SIZE: %i", strlen(sb.GetString()));
				return;
			}
		
			sock.sendTo(sb.GetString(), strlen(sb.GetString()), sarg->ipaddr, sarg->port);
			sock.sendTo(sb.GetString(), strlen(sb.GetString()), HV_SERVER_IP, HV_SERVER_PORT);
			usleep(30000);
		}
	} 
	catch (SocketException &e) {
		cerr << e.what() << endl;
		exit(1);
	}
}