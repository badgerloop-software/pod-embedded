#include <iostream>
#include <termios.h>
#include <pthread.h>

extern "C" {
#include <uart.h>
}

void* ReadLoop(void *uartSettings) {
    auto *settings = (uart_settings*) uartSettings;

    // Reads and prints characters one at a time
    char* text = (char*) malloc(50);
    while(true) {
        text = 0;
        //Read a single character
        char character;
        read_uart(settings, &character, 1);

        //Write that character
        std::cout << character;
        std::cout.flush();
    }
}

int main(){
    uart_settings settings = {
            .baud = B9600,
            .file = 0,
            .serialPort = (char *) "/dev/ttyUSB1"
    };

    uart_begin(&settings);

    pthread_t readThread;
    pthread_create(&readThread, nullptr, ReadLoop, &settings);

    std::cout << "Started running test.";

    // Input loop
    std::string input;
    do {
        std::cin >> input;
        write_uart(&settings, (char *) input.c_str(), sizeof(input.c_str()));
    } while (input.size() > 0);

    pthread_cancel(readThread);

    std::cout << "Stopped running test.";

}