#include "TCPServer.h"
#include <vector>
#include <iostream>
#include <string.h>
#include <future>
#include <PracticalSocket/PracticalSocket.h>

extern "C" {
    #include "data.h"
    #include "state_machine.h"
    #include "hv_iox.h"
    #include "braking.h"
    #include "motor.h"
    #include "rms.h"

    extern void resetNav();
    extern stateMachine_t stateMachine;
}

pthread_t TCPThread;

void SetupTCPServer() {
    if (pthread_create(&TCPThread, nullptr, TCPLoop, nullptr))
        fprintf(stderr, "Error creating TCP loop thread\n");
}

void *TCPLoop(void *arg) {

    TCPServerSocket serverSocket(TCP_PORT);

    while(1){
        TCPSocket* socket = serverSocket.accept();

        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, HandleConnection, (void *)socket))
            fprintf(stderr, "Error creating connection handler for socket\n");
    }
}

void *HandleConnection(void *arg) {
    // Detach the current thread to deallocate resources on return
    pthread_detach(pthread_self());

    auto* socket = static_cast<TCPSocket*>(arg);

    cout << "TCP connection CREATED with " << socket -> getLocalAddress() << ":" << socket -> getLocalPort() << ".\n";
    cout.flush();

    uint8_t* commandBuffer[MAX_COMMAND_SIZE];
    int commandSize;
    while(true){
        // Clear the commandBuffer
        memset(commandBuffer, 0, MAX_COMMAND_SIZE);

        // Future to allow for a timeout
        std::future<uint8_t*> futureCommand = std::async(std::launch::async, [&commandSize, &commandBuffer, socket]() {
            commandSize = socket -> recv((void *)commandBuffer, MAX_COMMAND_SIZE);
            return *commandBuffer;
        });

        std::future_status status;

        uint32_t timeoutCounter = 0;

        do {
            status = futureCommand.wait_for(std::chrono::milliseconds(FUTURE_PAUSE));
            timeoutCounter += FUTURE_PAUSE;

            if(timeoutCounter >= TIMEOUT_DURATION){
                socket -> send((char*) "Connection timed out.", 21);
                socket -> send((char*) "", 0);
                cout << "TCP connection TIMED OUT with " << socket -> getLocalAddress() << ":" << socket -> getLocalPort() << ".\n";
                cout.flush();
                goto CONNECTION_END;
            }

        } while (status != std::future_status::ready);

        // Process the contents of the command
        char* cmd = reinterpret_cast<char*>(commandBuffer);
        int cmdLength = strlen(cmd);

        #ifdef TCPSERVER_TEST
            // Trim off new line if there is one at the end
            if(cmd[cmdLength - 1] == '\n')
                cmdLength --;

            cout << "Command received: '" << std::string(cmd, cmdLength) << "'\n";
                cout.flush();
        #endif

        // Break the loop and return when the command is an empty string
        if(cmdLength == 0) {
            cout << "Zero character string sent.\n";
            break;
        }

        // Ignore keepalive signals
        if(strncmp(cmd, "keepalive", cmdLength) == 0)
            continue;

        /**
         ** POD COMMANDS GO HERE
        **/

        // Set the readyPump flag
        if (strncmp(cmd, "readyPump", cmdLength) == 0) {
            data->flags->readyPump = 1;
        }

        // Set the readyPump flag /****IF THIS SUPPOSED TO BE data->flags->readyPump = 0?****/
        if (strncmp(cmd, "pumpDown", cmdLength) == 0) {
            data->flags->readyPump = 1;
        }

        // Set the readyCommand flag
        if (strncmp(cmd, "readyCommand", cmdLength) == 0) {
            data->flags->readyCommand = 1;
        }

        // Set the propulse flag
        if (strncmp(cmd, "propulse", cmdLength) == 0) {
            data->flags->propulse = 1;
        }

        // Set the emergencyBrake flag
        if (strncmp(cmd, "emergencyBrake", cmdLength) == 0) {
            data->flags->emergencyBrake = 1;
        }

        // Turn on the MCU latch
        if (strncmp(cmd, "mcuLatchOn", cmdLength) == 0) {
            setMCULatch(true);
        }

        // Turn off the MCU latch
        if (strncmp(cmd, "mcuLatchOff", cmdLength) == 0) {
            setMCULatch(false);
        }

        // "enPrecharge"
        if (strncmp(cmd, "enPrecharge", cmdLength) == 0) {
            //pthread_create(&hbT, NULL, hbLoop, NULL);*/
            rmsEnHeartbeat();
            rmsClrFaults();
            rmsInvDis();
            //noTorqueMode = true;
        }

        // "cmdTorque"
        if (strncmp(cmd, "cmdTorque", cmdLength) == 0) {
            setMotorEn();
        }

        // Enable high voltage
        if (strncmp(cmd, "hvEnable", cmdLength) == 0) {
            /* Lets add a safety check here */
            setMCUHVEnabled(true);
        }

        // Disable high voltage
        if (strncmp(cmd, "hvDisable", cmdLength) == 0) {
            setMCUHVEnabled(false);
        }

        // This doens't do anything...
//        // Override the current state with the number which follows override
//        if (strncmp(cmd, "override", 8) == 0){
//            fprintf(stderr, "Override received for state: %s\n", cmd+9);
//            sprintf(stateMachine.overrideStateName, "%s%c", cmd+9, '\0');
//        }

        // Heartbeat so that the dashboard can know the TCP server is still active
        if (strncmp(cmd, "ping", cmdLength) == 0) {
            // Send acknowledge packet back
            socket -> send((char*) "pong1", 5);
        }

        /* Should we be concerned about this?
         *	if (strncmp(cmd, "power off", cmdLength) == 0) {
         *		// DO POWER OFF
         *	}
        */

        /* Is this any different than the override command?
         * if (strncmp(cmd, "state", 5) == 0) {
         *   *lastPacket[HV] = getuSTimestamp();
         *   data->state = cmd[5] - 0x30;
         * }
        */

        // Resets nav and sets the readyToBrake flag
        if (strncmp(cmd, "clrMotion", cmdLength) == 0) {
            resetNav();
            data->flags->readyToBrake = true;
        }

        // Enables the brakes on the pod
        if (strncmp(cmd, "brake", cmdLength) == 0) {
            //data->flags->shouldBrake = true;
            brake();
        }

        // Actuates the primary brakes
        if (strncmp(cmd, "primBrakeOn", cmdLength) == 0) {
            //data->flags->brakePrimAct = true;
            brakePrimaryActuate();
        }

        // Unactuates the primary brakes
        if (strncmp(cmd, "primBrakeOff", cmdLength) == 0) {
            //data->flags->brakePrimRetr = true;
            brakePrimaryUnactuate();
        }

        // Actuates the secondary brakes
        if (strncmp(cmd, "secBrakeOn", cmdLength) == 0) {
            //data->flags->brakeSecAct = true;
            brakeSecondaryActuate();
        }

        // Unactuates the secondary brakes
        if (strncmp(cmd, "secBrakeOff", cmdLength) == 0) {
            //data->flags->brakeSecRetr = true;
            brakeSecondaryUnactuate();
        }

    }

    cout << "TCP connection TERMINATED with " << socket -> getLocalAddress() << ":" << socket -> getLocalPort() << ".\n";
    cout.flush();

    // Label to break free from nested loop when the timeout is reached
    CONNECTION_END:

    // End connection
    delete socket;
    return nullptr;
}




