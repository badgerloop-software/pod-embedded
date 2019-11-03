#include <iostream>
#include "TCPServer.h"

int main() {

    SetupTCPServer();

    std::cout << "TCP server running on port " << TCP_PORT << "\n";

    // Keeps the main thread alive until the user enters some text
    std::cin.ignore();
    return 0;
}