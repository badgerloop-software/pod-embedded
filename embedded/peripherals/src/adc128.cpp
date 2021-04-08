#include "adc128.h"


i2c_settings u2 = {
    .fd = 0,
    .bus = 2,
    .deviceAddress = U2_ADDR,
    .openMode = O_RDWR
};

i2c_settings u4 = {
    .fd = 0,
    .bus = 2,
    .deviceAddress = U4_ADDR,
    .openMode = O_RDWR
};

Adc u2_adc = Adc(&u2, U2_ADDR);
Adc u4_adc = Adc(&u4, U4_ADDR);


Adc::Adc(i2c_settings *i2c, int addr7) {
    this->addr8 = addr7 << 1;
    this->i2c = i2c;
    this->isInit = false;
}

int Adc::init() {
    if (this->isInit) {
        return 0;
    }
    uint8_t flush[1];
    unsigned char cmd[2];
    unsigned char reg[1];
    unsigned char data[1];

    printf("I2C Init\n");

    if(i2c_begin(this->i2c) == -1) {
        fprintf(stderr, "Could not open i2c bus.\n");
        return -1;
    }

    /* check if there actually is a device there */
    if (read_i2c(i2c, flush, 1) != 0) {
        fprintf(stderr, "There is not device at bus");
        return -1;
    }

    /* Adv Config Reg */
    reg[0] = 0x0B;
    if (write_byte_i2c(i2c, reg)) {
        fprintf(stderr, "Failed to write to adv config reg\n");
        return 1;
    }

    if (read_i2c(i2c, data, 1)) {
        return 1;
    }

    /* We always need to follow read-modify-write pattern */
    data[0] &= ~(0x07);
    data[0] |= 0x03;

    cmd[0] = reg[0];
    cmd[1] = data[0];  /* Ext ref, Mode 1 */
    printf("[ADC128] Ext ref, Mode 1");
    if (write_data_i2c(i2c, cmd, 2)) {
        return 1;
    }

    usleep(10000);

    /* Conv rate reg*/
    reg[0] = 0x07;

    if (write_data_i2c(i2c, reg, 1)) {
        return 1;    
    }

    if (read_i2c(i2c, data, 1)) {
        return 1;    
    }

    data[0] &= ~(0x01); 
    data[0] |= 0x01;    /* Cont conv */

    cmd[0] = reg[0];
    cmd[1] = data[0];

    if (write_data_i2c(i2c, cmd, 2)) {
        return 1;
    }

    usleep(10000);

    /* Limit Regs */
    /*    reg[0] = */
    /*    for int*/


    /* Config Reg */
    reg[0] = 0x00;
    if (write_data_i2c(i2c, reg, 1)) {
        return 1;
    }

    if (read_i2c(i2c, data, 1)) {
        return 1;
    }
    usleep(10000);
    data[0] &= ~(0x09);
    data[0] |= 0x01;

    cmd[0] = reg[0];
    cmd[1] = data[0];

    if (write_data_i2c(i2c, cmd, 2)) {
        return 1;
    }

    usleep(10000);

    if (write_data_i2c(i2c, reg, 1)) {
        return 1;
    }

    if (read_i2c(i2c, data, 1)) {
        return 1;
    }
    this->isInit = true; 
    return 0;
}

/* TODO: Possible optimization, if we store as 2 8 bit ints, we could then
 * directly send rather than converting back */
uint16_t Adc::readChannel(AdcChan chan) {
    unsigned char cmd[1] = {(char) chan};
    unsigned char d[2];

    if (!this->isInit) {
        return 0;
    }

    if (write_data_i2c(i2c, cmd, 1)) {
        return 0;
    }

    if (read_i2c(i2c, d, 2)) {
        return 0;
    }
    uint16_t ret = ((( (uint16_t) d[1]) & 0xf) << 8) | ((uint16_t) d[0]);
    return ret;
}

int Adc::get8BitAddress() {
    return i2c->deviceAddress;
}

int Adc::isBusy() {
    unsigned char cmd[1];
    cmd[0] = 0x0C;
    
    if (!this->isInit) {
        return 0;
    }

    if (write_data_i2c(i2c, cmd, 1)) {
        return 1;    
    }

    if (read_i2c(i2c, cmd, 1)) {
        return 1;    
    }
    printf("Check success: cmd[0] & 0x03 == %d\n\r", cmd[0] & 0x03);
    /* 0x01 means its powering up, 0x02 is converting, we just generally want to know if it is free */
    return cmd[0] & 0x03; 
}

int Adc::debug() {
    printf("ADC FOUND\n\r");
    return 0;
}
