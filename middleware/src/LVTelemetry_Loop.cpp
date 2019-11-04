#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include <stdint.h>
#include <chrono>
#include <ctime>
#include "PracticalSocket.h"
#include "LVTelemetry_Loop.h"
#include "document.h"
#include "writer.h"
#include "connStat.h"
#include "data.h"

/* ADD SENSOR INCLUDES HERE */
extern "C" 
{
    #include "lv_iox.h"
    #include "imu.h"
}


using namespace rapidjson;
using namespace std;

pthread_t LVTelemThread;

void SetupLVTelemetry(char* ip, int port){
	
	LVTelemArgs *args = (LVTelemArgs*) malloc(sizeof(LVTelemArgs));
	
	if(args == NULL){
		fprintf(stderr, "MALLOC ERROR\n");
		exit(1);
	}
	
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

			// PACKET ID
			Value packet_id;
			packet_id.SetUint64(packetCount++);
			
			// TIME
			Value age;
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()
			);
			age.SetUint64(ms.count());
			
			// STOPPING DISTANCE
			Value stopDistance;
			stopDistance.SetNull();
			
			// POSITION
			Value pos;
			//pos.SetFloat(getPosX());
            pos.SetFloat(0);

			// RETRO
			Value retro;
			retro.SetInt(data->motion->retroCount);
			
			// VELOCITY - Change "X" to "Y" if need be
			Value vel;
			//vel.SetFloat(getVelX());
            vel.SetFloat(0);
	
			Value lstRet;
            lstRet.SetUint64(data->timers->lastRetro);
			// ACCELERATION - Change "X" to "Y" if need be
			Value accel;
			accel.SetFloat(-1 * getAccelX());
            //accel.SetFloat(0);
				
			// PRESSURE VESSEL PRESSURE
			Value pressureV;
			pressureV.SetDouble(data->pressure->pv);
			
            Value primTank, primLine, primAct;
            primTank.SetFloat(data->pressure->primTank);
            primLine.SetFloat(data->pressure->primLine);
            primAct.SetFloat(data->pressure->primAct);
            
            Value secTank, secLine, secAct;
            secTank.SetFloat(data->pressure->secTank);
            secLine.SetFloat(data->pressure->secLine);
            secAct.SetFloat(data->pressure->secAct);
 
			// CURRENT PRESSURE
			Value currP;
			currP.SetNull();
			Value missedRetro;
            missedRetro.SetInt(data->motion->missedRetro);
            Value primBrake, secBrake;
            primBrake.SetInt(limSwitchGet(PRIM_LIM_SWITCH));
            secBrake.SetInt(limSwitchGet(SEC_LIM_SWITCH));
            Value readyFlag;
            readyFlag.SetInt(data->flags->readyToBrake);
			
			/* INSERT VALUES INTO JSON DOCUMENTS */
			
			document.AddMember("id", packet_id, document.GetAllocator());
			
			Document motionDoc;
			motionDoc.SetObject();
			motionDoc.AddMember("stoppingDistance", stopDistance, motionDoc.GetAllocator());
			motionDoc.AddMember("position", pos, motionDoc.GetAllocator());
			motionDoc.AddMember("retro", retro, motionDoc.GetAllocator());
			motionDoc.AddMember("velocity", vel, motionDoc.GetAllocator());
			motionDoc.AddMember("acceleration", accel, motionDoc.GetAllocator());
			motionDoc.AddMember("lastRetro", lstRet, motionDoc.GetAllocator());
			Document brakingDoc;
			brakingDoc.SetObject();
			brakingDoc.AddMember("pressureVesselPressure", pressureV, brakingDoc.GetAllocator());
			brakingDoc.AddMember("currentPressure", currP, brakingDoc.GetAllocator());
			brakingDoc.AddMember("primBrake", primBrake, brakingDoc.GetAllocator());
            brakingDoc.AddMember("secBrake", secBrake, brakingDoc.GetAllocator());
            brakingDoc.AddMember("primaryTank", primTank, brakingDoc.GetAllocator()); 
            brakingDoc.AddMember("primaryLine", primLine, brakingDoc.GetAllocator()); 
            brakingDoc.AddMember("primaryActuation", primAct, brakingDoc.GetAllocator()); 
            brakingDoc.AddMember("secondaryTank", secTank, brakingDoc.GetAllocator()); 
            brakingDoc.AddMember("secondaryLine", secLine, brakingDoc.GetAllocator()); 
            brakingDoc.AddMember("secondaryActuation", secAct, brakingDoc.GetAllocator()); 
            brakingDoc.AddMember("readyToBrake", readyFlag, brakingDoc.GetAllocator());
			/* ADD DOCUMENTS TO MAIN JSON DOCUMENT */
			
			document.AddMember("time", age, document.GetAllocator());
			document.AddMember("motion", motionDoc, document.GetAllocator());
			document.AddMember("braking", brakingDoc, document.GetAllocator());
			
			StringBuffer sb;
			Writer<StringBuffer> writer(sb); // PrettyWriter<StringBuffer> writer(sb); for debugging, don't forget to change header too
			document.Accept(writer);
			
			// Repeatedly send the string (not including \0) to the servers
		
			sock.sendTo(sb.GetString(), strlen(sb.GetString()), sarg->ipaddr, sarg->port);
			sock.sendTo(sb.GetString(), strlen(sb.GetString()), HV_SERVER_IP, HV_TELEM_RECV_PORT);
			usleep(30000);
		}
	} 
	catch (SocketException &e) {
		cerr << e.what() << endl;
		exit(1);
	}
}
