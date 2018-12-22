#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <chrono>
#include <pthread.h>
#include "PracticalSocket.h"
#include "TelemetrySender.h"
#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

/* ADD SENSOR INCLUDES HERE */
#include "../../../embedded/xsens_imu/src/imu.h"

using namespace rapidjson;
using namespace std;

pthread_t TelemThread;

void SetupTelemetry(){
	if (pthread_create(&TelemThread, NULL, TelemetryLoop, NULL)){
		fprintf(stderr, "Error creating Telemetry thread\n");
	}
}


void *TelemetryLoop(){

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
			document.AddMember("age", age, document.GetAllocator());
			
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
			
			// PACK VOLTAGE
			Value packV;
			packV.SetNull();
			
			// PACK CURRENT
			Value packC;
			packC.SetNull();
			
			// PACK SOC
			Value packSOC;
			packSOC.SetNull();
			
			// PACK AH
			Value packAH;
			packAH.SetNull();
			
			// CELL MAX VOLTAGE
			Value cellMaxV;
			cellMaxV.SetNull();
			
			// CELL MIN VOLTAGE
			Value cellMinV;
			cellMaxV.SetNull();
			
			// HIGH TEMP
			Value tempH;
			tempH.SetNull();
			
			// LOW TEMP
			Value tempL;
			tempL.SetNull();
			
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
			
			// PRESSURE VESSEL PRESSURE
			Value pressureV;
			pressureV.SetNull();
			
			// CURRENT PRESSURE
			Value currP;
			currP.SetNull();
			
			
			/* INSERT VALUES INTO JSON DOCUMENTS */
			
			Document motionDoc;
			motionDoc.SetObject();
			motionDoc.AddMember("stoppingDistance", stopDistance, motionDoc.GetAllocator());
			motionDoc.AddMember("position", pos, motionDoc.GetAllocator());
			motionDoc.AddMember("retro", retro, motionDoc.GetAllocator());
			motionDoc.AddMember("velocity", vel, motionDoc.GetAllocator());
			motionDoc.AddMember("acceleration", accel, motionDoc.GetAllocator());
			
			Document batteryDoc;
			batteryDoc.SetObject();
			batteryDoc.AddMember("packVoltage", packV, batteryDoc.GetAllocator());
			batteryDoc.AddMember("packCurrent", packC, batteryDoc.GetAllocator());
			batteryDoc.AddMember("packSOC", packSOC, batteryDoc.GetAllocator());
			batteryDoc.AddMember("packAH", packAH, batteryDoc.GetAllocator());
			batteryDoc.AddMember("cellMaxVoltage", cellMaxV, batteryDoc.GetAllocator());
			batteryDoc.AddMember("cellMinVoltage", cellMinV, batteryDoc.GetAllocator());
			batteryDoc.AddMember("highTemp", tempH, batteryDoc.GetAllocator());
			batteryDoc.AddMember("lowTemp", tempL, batteryDoc.GetAllocator());
			
			Document brakingDoc;
			brakingDoc.SetObject();
			brakingDoc.AddMember("secondaryTank", secondaryTank, brakingDoc.GetAllocator());
			brakingDoc.AddMember("secondaryLine", secondaryLine, brakingDoc.GetAllocator());
			brakingDoc.AddMember("secondaryActuation", secondaryActuation, brakingDoc.GetAllocator());
			brakingDoc.AddMember("primaryTank", primaryTank, brakingDoc.GetAllocator());
			brakingDoc.AddMember("primaryLine", primaryLine, brakingDoc.GetAllocator());
			brakingDoc.AddMember("primaryActuation", primaryActuation, brakingDoc.GetAllocator());
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
			
			// Repeatedly send the string (not including \0) to the server
		
			sock.sendTo(sb.GetString(), strlen(sb.GetString()), IPADDR, PORT);
			usleep(3000);
		}
	} 
	catch (SocketException &e) {
		cerr << e.what() << endl;
		exit(1);
	}
}