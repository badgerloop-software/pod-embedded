#ifndef __ADC128_H__
#define __ADC128_H__

const int railADC7Addr = 0x1f;
const int presADC7Addr = 0x1d;

typedef enum AdcChan {
    CHAN_0 = 0x20,
    CHAN_1 = 0x21,
    CHAN_2 = 0x22,
    CHAN_3 = 0x23,
    CHAN_4 = 0x24,
    CHAN_5 = 0x25,
    CHAN_6 = 0x26,
    CHAN_7 = 0x27,
} AdcChan;

class Adc {
    private:
        int addr8;
        i2c_settings *i2c;
        bool isInit;
    public:
        Adc(i2c_settings *i2c, int addr7);
        int initPressureSensors(void);
        int readPressureSensor(int sensor, uint8_t channel, uint8_t* data);
        int get8BitAddress(void);
        int isBusy(void);
        int debug(void);
};


#endif
