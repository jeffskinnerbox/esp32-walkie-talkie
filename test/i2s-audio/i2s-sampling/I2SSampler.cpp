
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
#include "driver/i2s.h"
#else
#endif

#include "DeBug.h"
#include "I2SSampler.h"

//---------------------------- Static Scoped Macros ----------------------------

//-------------------------- Static Scoped Variables ---------------------------

extern DeBug DB;         // declare object DB as external, and member of class DeBug




// ------------------------ Constructors & Destructors -------------------------

I2SSampler::I2SSampler(i2s_port_t i2sPort, const i2s_config_t &i2s_config) : m_i2sPort(i2sPort), m_i2s_config(i2s_config) {
    DEBUGTRACE(INFO, "I'm in I2SSampler constructor");
}


// ------------------------------- Public Methods ------------------------------

// This configures the I2S driver and pins.
// This function must be called before any I2S driver read/write operations.
void I2SSampler::start() {

    esp_err_t err;

    //install and start i2s driver
    err = i2s_driver_install(m_i2sPort, &m_i2s_config, 0, NULL);
    if (err != ESP_OK) {
        DEBUGTRACE(WARN, "Failed installing driver.  Error code = %d\r\n", err);
        while (true);
    }

    // set up the I2S configuration from the subclass
    configureI2S();
    // replace the above with this ?? - https://diyi0t.com/i2s-sound-tutorial-for-esp32/
    //err = i2s_set_pin(I2S_PORT, &pin_config);
    //if (err != ESP_OK) {
        //Serial.printf("Failed setting pin: %d\n", err);
        //while (true);
    //}

    DEBUGTRACE(INFO, "Using I2S port number: ", m_i2sPort);
    DEBUGTRACE(INFO, "Maximum number of I2S ports (aka channels or processors): ", I2S_NUM_MAX);
    DEBUGTRACE(INFO, "I2S driver installed.");

}


void I2SSampler::stop() {

    // clear any I2S configuration
    unConfigureI2S();

    // stop the i2S driver
    i2s_driver_uninstall(m_i2sPort);

    DEBUGTRACE(INFO, "I'm in I2SSampler stop()");

}
