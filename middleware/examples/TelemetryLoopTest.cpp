#include <iostream>
#include "TelemetryLoop.h"
#include <hv_iox.h>

#define LOCALHOST (char *) "0.0.0.0"

extern "C" {
    #include "data.h"
}

HVIox hv_iox;

int main() {

    initData();

    std::cout << "Enter some text to stop sending packets..." << std::endl;

    SetupTelemetry(LOCALHOST, 3000);

    // Keeps the main thread alive until the user enters some text
    std::cin.ignore();
    return 0;
}