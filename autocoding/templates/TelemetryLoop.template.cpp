#include "TelemetryLoop.h"

#include <CRCpp/CRC.h>
#include <PracticalSocket/PracticalSocket.h>
#include <bitset>
#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory.h>
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>
#include <vector>

#include <hv_iox.h>
extern "C" {
#include <lv_iox.h>
}

#define BUFFER_SIZE 500
#define ENDIAN "LITTLE" // TODO: Is there a way to check this during compile-time or something?

// Because it kind of looks like "BADGER"
#define HEADER 0xBAD6E4

pthread_t telemetryThread;

template <typename type>
void addToBuffer(std::vector<uint8_t>* buffer, type value, uint8_t size = 0)
{
    // TODO: Find some way to send information about the CPU-dependent sizes to standardize things

    // If not explicitly stated, the number of bits is the size of the contents of the pointer
    int bytes = (size == 0) ? sizeof(value) : size;

    const uint8_t* byteArray = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(&value));

    for (int i = 0; i < bytes; i++)
        if (ENDIAN == "BIG") // NOLINT(misc-redundant-expression)
            buffer->push_back(byteArray[(bytes - 1) - i]);
        else
            buffer->push_back(byteArray[i]);
}

void SetupTelemetry(char* ip, int port)
{
    TelemArgs* args = (TelemArgs*)malloc(sizeof(TelemArgs));

    // Ensure memory was actually allocated
    if (args == NULL) {
        fprintf(stderr, "MALLOC ERROR\n");
        exit(1);
    }

    // Assign the telemetry arguments
    args->ipaddr = strdup(ip);
    args->port = port;

    // Create a thread for the telemetry loop to run in
    if (pthread_create(&telemetryThread, NULL, TelemetryLoop, args))
        fprintf(stderr, "Error creating LV Telemetry thread\n");
}

void* TelemetryLoop(void* arg)
{
    // Cast the argument
    TelemArgs* sarg = (TelemArgs*)arg;

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
                std::chrono::system_clock::now().time_since_epoch())
                                .count();
            addToBuffer(&buffer, &time);

            // Write 1 byte IMD status
            uint8_t imdStatus = hv_iox.getIMDStatus();
            addToBuffer(&buffer, &imdStatus);

            // Write 4 byte primary brake
            int32_t primBrake = 0;
// #ifndef TELEMETRY_LOOP_TEST
//             primBrake = limSwitchGet(PRIM_LIM_SWITCH);
// #endif
            addToBuffer(&buffer, &primBrake);

            // Write 4 byte secondary brake
            int32_t secBrake = 0;
// #ifndef TELEMETRY_LOOP_TEST
//             secBrake = limSwitchGet(SEC_LIM_SWITCH);
// #endif
            addToBuffer(&buffer, &secBrake);

            /**!!AUTO-GENERATE HERE!!**/

            // Tail: Cyclic Redundancy Check (32 bit)
            uint32_t crc = CRC::Calculate(buffer.data(), buffer.size(), CRC::CRC_32());
            addToBuffer(&buffer, &crc);

            // Send data and reset buffer
            sock.sendTo(buffer.data(), buffer.size(), sarg->ipaddr, sarg->port);
            buffer.clear();

            packetNumber++;

#ifdef TELEMETRY_LOOP_TEST
            if (packetNumber % 100 == 0) {
                cout << packetNumber << " packets have been sent.\n";
                cout.flush();
            }
#endif

            // Pause for 30 milliseconds before sending the next packet
            usleep(30000);
        }
    } catch (SocketException& e) {
        cerr << e.what() << endl;
        exit(1);
    }
}
