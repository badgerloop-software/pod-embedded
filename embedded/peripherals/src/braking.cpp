
#include <braking.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

extern "C" {
    #include <data.h>
}

#define VOLTAGE_2000_SCALING(x) ((((((x / 256.0) * 5.0) - 0.5) / 4.0) * 2000.0))
#define CURRENT_500_SCALING(x) (((((x / 256.0) * 5.0) - 0.6) / 2.4) * 500.0)
#define CURRENT_50_SCALING(x) (((((x / 256.0) * 5.0) - 0.6) / 2.4) * 50.0)

#define RING_SIZE 200
#define LOOP_PERIOD 20000

#define BAUDRATE 9600
#define UART_TIMEOUT_MS 5000

static double avgDouble(double* arr, int size);
static pthread_t presMonThread;

buart nucleo;
BPacket cmd_ack = BPacket(BPacket::ACK);
BPacket cmd_ackack = BPacket(BPacket::ACKACK);
BPacket cmd_brake = BPacket(BPacket::BRAKE);
BPacket cmd_unbrake = BPacket(BPacket::UNBRAKE);



sem_t bigSem;

int initComms()
{
    printf("init coms.\n");
    char buff[3];


    nucleo.openDevice(4, BAUDRATE);
    nucleo.writeCommand(&cmd_ack);
    nucleo.readString(buff, 3, UART_TIMEOUT_MS);


    if(buff[2] != 'k') {
        printf("WRONG CHAR READ: should have read 'k' but read '%c'\n", buff[2]);
        return 1;
    }
    printf("comms init\n");
    return 0;
}

static double avgDouble(double* arr, int size)
{
    int i = 0;
    double sum = 0;
    for (i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum / (double)size;
}

int brake()
{
    char buff[3];
    nucleo.writeCommand(&cmd_brake);
    nucleo.readString(buff, 3, UART_TIMEOUT_MS);
    if (buff[2] != 'a') {
        printf("WRONG CHAR READ: should have read 'a' but read '%c'\n", buff[2]);
        return 1;
    }
    nucleo.flushReceiver();
    return 0;

}

int brakePrimaryActuate(){
    brake();
}

int brakeSecondaryActuate(){
    brake();
}



int unbrake()
{
    char buff[3];
    nucleo.writeCommand(&cmd_unbrake);
    nucleo.readString(buff, 3, UART_TIMEOUT_MS);
    
    if (buff[2] != 'a') {
        printf("WRONG CHAR READ: should have read 'a' but read '%c'\n", buff[2]);
        return 1;
    }
    return 0;
}

int brakePrimaryUnactuate(){
    unbrake();
}

int brakeSecondaryUnactuate(){
    unbrake();
}

