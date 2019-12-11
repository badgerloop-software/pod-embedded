#include <uart.h>
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int uart_begin(uart_settings *uart){
    if ((uart -> file = open(uart -> serialPort, O_RDWR)) < 0) {
        printf("Error - Could not open UART file\n");
        return 0;
    }

    struct termios attribs;
    if (tcgetattr(uart -> file, &attribs) < 0) {
        printf("Error - Problem getting UART file attributes\n");
        return 0;
    }

    if(cfsetispeed(&attribs, uart -> baud) < 0) {
        perror("Error - Invalid UART Baud Rate. Please use predefined values found in termios.h.");
        return 0;
    }

    attribs.c_cflag = uart -> baud | CS8 | CREAD | CLOCAL;
    attribs.c_iflag = IGNPAR | ICRNL;
    tcflush(uart -> file, TCIFLUSH);

    if (tcsetattr(uart -> file, TCSANOW, &attribs) < 0) {
        printf("Error - Problem setting UART file attributes\n");
        return 0;
    }

    return 1;
}

int uart_close(uart_settings *uart){
    return close(uart -> file);
}

int write_uart(uart_settings *uart, char *writeBuffer, int bufferSize){
    return write(uart -> file, writeBuffer, bufferSize);
}

int read_uart(uart_settings *uart, char *readBuffer, int bufferSize){
    return read(uart -> file, readBuffer, bufferSize);
}