#include "i2c.h"
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
    if (write_data_i2c(i2c, reg, 1)) {
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

    if (write_data_i2c(i2c, cmd, 2)) {
        return 1;
    }

    wait_us(10000);

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

    wait_us(10000);

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
    wait_us(10000);
    data[0] &= ~(0x09);
    data[0] |= 0x01;

    cmd[0] = reg[0];
    cmd[1] = data[0];

    if (write_data_i2c(i2c, cmd, 2)) {
        return 1;
    }

    wait_us(10000);

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
    char cmd[1] = {(char) chan};
    char d[2];

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
    return i2c;
}

int Adc::isBusy() {
    char cmd[1];
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
