
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.7

DESCRIPTION
    Use this file to store all of application configruation details

REFERENCE MATERIALS
    SD Card - HiLetgo Micro SD TF Card Adater Reader, 6 Pin SPI Interface  - https://www.amazon.com/dp/B07BJ2P6X6

WARNING

CREATED BY
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

#pragma once                       // compiler to skip subsequent includes of this file


// ------------------------- host name  & serial speed -------------------------
#define HOSTNAME "hello-world-6"   // hostname of this application
#define SERIAL_SPEED 9600          // bit rate / speed of serial interface


// ---------------------------- board pin assignment ---------------------------
#ifdef ESP32
#define PCBLED LED_BUILTIN         // ESP32 Pin to which onboard LED is connected
#else                              // nodemcuv2
#define PCBLED D0                  // ESP8266 Pin to which onboard LED is connected
#endif


// ------------------------ sd card board pin assignment -----------------------
#ifdef ESP32                       // nodemcu-32s board
#define CS_PIN   GPIO_NUM_5        // chip select (aka ss = slave select)
#define SCK_PIN  GPIO_NUM_18       // clock (aka sclk)
#define MOSI_PIN GPIO_NUM_23       // master output / slave input
#define MISO_PIN GPIO_NUM_19       // master input / slave output
#else                              // nodemcuv2
#endif


// ------------------------- led blink rate for status -------------------------
#define STD_BLKRATE   1000UL       // LED blink rate for 'sign-of-life'/normal operation (milli-seconds)
#define FAST_BLKRATE  250UL        // LED blink rate for 'troubled' operation (milli-seconds)
#define PANIC_BLKRATE 100UL        // LED blink rate for panic operation (milli-seconds)

// variables for OTA management
#define OTA_MAXTIME   15000UL      // max time to wait for ota handler to engage (milli-seconds)
#define OTA_BLKRATE   250UL        // LED blink rate to signal accepting OTA update (milli-seconds)


// ------------------------------- time intervals ------------------------------
#define ONE_SECOND    1000UL       // milliseconds in one second
#define TWO_SECOND    2000UL       // milliseconds in two second
#define FIVE_SECOND   5000UL       // milliseconds in five second
#define TEN_SECOND    10000UL      // milliseconds in ten second
#define HALF_MINUTE   30000UL      // milliseconds in half a minute
#define ONE_MINUTE    60000UL      // milliseconds in one minute
#define ONE_HOUR      3600000UL    // milliseconds in one hour
#define ONE_DAY       85400000UL   // milliseconds in one day
#define SEVENTYYEARS  2208988800UL // seconds in seventy years


// ----------------------------- sizes of buffers ------------------------------
#define BUF1_SIZE 25               // array size for small buffers
#define BUF2_SIZE 50               // array size for medium buffers
#define BUF3_SIZE 100              // array size for large buffers


// ------------------------------- include files -------------------------------
#include "secrets.h"               // include here any keys, passwords, or other secrets

