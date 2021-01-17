#include "LVTCPSocket.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    SetupLVTCPServer();

    while (1) {
        usleep(1000000);
    }
}