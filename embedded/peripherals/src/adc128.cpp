#include "mbed.h"
#include "adc128.h"

Adc::Adc(i2c_settings *i2c, int addr7) {
    this->addr8 = addr7 << 1;
    this->i2c = i2c;
    this->isInit = false;
}

int Adc::init() {
    char cmd[2];
    char reg[1];
    char data[1];
    /* Adv Config Reg */
    reg[0] = 0x0B;
    if (i2c->write(addr8, reg, 1)) {
        return 1;
    }

    if (i2c->read(addr8, data, 1)) {
        return 1;
    }

    /* We always need to follow read-modify-write pattern */
    data[0] &= ~(0x07);
    data[0] |= 0x03;

    cmd[0] = reg[0];
    cmd[1] = data[0];  /* Ext ref, Mode 1 */

    if (i2c->write(addr8, cmd, 2)) {
        return 1;
    }

    wait_us(10000);

    /* Conv rate reg*/
    reg[0] = 0x07;

    if (i2c->write(addr8, reg, 1)) {
        return 1;    
    }

    if (i2c->read(addr8, data, 1)) {
        return 1;    
    }

    data[0] &= ~(0x01); 
    data[0] |= 0x01;    /* Cont conv */

    cmd[0] = reg[0];
    cmd[1] = data[0];

    if (i2c->write(addr8, cmd, 2)) {
        return 1;
    }

    wait_us(10000);

    /* Limit Regs */
    /*    reg[0] = */
    /*    for int*/


    /* Config Reg */
    reg[0] = 0x00;
    if (i2c->write(addr8, reg, 1)) {
        return 1;
    }

    if (i2c->read(addr8, data, 1)) {
        return 1;
    }
    wait_us(10000);
    data[0] &= ~(0x09);
    data[0] |= 0x01;

    cmd[0] = reg[0];
    cmd[1] = data[0];

    if (i2c->write(addr8, cmd, 2)) {
        return 1;
    }

    wait_us(10000);

    if (i2c->write(addr8, reg, 1)) {
        return 1;
    }

    if (i2c->read(addr8, data, 1)) {
        return 1;
    }
    this->isInit = true; 
    return 0;
}

/* TODO: Possible optimization, if we store as 2 8 bit ints, we could then
 * directly send rather than converting back */
uint16_t Adc::readChannel(AdcChan chan, uint8_t devNum, uint8_t channel, uint8_t* data) {
    char cmd[1] = {(char) chan};
    char d[2];

    if (!this->isInit) {
        return 0;
    }

    uint8_t cmdByte = 0;
    cmdByte = SD_BIT | CHANNEL(channel) | PD_BITS;

    if (write_byte_i2c(adcs[devNum], cmdByte) != 0) {
        fprintf(stderr, "Failed to write channel select byte: %#x\n", cmdByte);
        return 0
    }

    selectChannel(devNum, channel);
    if (read_i2c((i2c_settings*)adcs[devNum], data, 1) != 0) {
        fprintf(stderr, "Failed to read data from ADC %d.\n", devNum);
        return 0;
    }
    uint16_t ret = ((( (uint16_t) d[1]) & 0xf) << 8) | ((uint16_t) d[0]);
    return ret;
}

int Adc::get8BitAddress() {
    return addr8;
}

int Adc::isBusy() {
    char cmd[1];
    cmd[0] = 0x0C;
    
    if (!this->isInit) {
        return 0;
    }

    uint8_t cmdByte = 0;
    cmdByte = SD_BIT | CHANNEL(channel) | PD_BITS;

    if (write_byte_i2c(adcs[devNum], cmdByte) != 0) {
        fprintf(stderr, "Failed to write channel select byte: %#x\n", cmdByte);
        return 0
    }

    selectChannel(devNum, channel);
    if (read_i2c((i2c_settings*)adcs[devNum], data, 1) != 0) {
        fprintf(stderr, "Failed to read data from ADC %d.\n", devNum);
        return 0;
    }
    printf("Check success: cmd[0] & 0x03 == %d\n\r", cmd[0] & 0x03);
    /* 0x01 means its powering up, 0x02 is converting, we just generally want to know if it is free */
    return cmd[0] & 0x03; 
}

int Adc::debug() {
    printf("ADC FOUND\n\r");
    return 0;
}

