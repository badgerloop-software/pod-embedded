#ifndef __ADC_TEMP_H__
#define __ADC_TEMP_H__

#include <stdbool.h>

extern "C" {
#include <data.h>
#include <i2c.h>
#include <NCD9830DBR2G.h>
}

#define TELEM_THERM_0 CHANNEL_0
#define TELEM_THERM_1 CHANNEL_1
#define TELEM_THERM_4 CHANNEL_2
#define TELEM_THERM_5 CHANNEL_3
#define ADC_EXTRA0 CHANNEL_4
#define ADC_EXTRA1 CHANNEL_5
#define ADC_EXTRA2 CHANNEL_6
#define TELEM_I_28V CHANNEL_7

class adc_temp {
    private:
        iox_settings iox;
    public:
        adc_temp(void);

        int init(bool);
    
        int setupIox(void);
    
        i2c_settings getadc_tempDev(void);
};

extern adc_temp adcTemp;
#endif
