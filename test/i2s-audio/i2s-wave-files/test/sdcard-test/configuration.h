
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION
    Use this file to store all of application configruation details like hostname,
    PIN assignments, buffer sizes, etc. that are likely to appear in multiple file.

REFERENCE MATERIALS

WARNING

CREATED BY
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

#pragma once                // compiler to skip subsequent includes of this file


// --------------------------------- host name ---------------------------------
#define HOSTNAME "sdcard-test"     // hostname of this application
#define SERIAL_SPEED 9600          // bit rate speed of serial interface


// --------------------------- board pin assignment ----------------------------
#ifdef ESP32
#define PCBLED   LED_BUILTIN       // ESP32 Pin to which onboard LED is connected
#define CS_PIN   GPIO_NUM_5        // chip select (aka ss = slave select)
#define SCK_PIN  GPIO_NUM_18       // clock (aka sclk)
#define MOSI_PIN GPIO_NUM_23       // master output / slave input
#define MISO_PIN GPIO_NUM_19       // master input / slave output
#else
#define PCBLED   D0                // ESP8266 Pin to which onboard LED is connected
#endif


// ----------------------------------- ???  ------------------------------------
#define ONE_SECOND    1000UL       // milliseconds in one second
#define TWO_SECOND    2000UL       // milliseconds in two second
#define FIVE_SECOND   5000UL       // milliseconds in five second
#define TEN_SECOND    10000UL      // milliseconds in ten second
#define HALF_MINUTE   30000UL      // milliseconds in half a minute
#define ONE_MINUTE    60000UL      // milliseconds in one minute
#define ONE_HOUR      3600000UL    // milliseconds in one hour
#define ONE_DAY       85400000UL   // milliseconds in one day
#define SEVENTYYEARS  2208988800UL // seconds in seventy years

#define STD_BLKRATE   1000UL       // LED blink rate for 'sign-of-life'/normal operation (milli-seconds)
#define FAST_BLKRATE  250UL        // LED blink rate for 'troubled' operation (milli-seconds)
#define PANIC_BLKRATE 60UL         // LED blink rate for 'panic' operation (milli-seconds)


// ----------------------------- sizes of buffers ------------------------------
#define BUF1_SIZE 25             // array size for small buffers
#define BUF2_SIZE 50             // array size for medium buffers
#define BUF3_SIZE 100            // array size for large buffers


// ------------------------------- include files -------------------------------
//#include "secrets.h"               // include here any keys, passwords, or other secrets

