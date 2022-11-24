
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
            DEBUGTRACE(ERROR, "Unknown error code in errorHandler: ", error_code);
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            MQ.publish(ERROR_TOPIC, "Unknown error code in errorHandler", false);
            Serial.flush();                  // make sure serial messages are posted
            ESP.restart();
    }
}



//-------------------------------- MQTT Routines -------------------------------

//void SubscriptionCallback(char* topic, byte* payload, unsigned int length) {
void SubscriptionCallback(char* topic, unsigned char* payload, unsigned int length) {

    DEBUGPRINT("\r\n");
    DEBUGTRACE(INFO, "Message arrived on topic = ", topic);

    snprintf(msg, length + 1, "%s", payload);
    DEBUGTRACE(INFO, "MQTT message = ", msg);

    /*switch((char)payload[0]) {*/
        /*case '1':          // toggle the nodemcu red LED to blinking or off*/
            /*if (blinkLED) {*/
                /*blinkLED = false;*/
                /*nodemcuLED = HIGH;*/
                /*digitalWrite(LED, nodemcuLED);   // make sure nodmcu red led is 'off'*/
            /*} else {*/
                /*blinkLED = true;*/
            /*}*/
            /*break;*/
        /*case '2':          // write to topic how much the clock has drifted from the ntp time server*/
            /*TimeDriftCheck();*/
            /*break;*/
        /*case '3':          // forced time refresh*/
            /*TimeRefresh(NULL);*/
            /*break;*/
        /*default:*/
            /*errorHandler(UNKNMQTT, "MQTT message unknown.  No action taken.");*/
    /*}*/
}


/*bool MQTTPublishMsg(char *topic, char* payload) {*/

    /*// if not connect to mqtt broker, then reconnect*/
    /*if (!MQ.connected()) {*/
        /*MQ.reconnect();*/
    /*}*/

    /*// publish message*/
    /*MQ.publish(topic, payload, true);*/

/*    // if not connect to mqtt broker, then reconnect*/
    /*if (!MQ.connected()) {*/
        /*MQ.reconnect();*/
    /*}*/

    /*// format message for sending*/
    /*if (status24hour)*/
        /*snprintf(msg, MSGSIZE, "%02d:%02d:%02d", displayHours, displayMinutes, displaySeconds);*/
    /*else*/
        /*snprintf(msg, MSGSIZE, "%02d:%02d:%02d%s", displayHours, displayMinutes, displaySeconds, displayAMPM);*/

    /*// publish message*/
    /*MQ.publish(TIMETOPIC, msg, true);*/

/*}*/



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
        DEBUGON(true, true, false);          // turn on serial and telnet tracing

        OTA.setupOTA();                      // should always follow 'DEBUGSETUP()' and after wifi is up
        MQ.reconnect();                      // connect to mqtt broker

        MQ.setServer(mqtt_server, mqtt_port); // set your mqtt broker to be used
        MQ.setCallback(SubscriptionCallback); // set the callback for subscribe topic
    } else {
        errorHandler(NOWIFI, "Can't go on without WiFi connection. Press reset to try again.");
    }

    DEBUGTRACE(HEADING, "-------------------------------- Exited setup() --------------------------------");

}


void loop() {

    DEBUGLOOP();                         // place this anywhere in the loop() routine
    OTA.loopOTA();                       // place this anywhere in the loop() routine

    if (blinkLED(STD_BLKRATE))
        DEBUGTRACE(INFO, "Hello World 5");

    // allow the client to process mqtt subscriptions, publish, and refresh connection
    MQ.loop();                           // place this anywhere in the loop() routine

}
