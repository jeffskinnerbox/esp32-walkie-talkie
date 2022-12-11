#pragma once

#ifdef ESP32
#include <freertos/FreeRTOS.h>
#include <driver/i2s.h>
#else
#endif

/**
 * Base Class for both the ADC and I2S sampler
 **/
class I2SSampler {
protected:
    //-------------- private variables -------------
    i2s_port_t m_i2sPort = I2S_NUM_0;     // i2s port (aka processor) number, the max port number is I2S_NUM_MAX - 1
    i2s_config_t m_i2s_config;

    //--------------- private methods --------------
    virtual void configureI2S() = 0;
    virtual void unConfigureI2S() {};
    virtual void processI2SData(void *samples, size_t count) {};   // nothing to do for the default case

public:
    //-- constructors & destructors for the class --
    I2SSampler(i2s_port_t i2sPort, const i2s_config_t &i2sConfig);

    //--------------- public methods ---------------
    void start();
    void stop();
    virtual int read(int16_t *samples, int count) = 0;
    int sample_rate() { return m_i2s_config.sample_rate; };
};
