
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.2

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
#include <PubSubClient.h>

// this project's include files
#include "DeBug.h"
#include "configuration.h"
#include "WiFiHandler.h"
#include "OTAHandler.h"
#include "MQTTHandler.h"


//---------------------- Static Scoped Macros & Variables ----------------------

// version stamp
#define VER HOSTNAME " - "  __DATE__ " at " __TIME__
static char version[] = VER;

// time intervals
#define ONE_SECOND    1000UL        // milliseconds in one second
#define ONE_MINUTE    60000UL       // milliseconds in one minute

// error codes
#define NOWIFI        1             // can't get wifi connection
#define UNKNMQTT      2             // error code meaning don't understand mqtt command request
#define RESET         3             // restart the processor
#define UNKNMQTT      4             // error code meaning don't understand mqtt command request



//------------------------------- MQTT Parameters ------------------------------

// MQTT message buffer for publishing and subscription
static const int MSGSIZE = 80;                 // size of mqtt message buffer
static char msg[MSGSIZE];                      // buffer to hold mqtt messages



//-------------------------- Static Scoped Variables ---------------------------

// Update these with values suitable for your wifi and mqtt broker
static char* wifi_ssid = WIFISSID;
static char* wifi_password = WIFIPASS;
static unsigned long wifi_time = WIFITIME;
static char* mqtt_server = MQTTSERVER;
static int mqtt_port = MQTTPORT;

// wifi handlers object constructor
WiFiHandler WT = WiFiHandler();
static MQTTHandler MQ = MQTTHandler(mqtt_server, mqtt_port);



//--------------------------- Global Scoped Variables --------------------------

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
            MQ.publish(ERROR_TOPIC, msg, false);

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
        case UNKNMQTT:
            DEBUGTRACE(ERROR, msg);
            MQ.publish(ERROR_TOPIC, msg, false);
            break;
        default:
            // nothing can be done so restart
            DEBUGTRACE(ERROR, "Unknown error code in errorHandler: %d", error_code);
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            MQ.publish(ERROR_TOPIC, "Unknown error code in errorHandler", false);
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
    DEBUGTRACE(INFO, "Application Version = %s", version);
    DEBUGTRACE(INFO, "Device hostname is set to: %s", HOSTNAME);

    pinMode(LED_BUILTIN, OUTPUT);

    // scan for wifi access points
    DEBUGWIFISCAN();

    // attempt to connect and initialise WiFi network
    if (WT.wifiConnect(WIFISSID, WIFIPASS, WIFITIME)) {       // connecting to wifi
        DEBUGTRACE(INFO, "WiFi connected successfully to SSID: %s", WIFISSID);
        tout = ONE_SECOND + millis();    // milliseconds of time to display message
        while (millis() < tout) {
            // any activity you want to do while waiting for wifi to come up
            yield();                         // prevent the watchdog timer doing a reboot
        }
        DEBUGON(true, true, false);          // turn on serial and telnet tracing
    } else {
        errorHandler(NOWIFI, "Can't go on without WiFi connection. Press reset to try again.");
    }

    DEBUGINFO();

    DEBUGTRACE(HEADING, "-------------------------------- Exited setup() --------------------------------");

}


void loop() {

    static char c_num = 'a';
    static unsigned char uc_num = 0x00;
    static int i_num = 0;
    static unsigned int ui_num = 0;
    static long l_num = 0;
    static float f_num = 0;

    DEBUGLOOP();                         // place this anywhere in the loop() routine
    OTA.loopOTA();                       // place this anywhere in the loop() routine

    blinkLED(STD_BLKRATE);

    DEBUGTRACE(HEADING, "------------------------ In loop() executing test cases ------------------------");

    //Cplusplus.com - https://cplusplus.com/reference/cstdio/snprintf
    DEBUGTRACE(INFO, "A text string with one variable:");
    DEBUGTRACE(INFO, "\t%c (signed character)", c_num++);
    DEBUGTRACE(INFO, "\t%c (unsigned character)", uc_num++);
    DEBUGTRACE(INFO, "\t%i (signed integer)", i_num++);
    DEBUGTRACE(INFO, "\t%6d (signed integer, must occupy six spaces)", i_num);
    DEBUGTRACE(INFO, "\t%u (unsigned integer)", ui_num++);
    DEBUGTRACE(INFO, "\t%li (signed long integer)", l_num++);
    DEBUGTRACE(INFO, "\t%o (unsigned octal integer)", ui_num);
    DEBUGTRACE(INFO, "\t%x (unsigned hexadecimal integer, lowercase)", ui_num);
    DEBUGTRACE(INFO, "\t%X (unsigned hexadecimal integer, uppercase)", ui_num);
    DEBUGTRACE(INFO, "\t%.4f (decimal floating point)", f_num = f_num + 0.125);
    DEBUGTRACE(INFO, "\t%.5e (scientific notation, lowercase)", f_num);
    DEBUGTRACE(INFO, "\t%g (use the shortest representation of %%e or %%f)", f_num);
    DEBUGTRACE(INFO, "\t%p (pointer address)", &f_num);

    delay(5000);

}

/*------------------------------------------------------------------------------

Description:
    DEBUGTRACE is makeing use of snprintf.
    snprintf stands for “String print”. Instead of printing on console,
    it store output on character buffer which are specified in snprintf.

    The size of the buffer should be large enough to contain the entire
    resulting string (see snprintf for a safer version).

    A terminating null character is automatically appended after the content written.

Syntax:
    int snprintf(char *buffer, int buffer_size, const char *format-specifier, ...);

    A format specifier follows this prototype:
    %[flags][width][.precision][length]specifier

Return:
    If successful, it returns the total number of characters written excluding
    null-character (aka '\0') appended in the string, in case of failure a
    negative number is returned.

Sources:
    Cplusplus.com - https://cplusplus.com/reference/cstdio/snprintf/
    Variadic function templates in C++ - https://www.geeksforgeeks.org/variadic-function-templates-c/
    Variadic templates in C++ - https://eli.thegreenplace.net/2014/variadic-templates-in-c/

------------------------------------------------------------------------------*/
