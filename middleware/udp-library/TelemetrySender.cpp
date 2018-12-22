#include <iostream>
#include <unistd.h>
#include "PracticalSocket.h"
using namespace std;


int main(int argc, char *argv[]) {
//	string destAddress = argv[1];             // First arg:  destination address
//	unsigned short destPort = atoi(argv[2]);  // Second arg: destination port
//	char* sendString = argv[3];               // Third arg:  string to broadcast
	
	string destAddress = "127.0.0.1";
	int destPort = 33333;
	string sendString = "{\"age\": 1529644667834,\"data\":{\"motion\" : {\"stoppingDistance\": 1,\"position\": 2,\"retro\": 3.5,\"velocity\": 4.2553,\"acceleration\": 5.622},\"battery\": {\"packVoltage\": 6.24,\"packCurrent\": 7,\"packSOC\": 8,\"packAH\": 9,\"cellMaxVoltage\": 10,\"cellMinVoltage\": 11,\"highTemp\": 12,\"lowTemp\": 13},\"braking\": {\"secondaryTank\": 14,\"secondaryLine\": 15,\"secondaryActuation\": 16,\"primaryTank\": 17,\"primaryLine\": 18.8,\"primaryActuation\": 19,\"pressureVesselPressure\": 20.2383,\"currentPressure\": 21}}}";

	try {
		UDPSocket sock;
		
		// Repeatedly send the string (not including \0) to the server
		for (;;) {
			sock.sendTo(sendString.c_str(), strlen(sendString.c_str()), destAddress, destPort);
			usleep(300000);
			cout << "Sent" << endl;
		}
	} 
	catch (SocketException &e) {
		cerr << e.what() << endl;
		exit(1);
	}
}