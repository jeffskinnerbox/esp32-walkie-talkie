
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION:
    This a ESP32 test routine using the WiFiHandler, OTAHandler, and DeBug classes.
    Debug trace messages will be sent to the ESP32's USB serial interface and
    those same messages can be viewed remotely via telnet.  The development board
    firmware can be updated via OTA.

PHYSICAL DESIGN:
    Hardware
        ESP-32S (aka ESP-WROOM-32 ESP32) Development Board - https://www.amazon.com/gp/product/B0718T232Z/

    Wiring
        Nothing required

MONITOR:
    To monitor the trace messages of the ESP32 development board,
    connection the USB port to a Linux computer and execute the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter 'CNTR-a :quit'.
    To stop the screen scrolling, enter 'CNTR-a'.

    To monitor using telnet, execute the following:

        telnet ESP_24F9FB             # using esp32 hostname ('ESP_' + chip ID)
        telnet 192.168.1.109          # using esp32 ip address
        telnet hello-world-5.local

    To terminate telnet monitoring, enter 'Ctrl-] quit'.

    To find the device on your LAN:

        sudo netdiscover -c 3 -s 10 -L -N -r 192.168.1.0/24
        sudo arp-scan 192.168.1.0/24 | grep Espressif

TESTING
    To test the telnet capabilities

    To test the OTA capabilities

    For testing, you may want to "see" the ticking of the clock.  To "see the tick-tock"
    on the serial connection, set HEARTBEAT to true.  You may also want to have more
    frequent calls to NTP to refeash the time.  To speed up the refreashing, modify REFRESH
    to a value like 5000UL.

    To test the MQTT capabilities, use a public MQTT broker site
    and Mosquitto clients like this:

        mosquitto_sub -v -h broker.mqtt-dashboard.com -t "hello-world-5/message"
        mosquitto_sub -v -h broker.mqtt-dashboard.com -t "hello-world-5/error"
        mosquitto_sub -v -h broker.mqtt-dashboard.com -t "hello-world-5/#"

        mosquitto_pub -h broker.mqtt-dashboard.com -t "hello-world-5/command" -m "2"

USAGE
    commands sent via MQTT:
        '1' - turn on red LED so it blinks with the tick-tock
        '2' - calculate drift sinse last time refresh
        '3' - force a time refresh with NTP server

REFERENCE MATERIALS:

SOURCES:

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/


#define TDEBUG  true              // activate trace message printing for debugging

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)
//#include <PubSubClient.h>

// arduino and esp include files
//#include <FS.h>
//#include <SD_MMC.h>
/*#include <SD.h>*/
/*#include <SPI.h>*/

// this project's include files
#include "DeBug.h"
#include "configuration.h"
#include "CardHandler.h"


//---------------------- Static Scoped Macros & Variables ----------------------

// version stamp
#define VER HOSTNAME " - "  __DATE__ " at " __TIME__
static char version[] = VER;

// declare the sd card and wifi handlers
//static WiFiHandler WH = WiFiHandler();


//-------------------------- Static Scoped Variables ---------------------------

CardHandler *SDCard;



//--------------------------- Global Scoped Variables --------------------------

extern DeBug DB;   // declare object DB as external, and member of class DeBug



//------------------------------ Helper Routines -------------------------------

// blink led to show 'sign of life' and other status
bool blinkLED(unsigned long rate) {

    unsigned long currentTime = millis();          // initialized everytime you enter this routine
    static unsigned long previousTime = millis();  // initialized first time you enter this routine

    // blink LED at the rate specified
    if (currentTime - previousTime >= rate) {
        previousTime = currentTime;                            // save the last time you blinked the LED
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // toggle led state
        return true;
    }

    return false;

}



//------------------------------- Main Routines --------------------------------

void setup() {

    Serial.begin(SERIAL_SPEED);   // always start Serial first so it can be used by DeBug
    while (!Serial) {}            // wait for serial port to connect

    DEBUGSETUP();                 // must be right after 'Serial.begin' and before any debug message
    DEBUGTRACE(HEADING, "------------------------------- Entered setup() --------------------------------");
    DEBUGTRACE(INFO, "Application Version = %s", version);

    SDCard = new CardHandler();

    SDCard->testFileIO("/test.txt");

    DEBUGTRACE(HEADING, "-------------------------------- Exited setup() --------------------------------");

}


void loop() {

    DEBUGLOOP();                         // place this anywhere in the loop() routine

    DEBUGTRACE(INFO, "---------- I2S Wave File Test ----------");

    SDCard->listDir("/", 0);
    SDCard->createDir("/mydir");
    SDCard->listDir("/", 0);
    SDCard->removeDir("/mydir");
    SDCard->listDir("/", 2);
    SDCard->writeFile("/hello.txt", "Hello ");
    SDCard->readFile("/foo.txt");
    SDCard->appendFile("/hello.txt", "World!\n\r");
    SDCard->readFile("/hello.txt");
    SDCard->listDir("/", 3);
    SDCard->deleteFile("/foo.txt");
    SDCard->renameFile("/hello.txt", "/foo.txt");

    delay(5000);

}
