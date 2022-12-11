
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/


#define TDEBUG  true       // activate trace message printing for debugging

# define INIT_FAILED 1

// arduino and esp include files
#include "FS.h"
#include "SD.h"
#include "SPI.h"

// ESP8266 libraries (~/.arduino15/packages/esp8266)
//#include <WiFiUdp.h>
//#include <ESP8266WiFi.h>

// Arduino libraries (~/src/arduino/libraries)
//#include <Arduino.h>

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// this project's include files
#include "DeBug.h"
#include "configuration.h"
#include "CardHandler.h"


extern DeBug DB;        // declare object DB as external, it is instantiated in DeBug.cpp



// ------------------------ Constructors & Destructors -------------------------

// Constructor to create CardHandler
CardHandler::CardHandler(void) {

    if (!initSDCard())
        errorHandler(INIT_FAILED, "SD Card initialization failure.");

}


// Destructor to delete WiFiHandler
CardHandler::~CardHandler(void) {
}


//------------------------------ Private Methods -------------------------------

// handle errors by displaying a code and then taking action (e.g. restart)
void CardHandler::errorHandler(int error_code, char *msg) {

    int i = 0;
    unsigned long tout;                      // time-out time

    DEBUGTRACE(INFO, "In errorHandler() and error_code = %d", error_code);

    switch(error_code) {
        case INIT_FAILED:
            DEBUGTRACE(ERROR, msg);
            DEBUGTRACE(FATAL, "An automated restart will be requested.");

            tout = ONE_MINUTE + millis();    // milliseconds of time to display message
            while (millis() < tout) {
                // any activity you want to do while waiting
                yield();                     // prevent the watchdog timer from doing a reboot
            }

            // nothing can be done so restart
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.restart();
            break;
        default:
            // nothing can be done so restart
            DEBUGTRACE(ERROR, "Unknown error code in errorHandler: ", error_code);
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.restart();
    }
}


// initialize the SD card for use
bool CardHandler::initSDCard(void) {

    if (!SD.begin(cs_pin)) {
        DEBUGTRACE(ERROR, "SD card mount failed");
        return false;
    } else
        uint8_t cardType = SD.cardType();

    switch(cardType) {
        case CARD_NONE:
            DEBUGTRACE(ERROR, "No SD card attached");
            return false;
            break;
        case CARD_MMC:
            DEBUGTRACE(INFO, "SD card type: MMC");
            break;
        case CARD_SD:
            DEBUGTRACE(INFO, "SD card type: SDSC");
            break;
        case CARD_SDHC:
            DEBUGTRACE(INFO, "SD card type: SDHC");
            break;
        default:
            DEBUGTRACE(WARN, "SD card type: unknown");
            return false;
    }

    cardSize = SD.cardSize() / (1024 * 1024);
    DEBUGTRACE(INFO, "SD card size: %lluMB\n", cardSize);

    return true;

}

//------------------------------- Public Methods -------------------------------



