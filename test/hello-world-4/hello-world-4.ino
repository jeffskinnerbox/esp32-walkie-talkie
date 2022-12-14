
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
        telnet hello-world-4.local

    To terminate telnet monitoring, enter 'Ctrl-] quit'.

    To find the device on your LAN:

        sudo netdiscover -c 3 -s 10 -L -N -r 192.168.1.0/24
        sudo arp-scan 192.168.1.0/24 | grep Espressif

TESTING
    To test the telnet capabilities

    To test the OTA capabilities

REFERENCE MATERIALS:

SOURCES:

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/


#define TDEBUG  true              // activate trace message printing for debugging

// this project's include files
#include "DeBug.h"
#include "configuration.h"
#include "WiFiHandler.h"
#include "OTAHandler.h"



//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "hello-world-4" " - "  __DATE__ " at " __TIME__
static char version[] = VER;

// time intervals
#define ONE_SECOND    1000UL        // milliseconds in one second
#define ONE_MINUTE    60000UL       // milliseconds in one minute

// error codes
#define NOWIFI        1             // can't get wifi connection
#define UNKNMQTT      2             // error code meaning don't understand mqtt command request



//-------------------------- Static Scoped Variables ---------------------------

// Update these with values suitable for your wifi
static char* wifi_ssid = WIFISSID;
static char* wifi_password = WIFIPASS;
static unsigned long wifi_time = WIFITIME;

// wifi handlers object constructor
WiFiHandler WT = WiFiHandler();

extern DeBug DB;                  // declare object DB as external, and member of class DeBug
extern OTAHandler OTA;            // declare object OTA as external, and member of class OTAHandler



//------------------------------ Helper Routines -------------------------------

// blink led to show 'sign of life' and other status
bool blinkLED(unsigned long rate) {
    unsigned long currentTime = millis();
    static unsigned long previousTime = millis();

    // blink LED at the rate specified
    if (currentTime - previousTime >= rate) {
        previousTime = currentTime;                            // save the last time you blinked the LED
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // toggle led state
        return true;
    }

    return false;

}



//--------------------------- Error Message Handler ----------------------------

// handle errors by displaying a code and then taking action (e.g. restart)
void errorHandler(int error_code, char *msg) {

    int i = 0;
    unsigned long tout;                      // time-out time

    DEBUGTRACE(INFO, "In errorHandsler() and error_code = %d", error_code);

    switch(error_code) {
        case NOWIFI:
            DEBUGTRACE(ERROR, msg);
            DEBUGTRACE(FATAL, "An automated restart will be requested.");

            tout = ONE_MINUTE + millis();    // milliseconds of time to display message
            while (millis() < tout) {
                // any activity you want to do while waiting for wifi
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



//------------------------------- Main Routines --------------------------------

void setup() {

    unsigned long tout;           // time-out time

    Serial.begin(SERIAL_SPEED);   // always start Serial first so it can be used by DeBug
    while (!Serial) {}            // wait for serial port to connect

    DEBUGSETUP();                 // should be right after 'Serial.begin' and before any debug message
    DEBUGTRACE(HEADING, "------------------------------- Entered setup() --------------------------------");
    DEBUGTRACE(INFO, "Application Version = ", version);

    pinMode (LED_BUILTIN, OUTPUT);

    // scan for wifi access points
    DEBUGWIFISCAN();

    // attempt to connect and initialise WiFi network
    if (WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME)) {       // connecting to wifi
        DEBUGTRACE(INFO, "WiFi connected successfully to SSID ", WIFISSID);
        tout = ONE_SECOND + millis();    // milliseconds of time to display message
        while (millis() < tout) {
            // any activity you want to do while waiting for wifi to come up
            yield();                     // prevent the watchdog timer doing a reboot
        }
    } else
        errorHandler(NOWIFI, "Can't go on without WiFi connection. Press reset to try again.");

    DEBUGON(true, true, false);          // turn on serial and telnet tracing

    OTA.setupOTA();                      // should always follow 'DEBUGSETUP()' and after wifi is up

    DEBUGTRACE(HEADING, "-------------------------------- Exited setup() --------------------------------");

}


void loop() {

    DEBUGLOOP();                         // place this anywhere in the loop() routine
    OTA.loopOTA();                       // place this anywhere in the loop() routine

    if (blinkLED(STD_BLKRATE))
        DEBUGTRACE(INFO, "Hello World 4");

}
