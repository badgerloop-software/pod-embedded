#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <chrono>
#include <pthread.h>
#include "PracticalSocket.h"
#include "LVTelemetry_Loop.h"
#include "document.h"     // rapidjson's DOM-style API
#include "prettywriter.h" // for stringify JSON

/* ADD SENSOR INCLUDES HERE */
#include "imu.h"

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
		
		while(1){
		
			// Create document
			Document document;
			document.SetObject();
			
			
			/* SET DATA VALUES */

			// TIME
			Value age;
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()
			);
			age.SetUint64(ms.count());
			
			// TYPE
			Value type;
			type.SetString("data");
			
			// STOPPING DISTANCE
			Value stopDistance;
			stopDistance.SetNull();
			
			// POSITION
			Value pos;
			pos.SetNull();
			
			// RETRO
			Value retro;
			retro.SetNull();
			
			// VELOCITY
			Value vel;
			vel.SetNull();
			
			// ACCELERATION
			Value accel;
			accel.SetNull();
				
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
			
			document.AddMember("age", age, document.GetAllocator());
			document.AddMember("type", type, document.GetAllocator());
			
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
			
			Document dataDoc;
			dataDoc.SetObject();
			
			/* ADD DOCUMENTS TO MAIN JSON DOCUMENT */
			
			dataDoc.AddMember("motion", motionDoc, dataDoc.GetAllocator());
			dataDoc.AddMember("battery", batteryDoc, dataDoc.GetAllocator());
			dataDoc.AddMember("braking", brakingDoc, dataDoc.GetAllocator());
			document.AddMember("data", dataDoc, document.GetAllocator());
			
			
			
			StringBuffer sb;
			PrettyWriter<StringBuffer> writer(sb);
			document.Accept(writer);    // Accept() traverses the DOM and generates Handler events.
			
			// Repeatedly send the string (not including \0) to the servers
		
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