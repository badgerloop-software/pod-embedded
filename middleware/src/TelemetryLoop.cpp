/* 
 * Please note that this is an auto-generated file which is automatically generated whenever a target is built.
 */

#include "TelemetryLoop.h"

#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <pthread.h>
#include <stdint.h>
#include <memory.h>
#include <vector>
#include <bitset>
#include <PracticalSocket/PracticalSocket.h>
#include <CRCpp/CRC.h>
#include <chrono>

extern "C" {
#include <hv_iox.h>
#include <lv_iox.h>
#include <data.h>
}


#define BUFFER_SIZE 500
#define ENDIAN "LITTLE" // TODO: Is there a way to check this during compile-time or something?

// Because it kind of looks like "BADGER"
#define HEADER 0xBAD6E4



pthread_t telemetryThread;

template <typename type>
void addToBuffer(std::vector<uint8_t>* buffer, type value, uint8_t size = 0){
    // TODO: Find some way to send information about the CPU-dependent sizes to standardize things

    // If not explicitly stated, the number of bits is the size of the contents of the pointer
    int bytes = (size == 0) ? sizeof(value) : size;

    const uint8_t* byteArray = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&value));

    for(int i = 0; i < bytes; i++)
        if(ENDIAN == "BIG") // NOLINT(misc-redundant-expression)
            buffer->push_back(byteArray[(bytes - 1) - i]);
        else
            buffer->push_back(byteArray[i]);
}

void SetupTelemetry(char* ip, int port){
    TelemArgs *args = (TelemArgs*) malloc(sizeof(TelemArgs));

    // Ensure memory was actually allocated
    if(args == NULL){
        fprintf(stderr, "MALLOC ERROR\n");
        exit(1);
    }

    // Assign the telemetry arguments
    args -> ipaddr = strdup(ip);
    args -> port = port;

    // Create a thread for the telemetry loop to run in
    if (pthread_create(&telemetryThread, NULL, TelemetryLoop, args))
        fprintf(stderr, "Error creating LV Telemetry thread\n");
}

