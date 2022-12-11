
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION
    This a ESP32 test routine


PHYSICAL DESIGN
    Hardware
        ESP-32S (aka ESP-WROOM-32 ESP32) Development Board - https://www.amazon.com/gp/product/B0718T232Z/

    Wiring

MONITOR
    To monitor the trace messages of the ESP32 development board,
    connection the USB port to a Linux computer and execute the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter `CNTR-a :quit`.
    To stop the screen scrolling, enter `CNTR-a`.

TESTING
    To test the

REFERENCE MATERIALS
    for use of ADC, see https://embeddedexplorer.com/esp32-adc-esp-idf-tutorial/

SOURCES
    https://github.com/atomic14/esp32_audio

------------------------------------------------------------------------------*/

#define TDEBUG  true         // should we activate trace message printing for debugging

#ifdef ESP32
#include "I2SMEMSSampler.h"
#include "soc/i2s_reg.h"
#include <algorithm>
#else
#endif

#include "DeBug.h"

//---------------------------- Static Scoped Macros ----------------------------

//-------------------------- Static Scoped Variables ---------------------------

extern DeBug DB;         // declare object DB as external, and member of class DeBug


// ------------------------ Constructors & Destructors -------------------------

I2SMEMSSampler::I2SMEMSSampler( i2s_port_t i2s_port, i2s_pin_config_t &i2s_pins, i2s_config_t i2s_config, bool fixSPH0645) : I2SSampler(i2s_port, i2s_config) {

    m_i2sPins = i2s_pins;
    m_fixSPH0645 = fixSPH0645;

    DEBUGTRACE(INFO, "I'm in I2SMEMSSampler constructor");
}



// ----------------------------- Protected Methods -----------------------------

void I2SMEMSSampler::configureI2S() {

    if (m_fixSPH0645) {
        // FIXES for SPH0645
        REG_SET_BIT(I2S_TIMING_REG(m_i2sPort), BIT(9));
        REG_SET_BIT(I2S_CONF_REG(m_i2sPort), I2S_RX_MSB_SHIFT);
    }

    i2s_set_pin(m_i2sPort, &m_i2sPins);

    DEBUGTRACE(INFO, "I'm in I2SMEMSSampler configureI2S");

}



// ------------------------------- Public Methods ------------------------------

int I2SMEMSSampler::read(int16_t *samples, int count) {

    int32_t raw_samples[256];
    int sample_index = 0;

    while (count > 0) {
        size_t bytes_read = 0;
        i2s_read(m_i2sPort, (void **)raw_samples, sizeof(int32_t) * std::min(count, 256), &bytes_read, portMAX_DELAY);
        int samples_read = bytes_read / sizeof(int32_t);
        for (int i = 0; i < samples_read; i++) {
            samples[sample_index] = (raw_samples[i] & 0xFFFFFFF0) >> 11;
            sample_index++;
            count--;
        }
    }

    DEBUGTRACE(INFO, "I'm in I2SMEMSSampler read()");

    return sample_index;

}

