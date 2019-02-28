#include "PracticalSocket.h"  
#include "document.h"
#include "data.h" 
#include <iostream>
#include <cstdlib>
#include <stdint.h>

using namespace rapidjson;

pthread_t HVRecvThread;
extern data_t data;

void SetupHVTelemRecv(){
	if (pthread_create(&HVRecvThread, NULL, HVTelemRecv, NULL)){
		fprintf(stderr, "Error creating LV Telemetry thread\n");
	}
}



void *HVTelemRecv(void *arg){
	(void) arg;
	
	uint64_t packetCounter = 0;
	
	while(1){
		try {
			UDPSocket sock(TLM_RECV_PORT);                
		
			char recvString[MAX_TLM_HV_RECV + 1];
			string sourceAddress;
			unsigned short sourcePort; 
			int bytesRcvd = sock.recvFrom(recvString, MAX_TLM_HV_RECV, sourceAddress, sourcePort);
			recvString[bytesRcvd] = '\0'; 
			
			cout << "Received " << recvString << " from " << sourceAddress << ": "
					 << sourcePort << endl;
					
					
			// Now to try and parse it
			Document document;
			document.Parse(recvString.c_str());
			
			// Get a counter
			if(!document.HasMember("hello"))
			
			
		} 
		catch (SocketException &e) {
			cerr << e.what() << endl;
		}
	}
}