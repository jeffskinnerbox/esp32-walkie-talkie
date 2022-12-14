
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5

DESCRIPTION
    Use this file to store all of application configruation details

REFERENCE MATERIALS

WARNING

CREATED BY
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

#pragma once                       // compiler to skip subsequent includes of this file

#include "secrets.h"               // include here any keys, passwords, or other secrets


// ------------------------------------- host name  -------------------------------------
#define HOSTNAME "hello-world-4"   // hostname of this application
#define SERIAL_SPEED 9600          // bit rate speed of serial interface


// ------------------------------------- board pin assignment -------------------------------------
#ifdef ESP32
#define PCBLED LED_BUILTIN         // ESP32 Pin to which onboard LED is connected
#else
#define PCBLED D0                  // ESP8266 Pin to which onboard LED is connected
#endif


// ------------------------------------- ???  -------------------------------------
#define STD_BLKRATE 1000UL         // LED blink rate for 'sign-of-life'/normal operation (milli-seconds)

// variables for OTA management
#define OTA_MAXTIME 15000UL        // max time to wait for ota handler to engage (milli-seconds)
#define OTA_BLKRATE 200UL          // LED blink rate to signal accepting OTA update (milli-seconds)


// ------------------------------------- sizes of buffers -------------------------------------
#define BUF1_SIZE 25               // array size for small buffers
#define BUF2_SIZE 50               // array size for large buffers

