#include "TelemetryLoop.h"
#include <iox_rick.h>
#include <iox_morty.h>
#include <iostream>

#define LOCALHOST (char*)"0.0.0.0"

extern "C"
{
#include "data.h"
}

iox_rick ioxRick;
iox_morty ioxMorty;

int main()
{

    initData();

    std::cout << "Enter some text to stop sending packets..." << std::endl;

    SetupTelemetry(LOCALHOST, 3000);

    // Keeps the main thread alive until the user enters some text
    std::cin.ignore();
    return 0;
}
