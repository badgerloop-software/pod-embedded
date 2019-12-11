#ifndef PODEMBEDDED_UART_H
#define PODEMBEDDED_UART_H

#define SERIAL_PORT /dev/ttyS0

typedef struct {
    int baud;
    int file;
    char* serialPort;
} uart_settings;

int uart_begin(uart_settings *uart);
int write_uart(uart_settings *uart, char *writeBuffer, int bufferSize);
int read_uart(uart_settings *uart, char *readBuffer, int bufferSize);

#endif //PODEMBEDDED_UART_H
