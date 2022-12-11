#pragma once

#include "I2SSampler.h"

class I2SMEMSSampler : public I2SSampler {
private:
    //-------------- private variables -------------
    i2s_pin_config_t m_i2sPins;
    bool m_fixSPH0645;

protected:
    //-------------- protected methods -------------
    void configureI2S();

public:
    //-- constructors & destructors for the class --
    I2SMEMSSampler(i2s_port_t i2s_port, i2s_pin_config_t &i2s_pins, i2s_config_t i2s_config, bool fixSPH0645 = false);

    //--------------- public methods ---------------
    virtual int read(int16_t *samples, int count);
};
