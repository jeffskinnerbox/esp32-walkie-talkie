
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

SOURCES
    https://github.com/atomic14/esp32_audio

------------------------------------------------------------------------------*/


#define TDEBUG true              // activate trace message printing for debugging

#ifdef ESP32
#include <Arduino.h>
#include <driver/dac.h>
#else
#endif

#include "DeBug.h"



//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "dac-sample-output" " - "  __DATE__ " at " __TIME__
static char version[] = VER;



//-------------------------- Static Scoped Variables ---------------------------

extern DeBug DB;         // declare object DB as external, and member of class DeBug



//------------------------------- Main Routines --------------------------------

void setup() {

    Serial.begin(9600);
    while (!Serial) {}                   // wait for serial port to connect

    DEBUGSETUP();                        // should be right after 'Serial.begin' and before any debug message
    DEBUGTRACE(HEADING, "------------------------------- Entered setup() --------------------------------");
    DEBUGTRACE(INFO, "Application Version = ", version);
    DEBUGINFO();

    DEBUGTRACE(INFO, "Starting up the DAC channel 1 ...");
    dac_output_enable(DAC_CHANNEL_1);    // enable the DAC channel, GPIO25

    DEBUGTRACE(HEADING, "------------------------------- Leaving setup() --------------------------------");

}


void loop() {

    DEBUGLOOP();

    // DAC output is 8-bit where 0 is zero volts and maximum (255) corresponds to VDD
    for (int i = 0; i < 256; i += 15) {
        DEBUGTRACE(INFO, "Sending %2d, expecting %.2f output voltage\r\n", i, i * 3.3 / 255.0f);

        dac_output_voltage(DAC_CHANNEL_1, i);

        delay(3000);
    }

}
