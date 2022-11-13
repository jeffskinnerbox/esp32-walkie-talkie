#pragma once

#include <driver/adc.h>   // find this using ... cd ~/.arduino15 ; findfile adc.h ; find version for esp32, esp32C3, esp32s2
#include "I2SSampler.h"

class ADCSampler : public I2SSampler
{
private:
    adc_unit_t m_adcUnit;
    adc1_channel_t m_adcChannel;

protected:
    void configureI2S();

public:
    ADCSampler(adc_unit_t adc_unit, adc1_channel_t adc_channel, const i2s_config_t &i2s_config);
    virtual int read(int16_t *samples, int count);
};
