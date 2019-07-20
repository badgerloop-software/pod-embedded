#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include <chrono>
#include <ctime>
#include "PracticalSocket.h"
#include "HVTelemetry_Loop.h"
#include "document.h"
#include "writer.h"

#include "data.h"
#include "connStat.h"

extern "C" {
#include "bms.h"
    extern float* getCellArray();
    extern float cells[72];
    extern double getLVBattVoltage();
    extern double getLVCurrent();
}

using namespace rapidjson;
using namespace std;

		// Create socket
UDPSocket sock;

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
			
			// CURRENT STATE
			Value state;
			state.SetUint(data->state);
						
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
			cellMaxV.SetFloat(data->bms->cellMaxVoltage);
			
			// CELL MIN VOLTAGE
			Value cellMinV;
			cellMinV.SetFloat(data->bms->cellMinVoltage);
			
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
			
            Value maxCellTemp;
            maxCellTemp.SetUint(data->bms->highTemp);
            
            Value minCellTemp;
            minCellTemp.SetUint(data->bms->lowTemp);

            Value avgCellTemp;
            avgCellTemp.SetUint(data->bms->avgTemp);

            Value igbtT;
            igbtT.SetInt(data->rms->igbtTemp);

            Value gateDrvTemp;
            gateDrvTemp.SetInt(data->rms->gateDriverBoardTemp);

            Value cntrlBoardTemp;
            cntrlBoardTemp.SetUint(data->rms->controlBoardTemp);

            Value motorTemp;
            motorTemp.SetUint(data->rms->motorTemp);

            Value motorSpeed;
            motorSpeed.SetInt(data->rms->motorSpeed);

            Value current;
            current.SetInt(getLVCurrent());

            Value phaseACurrent;
            phaseACurrent.SetInt(data->rms->phaseACurrent);

            Value busCurrent;
            busCurrent.SetInt(data->rms->dcBusCurrent);

            Value busV;
            busV.SetInt(data->rms->dcBusVoltage);

            Value cmdT;
            cmdT.SetInt(data->rms->commandedTorque);

            Value torqueFdbk;
            torqueFdbk.SetInt(data->rms->actualTorque);

            Value lvVolt;
            lvVolt.SetDouble(getLVBattVoltage());

			/* INSERT VALUES INTO JSON DOCUMENTS */
			
			document.AddMember("id", packet_id, document.GetAllocator());
			
			Document batteryDoc;
			batteryDoc.SetObject();
/*            Value battCells;*/
/*            battCells.SetArray();*/
/*            for (int i =0; i < 72; i++)*/
/*                    battCells.PushBack((Value)cells[i], batteryDoc.GetAllocator()); */
			batteryDoc.AddMember("packVoltage", packV, batteryDoc.GetAllocator());
			batteryDoc.AddMember("packCurrent", packC, batteryDoc.GetAllocator());
			batteryDoc.AddMember("packSOC", packSOC, batteryDoc.GetAllocator());
			batteryDoc.AddMember("packAH", packAH, batteryDoc.GetAllocator());
			batteryDoc.AddMember("cellMaxVoltage", cellMaxV, batteryDoc.GetAllocator());
			batteryDoc.AddMember("cellMinVoltage", cellMinV, batteryDoc.GetAllocator());
/*		    batteryDoc.AddMember("cells", battCells,
 *		    batteryDoc.GetAllocator());*/
            /**/
            batteryDoc.AddMember("lvCurrent", current, batteryDoc.GetAllocator());
            batteryDoc.AddMember("maxCellTemp", maxCellTemp, batteryDoc.GetAllocator());
            batteryDoc.AddMember("minCellTemp", minCellTemp, batteryDoc.GetAllocator());
            batteryDoc.AddMember("avgCellTemp", avgCellTemp, batteryDoc.GetAllocator());
            batteryDoc.AddMember("lvVoltage", lvVolt, batteryDoc.GetAllocator());
/*			brakingDoc.AddMember("secondaryTank", secondaryTank, brakingDoc.GetAllocator());*/
/*			brakingDoc.AddMember("secondaryLine", secondaryLine, brakingDoc.GetAllocator());*/
/*			brakingDoc.AddMember("secondaryActuation", secondaryActuation, brakingDoc.GetAllocator());*/
/*			brakingDoc.AddMember("primaryTank", primaryTank, brakingDoc.GetAllocator());*/
/*			brakingDoc.AddMember("primaryLine", primaryLine, brakingDoc.GetAllocator());*/
/*			brakingDoc.AddMember("primaryActuation", primaryActuation, brakingDoc.GetAllocator());*/
            Document motorDoc;
            motorDoc.SetObject();
            motorDoc.AddMember("phaseAIGBTTemp", igbtT, motorDoc.GetAllocator());
            motorDoc.AddMember("gateDriverBoardTemp", gateDrvTemp, motorDoc.GetAllocator());
            motorDoc.AddMember("controlBoardTemp", cntrlBoardTemp, motorDoc.GetAllocator());
            motorDoc.AddMember("motorTemp", motorTemp, motorDoc.GetAllocator());
            motorDoc.AddMember("motorSpeed", motorSpeed, motorDoc.GetAllocator());
            motorDoc.AddMember("phaseACurrent", phaseACurrent, motorDoc.GetAllocator());
            motorDoc.AddMember("busCurrent", busCurrent, motorDoc.GetAllocator());
            motorDoc.AddMember("busVoltage", busV, motorDoc.GetAllocator());
            motorDoc.AddMember("commandTorque", cmdT, motorDoc.GetAllocator());
            motorDoc.AddMember("torqueFeedback", torqueFdbk, motorDoc.GetAllocator());
			
            /* ADD DOCUMENTS TO MAIN JSON DOCUMENT */
			document.AddMember("motor", motorDoc, document.GetAllocator());
            document.AddMember("time", age, document.GetAllocator());
			document.AddMember("battery", batteryDoc, document.GetAllocator());
			document.AddMember("state", state, document.GetAllocator());
			StringBuffer sb;
			Writer<StringBuffer> writer(sb); // PrettyWriter<StringBuffer> writer(sb); for debugging, don't forget to change header too
			document.Accept(writer);
			
			// Repeatedly send the string (not including \0) to the server
		
			sock.sendTo(sb.GetString(), strlen(sb.GetString()), sarg->ipaddr, sarg->port);
//			printf("Sent string: %s\n", sb.GetString());
			document.GetAllocator().Clear();
            document = NULL;
            usleep(30000);
		}
	}
	catch (SocketException &e) {
		printf("what happened?\n");
        cerr << e.what() << endl;
		exit(1);
	}
}

void sendLVCommand(char *command) {
    sock.sendTo(command, strlen(command), LV_SERVER_IP, LV_SERVER_PORT);
}
