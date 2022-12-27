
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5

DESCRIPTION:

REFERENCE MATERIALS:

SOURCES:

CREATED BY:
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

#define TDEBUG  true       // activate trace message printing for debugging


#ifdef ESP32
#include <WiFi.h>
#include <ArduinoOTA.h>
#else                     // found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#endif

// found in Arduino libraries (~/Arduino/libraries)
#include <TelnetStream.h>

// found in Arduino libraries (~/src/arduino/libraries)

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// project include files
#include "DeBug.h"
#include "configuration.h"
#include "OTAHandler.h"

extern DeBug DB;                 // declare object DB as external, and member of class DeBug



// ------------------------ Constructors & Destructors -------------------------

// default constructors for the class
OTAHandler::OTAHandler(void) {
    ota_flag = true;              // if true, engage the arduino ota handler
    max_time = OTA_MAXTIME;       // max time to wait for arduino ota handler to engage (15 seconds)
    blink_rate = OTA_BLKRATE;     // interval at which to blink LED (milliseconds) during OTA
    standard_rate = STD_BLKRATE;  // interval at which to blink LED (milliseconds) after OTA
}


OTAHandler::OTAHandler(bool o, unsigned long m, unsigned long b, unsigned long s) {
    ota_flag = o;                 // if true, engage the arduino ota handler
    max_time = m;                 // max time to wait for arduino ota handler to engage (15 seconds)
    blink_rate = b;               // interval at which to blink LED (milliseconds) during OTA
    standard_rate = s;            // interval at which to blink LED (milliseconds) after OTA
}


OTAHandler::~OTAHandler(void) {
}


// ------------------------------ Private Methods ------------------------------

void OTAHandler::blinkLED(unsigned long rate) {
    unsigned long currentTime = millis();
    static unsigned long previousTime = millis();

    // blink LED to signal that you are OTA update ready
    if (currentTime - previousTime >= rate) {
        previousTime = currentTime;                  // save the last time you blinked the LED
        digitalWrite(PCBLED, !digitalRead(PCBLED));  // toggle led state
    }

}


// print status of OTAHandler object
void OTAHandler::printStatus(void) {

    DEBUGTRACE(INFO, "Current status of OTAHandler object: ");

    DEBUGTRACE(INFO, "\tOTA flag = %s", ota_flag ? "true" : "false");
    DEBUGTRACE(INFO, "\tOTA port = %s", String(OTAPORT));
    DEBUGTRACE(INFO, "\tHostname = %s", ArduinoOTA.getHostname() + ".local");
    DEBUGTRACE(INFO, "\tIP address = %d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);

}


void OTAHandler::TelnetStreamHandler(void) {

    switch (TelnetStream.read()) {
        case 'r':   // reboot the esp8266
            Serial.println("\n\rRebooting ...");
            TelnetStream.println("\n\rRebooting ...");
            TelnetStream.flush();
            TelnetStream.stop();
            delay(100);
            ESP.restart();
            break;
        case 'c':   // drop telnet connection to esp8266
            Serial.println("\n\rDropping telnet connection ... bye bye");
            TelnetStream.println("\n\rDropping telnet connection ... bye bye");
            TelnetStream.flush();
            TelnetStream.stop();
            break;
        default:
            break;
    }

}



// ------------------------------- Public Methods ------------------------------

void OTAHandler::setupOTA(void) {

    DEBUGTRACE(HEADING, "----------------------------- Entered OTAHandler() -----------------------------");

    pinMode(PCBLED, OUTPUT);                           // set led pin as output so you can blink it

    ArduinoOTA.setPort(OTAPORT);                       // if not set, port defaults to 8266 (3232 for esp32)
    ArduinoOTA.setHostname(HOSTNAME);                  // if not set, hostname defaults to esp8266-<ChipID-in-Hex>
    ArduinoOTA.setPassword((const char *)OTAPASSWORD); // if not set, no authentication required by default

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";      // executable firmware
        } else {
            type = "filesystem";  // U_SPIFFS
            // NOTE: if updating SPIFFS, this would be the place to unmount SPIFFS using SPIFFS.end()
        }

        Serial.println("Started OTA updating the " + type);
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ArduinoOTA.onEnd([]() { Serial.println("\n\rOTA updating ended"); });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();

    printStatus();

    DEBUGTRACE(HEADING, "------------------------------ Exited OTAHandler() -----------------------------");

}


void OTAHandler::loopOTA(void) {
    unsigned long currentTime = millis();
    static unsigned long previousTime = millis();
    static unsigned long elapsedTime, i = 0;

    TelnetStreamHandler();

    // if ota is being requested, activate the handler
    if (ota_flag) {
        DEBUGTRACE(HEADING, "--------------------------- Entered OTA update state ---------------------------");
        while (currentTime - previousTime < max_time) {
            ArduinoOTA.handle();     // OTA handler, look for OTA update request
            currentTime = millis();
            blinkLED(blink_rate);    // blink led at ota update enabled rate
            delay(10);
        }
        ota_flag = false;
        blinkLED(standard_rate);     // blink led at standard rate (ota update disabled)
        DEBUGTRACE(HEADING, "---------------------------- Exited OTA update state ---------------------------");
    }

}



// ---------------------------- Construct OTA Object ---------------------------

// for over-the-air firmware updates, construct object OTA as class OTAHandler
OTAHandler OTA = OTAHandler(true, 15000UL, 100, 1000);

