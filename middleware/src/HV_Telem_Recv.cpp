#include "HV_Telem_Recv.h"
#include "PracticalSocket.h"  
#include "document.h"
#include "data.h" 
#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include "connStat.h"

using namespace rapidjson;

pthread_t HVRecvThread;
extern data_t *data;

void SetupHVTelemRecv(){
	if (pthread_create(&HVRecvThread, NULL, HVTelemRecv, NULL)){
		fprintf(stderr, "Error creating LV Telemetry thread\n");
	}
}



void *HVTelemRecv(void *arg){
	(void) arg;
	
	int packetErrCounter = 0;
	uint64_t recentPacketID = 0;
	
	while(1){
		try {
			UDPSocket sock(HV_TELEM_RECV_PORT);                
		
			char recvString[MAX_TLM_HV_RECV + 1];
			string sourceAddress;
			unsigned short sourcePort; 
			int bytesRcvd = sock.recvFrom(recvString, MAX_TLM_HV_RECV, sourceAddress, sourcePort);
			recvString[bytesRcvd] = '\0'; 
			
			//cout << "Received " << recvString << " from " << sourceAddress << ": "
			//		 << sourcePort << endl;
					
					
			// Now to try and parse it
			Document document;
			document.Parse(recvString);
			
			// Get a counter
			if(document.HasMember("id")){
				// Make sure it's a new packet
				if(document["id"].GetUint64() != recentPacketID){
					recentPacketID = document["id"].GetUint64();
					
					// TODO complete as new sensors/etc are added
					// Parse data in
					
					// MOTION DATA
					const Value &motion = document["motion"];
					data->motion->vel = motion["velocity"].GetFloat();
					data->motion->accel = motion["acceleration"].GetFloat();
	                data->motion->retroCount = motion["retro"].GetInt();	
					printf("Updated vel to: %lf, accel to: %lf\n", data->motion->vel, data->motion->accel);	
		            data->motion->pos = motion["position"].GetFloat();
                    
                    const Value &b = document["braking"];
                    data->pressure->pv = b["pressureVesselPressure"].GetFloat();
                    data->pressure->primTank = b["primaryTank"].GetFloat();
                    data->pressure->primLine = b["primaryLine"].GetFloat();
                    data->pressure->primAct = b["primaryActuation"].GetFloat();

                    data->pressure->secTank = b["secondaryTank"].GetFloat();
                    data->pressure->secLine = b["secondaryLine"].GetFloat();
                    data->pressure->secAct =  b["secondaryActuation"].GetFloat();
                    data->flags->readyToBrake = (bool)b["readyToBrake"].GetInt();
				}
				else{
					fprintf(stderr, "Encountered total: %i dropped packets due to ID error\n", ++packetErrCounter);
				}
			
			}
		} 
		catch (SocketException &e) {
			cerr << e.what() << endl;
		}
	}
	
	return NULL;
}
