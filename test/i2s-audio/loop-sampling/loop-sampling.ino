
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


#define TDEBUG true              // activate trace message printing for debugging

#ifdef ESP32
//#include <Arduino.h>
//#include <WiFi.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <soc/adc_channel.h>    // gpio lookup macros
#else
#endif

#include "DeBug.h"



//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "loop-sample" " - "  __DATE__ " at " __TIME__
static char version[] = VER;



//-------------------------- Static Scoped Variables ---------------------------

extern DeBug DB;         // declare object DB as external, and member of class DeBug


// calibration values for the adc
#define DEFAULT_VREF 1100
esp_adc_cal_characteristics_t *adc_chars;



//------------------------------- Main Routines --------------------------------

// for use of ADC, see https://embeddedexplorer.com/esp32-adc-esp-idf-tutorial/
void setup() {

    esp_adc_cal_value_t val_type;

    Serial.begin(9600);
    while (!Serial) {}                   // wait for serial port to connect

    DEBUGSETUP();                        // should be right after 'Serial.begin' and before any debug message
    DEBUGTRACE(HEADING, "------------------------------- Entered setup() --------------------------------");
    DEBUGTRACE(INFO, "Application Version = ", version);
    //DEBUGINFO();

    // set quanitzation range for 12-bits = 0-4096 (acceptable values are 9, 10, 11, 12)
    adc1_config_width(ADC_WIDTH_BIT_12);

    //DEBUGTRACE(INFO, "GPIO number of ADC1 channel 7 = %d\r\n", (int)ADC1_CHANNEL_7_GPIO_NUM);
    //DEBUGTRACE(INFO, "ADC1 channel number of GPIO 32 = %d\r\n", (int)ADC1_GPIO32_CHANNEL);

    // analog voltage range voltage is 150mV - 2.45V = requires 11dB attenuation
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);    // GPIO Pin35
    DEBUGTRACE(INFO, "What type of ADC calibration is available:");

    // check to see what type of calibration is available for the adc
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK)
        DEBUGTRACE(INFO, "\tADC Calibration: Using voltage ref stored in eFuse");
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK)
        DEBUGTRACE(INFO, "\tADC Calibration: Using two point values from eFuse");
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_DEFAULT_VREF) == ESP_OK)
        DEBUGTRACE(INFO, "\tADC Calibration: Using default VREF");

    DEBUGTRACE(INFO, "Another method to check ADC calibration:");

    // check to see what type of calibration is available for the adc
    adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
        DEBUGTRACE(INFO, "\tADC Calibration: Using voltage ref stored in eFuse");
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
        DEBUGTRACE(INFO, "\tADC Calibration: Using two point values from eFuse");
    if (val_type == ESP_ADC_CAL_VAL_DEFAULT_VREF)
        DEBUGTRACE(INFO, "\tADC Calibration: Using default VREF");

    DEBUGTRACE(HEADING, "------------------------------- Leaving setup() --------------------------------");

}



void loop() {

    DEBUGLOOP();

    // for a more accurate reading you could read multiple samples here

    // read a voltage sample from the adc
    int sample = adc1_get_raw(ADC1_CHANNEL_7);    // GPIO Pin35

    // get the calibrated voltage sample
    int milliVolts = esp_adc_cal_raw_to_voltage(sample, adc_chars);

    DEBUGTRACE(INFO, "Sample = %d, Voltage = %d mV\r\n", sample, milliVolts);

    delay(500);

}
