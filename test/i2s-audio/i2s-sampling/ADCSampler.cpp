
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

#include "ADCSampler.h"

#include "DeBug.h"


//---------------------------- Static Scoped Macros ----------------------------

//-------------------------- Static Scoped Variables ---------------------------

extern DeBug DB;         // declare object DB as external, and member of class DeBug




// ------------------------ Constructors & Destructors -------------------------

ADCSampler::ADCSampler(adc_unit_t adcUnit, adc1_channel_t adcChannel, const i2s_config_t &i2s_config) : I2SSampler(I2S_NUM_0, i2s_config) {

    m_adcUnit = adcUnit;
    m_adcChannel = adcChannel;

    DEBUGTRACE(INFO, "I'm in ADCSampler constructor");

}



// ----------------------------- Protected Methods -----------------------------

void ADCSampler::configureI2S() {

    //init ADC pad
    i2s_set_adc_mode(m_adcUnit, m_adcChannel);

    // enable the adc
    i2s_adc_enable(m_i2sPort);

    DEBUGTRACE(INFO, "I'm in ADCSampler configureI2S()");

}


void ADCSampler::unConfigureI2S() {

    // make sure ot do this or the ADC is locked
    i2s_adc_disable(m_i2sPort);

    DEBUGTRACE(INFO, "I'm in ADCSampler unConfigureI2S()");

}



// ------------------------------- Public Methods ------------------------------

int ADCSampler::read(int16_t *samples, int count) {

    // read from i2s
    size_t bytes_read = 0;
    i2s_read(m_i2sPort, samples, sizeof(int16_t) * count, &bytes_read, portMAX_DELAY);
    int samples_read = bytes_read / sizeof(int16_t);

    for (int i = 0; i < samples_read; i++)
        samples[i] = (2048 - (uint16_t(samples[i]) & 0xfff)) * 15;

    DEBUGTRACE(INFO, "I'm in ADCSampler read()");

    return samples_read;

}