void* TelemetryLoop(void *arg) {
    // Cast the argument
    TelemArgs *sarg = (TelemArgs *) arg;

    try {
        // Create a UDP socket which telemetry packets are rapidly sent to
        UDPSocket sock;

        // Sent with each packet to keep track of ordering
        uint32_t packetNumber = 0;

        // Buffer to be sent by the socket
        std::vector<uint8_t> buffer;
        buffer.reserve(BUFFER_SIZE); // Prevents constant reallocation which can slow things down

        const uint32_t header = HEADER;

        while (true) {
            // Header: Write BAD6E4 (Because it kind of looks like "Badger")
            addToBuffer(&buffer, &header, 3);

            // Contents: Loop through the data and add everything in

            // Write 4 byte packet count
            addToBuffer(&buffer, &packetNumber);

            // Write 8 byte time
            uint64_t time = chrono::duration_cast<chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()
            ).count();
            addToBuffer(&buffer, &time);

            // Write 1 byte IMD status
            uint8_t imdStatus = hv_iox.getIMDStatus();
            addToBuffer(&buffer, &imdStatus);

            // Write 4 byte primary brake
            int32_t primBrake = 0;
            #ifndef TELEMETRY_LOOP_TEST
                primBrake = limSwitchGet(PRIM_LIM_SWITCH);
            #endif
            addToBuffer(&buffer, &primBrake);

            // Write 4 byte secondary brake
            int32_t secBrake = 0;
            #ifndef TELEMETRY_LOOP_TEST
                secBrake = limSwitchGet(SEC_LIM_SWITCH);
            #endif
            addToBuffer(&buffer, &secBrake);

            
            /* Autogenerated Code Begins */
            addToBuffer(&buffer, getDataState());
            addToBuffer(&buffer, getPressurePrimTank());
            addToBuffer(&buffer, getPressurePrimLine());
            addToBuffer(&buffer, getPressurePrimAct());
            addToBuffer(&buffer, getPressureSecTank());
            addToBuffer(&buffer, getPressureSecLine());
            addToBuffer(&buffer, getPressureSecAct());
            addToBuffer(&buffer, getPressureAmb());
            addToBuffer(&buffer, getPressurePv());
            addToBuffer(&buffer, getMotionPos());
            addToBuffer(&buffer, getMotionVel());
            addToBuffer(&buffer, getMotionAccel());
            addToBuffer(&buffer, getMotionRetroCount());
            addToBuffer(&buffer, getMotionMissedRetro());
            addToBuffer(&buffer, getBmsPackCurrent());
            addToBuffer(&buffer, getBmsPackVoltage());
            addToBuffer(&buffer, getBmsImdStatus());
            addToBuffer(&buffer, getBmsPackDCL());
            addToBuffer(&buffer, getBmsPackCCL());
            addToBuffer(&buffer, getBmsPackResistance());
            addToBuffer(&buffer, getBmsPackHealth());
            addToBuffer(&buffer, getBmsPackOpenVoltage());
            addToBuffer(&buffer, getBmsPackCycles());
            addToBuffer(&buffer, getBmsPackAh());
            addToBuffer(&buffer, getBmsInputVoltage());
            addToBuffer(&buffer, getBmsSoc());
            addToBuffer(&buffer, getBmsRelayStatus());
            addToBuffer(&buffer, getBmsHighTemp());
            addToBuffer(&buffer, getBmsLowTemp());
            addToBuffer(&buffer, getBmsAvgTemp());
            addToBuffer(&buffer, getBmsCellMaxVoltage());
            addToBuffer(&buffer, getBmsCellMinVoltage());
            addToBuffer(&buffer, getBmsCellAvgVoltage());
            addToBuffer(&buffer, getBmsMaxCells());
            addToBuffer(&buffer, getBmsNumCells());
            addToBuffer(&buffer, getRmsIgbtTemp());
            addToBuffer(&buffer, getRmsGateDriverBoardTemp());
            addToBuffer(&buffer, getRmsControlBoardTemp());
            addToBuffer(&buffer, getRmsMotorTemp());
            addToBuffer(&buffer, getRmsMotorSpeed());
            addToBuffer(&buffer, getRmsPhaseACurrent());
            addToBuffer(&buffer, getRmsPhaseBCurrent());
            addToBuffer(&buffer, getRmsPhaseCCurrent());
            addToBuffer(&buffer, getRmsDcBusVoltage());
            addToBuffer(&buffer, getRmsLvVoltage());
            addToBuffer(&buffer, getRmsCanCode1());
            addToBuffer(&buffer, getRmsCanCode2());
            addToBuffer(&buffer, getRmsFaultCode1());
            addToBuffer(&buffer, getRmsFaultCode2());
            addToBuffer(&buffer, getRmsCommandedTorque());
            addToBuffer(&buffer, getRmsActualTorque());
            addToBuffer(&buffer, getRmsRelayState());
            addToBuffer(&buffer, getRmsElectricalFreq());
            addToBuffer(&buffer, getRmsDcBusCurrent());
            addToBuffer(&buffer, getRmsOutputVoltageLn());
            addToBuffer(&buffer, getRmsVSMCode());
            addToBuffer(&buffer, getRmsKeyMode());
            addToBuffer(&buffer, getFlagsReadyPump());
            addToBuffer(&buffer, getFlagsPumpDown());
            addToBuffer(&buffer, getFlagsReadyCommand());
            addToBuffer(&buffer, getFlagsReadyToBrake());
            addToBuffer(&buffer, getFlagsPropulse());
            addToBuffer(&buffer, getFlagsEmergencyBrake());
            addToBuffer(&buffer, getFlagsShouldStop());
            addToBuffer(&buffer, getFlagsShutdown());
            addToBuffer(&buffer, getFlagsShouldBrake());
            addToBuffer(&buffer, getFlagsIsConnected());
            addToBuffer(&buffer, getFlagsBrakeInit());
            addToBuffer(&buffer, getFlagsBrakePrimAct());
            addToBuffer(&buffer, getFlagsBrakeSecAct());
            addToBuffer(&buffer, getFlagsBrakePrimRetr());
            addToBuffer(&buffer, getFlagsBrakeSecRetr());
            addToBuffer(&buffer, getFlagsClrMotionData());
            addToBuffer(&buffer, getTimersStartTime());
            addToBuffer(&buffer, getTimersOldRetro());
            addToBuffer(&buffer, getTimersLastRetro());
            for(int i = 0; i < 3; i++)
            	addToBuffer(&buffer, getTimersLastRetros(i));
            addToBuffer(&buffer, getTimersCrawlTimer());
            /* Autogenerated Code Ends */
            

            // Tail: Cyclic Redundancy Check (32 bit)
            uint32_t crc = CRC::Calculate(buffer.data(), buffer.size(), CRC::CRC_32());
            addToBuffer(&buffer, &crc);

            // Send data and reset buffer
            sock.sendTo(buffer.data(), buffer.size(), sarg->ipaddr, sarg->port);
            buffer.clear();

            packetNumber ++;

            #ifdef TELEMETRY_LOOP_TEST
                if(packetNumber % 100 == 0){
                    cout << packetNumber << " packets have been sent.\n";
                    cout.flush();
                }
            #endif

            // Pause for 30 milliseconds before sending the next packet
            usleep(30000);
        }
    } catch (SocketException &e) {
        cerr << e.what() << endl;
        exit(1);
    }
}
