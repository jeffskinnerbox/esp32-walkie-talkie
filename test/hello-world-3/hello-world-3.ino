
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION
    This a ESP32 test routine using the WiFiHandler and DeBug classes.
    Debug trace messages will be sent to the ESP32's USB serial interface and
    those same messages can be viewed remotely via telnet using
    the ESP32 TelnetStream connectivity.


PHYSICAL DESIGN
    Hardware
        ESP-32S (aka ESP-WROOM-32 ESP32) Development Board - https://www.amazon.com/gp/product/B0718T232Z/

    Wiring
        Nothing required

MONITOR
    To monitor the trace messages of the ESP32 development board,
    connection the USB port to a Linux computer and execute the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter `CNTR-a :quit`.
    To stop the screen scrolling, enter `CNTR-a`.

    To monitor using telnet, execute the following:

        telnet ESP_24F9FB             # using esp32 hostname ('ESP_' + chip ID)
        telnet -b 192.168.1.109       # using esp32 ip address
        telnet hello-world-3.local

    To terminate telnet monitoring, enter `Ctrl-] quit`.

    To find the device on your LAN:

        sudo netdiscover -c 3 -s 10 -L -N -r 192.168.1.0/24
        sudo arp-scan 192.168.1.200/24 | grep Espressif

TESTING
    To test the telnet capabilities

REFERENCE MATERIALS:

SOURCES

------------------------------------------------------------------------------*/


#define TDEBUG  true              // activate trace message printing for debugging

// this project's include files
#include "WiFiHandler.h"
#include "secrets.h"
#include "DeBug.h"



//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "hello-world-3" " - "  __DATE__ " at " __TIME__
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

// Update these with values suitable for your serial connection
static unsigned int serial_speed = 9600;

// wifi handlers object constructor
WiFiHandler WT = WiFiHandler();

extern DeBug DB;                  // declare object DB as external, and member of class DeBug
//extern OTAHandler OTA;          // declare object OTA as external, and member of class OTAHandler



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



//------------------------------ Helper Routines -------------------------------

void blinkLED(unsigned long rate) {
    unsigned long currentTime = millis();
    static unsigned long previousTime = millis();

    // blink LED at the rate specified
    if (currentTime - previousTime >= rate) {
        previousTime = currentTime;                            // save the last time you blinked the LED
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // toggle led state
    }

}



//------------------------------- Main Routines --------------------------------

void setup() {

    unsigned long tout;           // time-out time

    Serial.begin(serial_speed);   // always start Serial first so it can be used by DeBug
    while (!Serial) {}            // wait for serial port to connect

    DEBUGSETUP();                 // should be right after 'Serial.begin' and before any debug message
    DEBUGTRACE(HEADING, "------------------------------- Entered setup() --------------------------------");
    DEBUGTRACE(INFO, "Application Version = ", version);

    pinMode (LED_BUILTIN, OUTPUT);

    // scan for wifi access points
    DEBUGWIFISCAN();              // equivalent to WT.wifiScan() but with debug trace messages

    // attempt to connect and initialise WiFi network
    if (WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME)) {       // connecting to wifi
        DEBUGTRACE(INFO, "WiFi connected successfully to SSID ", WIFISSID);
        tout = ONE_SECOND + millis();   // milliseconds of time to display message
        while (millis() < tout) {
                // any activity you want to do while waiting for wifi
            yield();                    // prevent the watchdog timer doing a reboot
        }
    } else
        errorHandler(NOWIFI, "Can't go on without WiFi connection. Press reset to try again.");

    // turn on serial and telnet tracing
    DEBUGON(true, true, false);         // telnet tracing activation must be after wifi activation

    DEBUGINFO();

    DEBUGTRACE(HEADING, "------------------------------- Leaving setup() --------------------------------");

}


void loop() {

    unsigned long currentTime = millis();
    static unsigned long previousTime = millis();
    static bool state = HIGH;

    DEBUGLOOP();

    DEBUGTRACE(INFO, "Hello World 3");

    blinkLED(1000);   // remove any delay() calls to assure responsive DeBug tracing

}

