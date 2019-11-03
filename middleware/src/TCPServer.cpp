#include "TCPServer.h"
#include <vector>
#include <iostream>
#include <string.h>
#include <cctype>
#include <future>
#include <algorithm>
#include <PracticalSocket/PracticalSocket.h>

pthread_t TCPThread;

void SetupTCPServer() {
    if (pthread_create(&TCPThread, NULL, TCPLoop, NULL))
        fprintf(stderr, "Error creating TCP loop thread\n");
}

void *TCPLoop(void *arg) {

    TCPServerSocket serverSocket(TCP_PORT);

    while(true){
        TCPSocket* socket = serverSocket.accept();

        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, HandleConnection, (void *)socket))
            fprintf(stderr, "Error creating connection handler for socket\n");
    }
}

void *HandleConnection(void *arg) {
    // Detach the current thread to deallocate resources on return
    pthread_detach(pthread_self());

    TCPSocket* socket = static_cast<TCPSocket*>(arg);

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

    }

    cout << "TCP connection TERMINATED with " << socket -> getLocalAddress() << ":" << socket -> getLocalPort() << ".\n";
    cout.flush();

    // Label to break free from nested loop when the timeout is reached
    CONNECTION_END:

    // End connection
    delete socket;
    return NULL;
}




