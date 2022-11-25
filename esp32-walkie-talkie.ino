
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5

DESCRIPTION:
    Demonstration program showing the the use of the MD_Parola library display
    scrolling features.

    User can enter text through a web browser and this will display as a
    scrolling message on the display. Some parameters for the text can also
    be controlled from the web page.

    The IP address for the ESP8266 is displayed on the scrolling display
    after startup initialisation and connected to the WiFi network.

PHYSICAL DESIGN:
    Hardware
        MAX7219 Dot Matrix Module 4-in-1 Display - https://www.banggood.com/3Pcs-MAX7219-Dot-Matrix-Module-4-in-1-Display-Screen-For-Arduino-p-1230975.html
        HiLetgo New Version ESP8266 NodeMCU LUA CP2102 ESP-12E - https://www.amazon.com/gp/product/B010O1G1ES

    Wiring
        Connections for ESP8266 hardware SPI are:
        MAX72XX Pin    ESP8266 Pin    Notes / Comments
        Vcc              3v3           LED matrices seem to work at 3.3V
        GND              GND           Ground
        DIN              D7            HSPID or HMOSI
        CS or LD         D8            HSPICS or HCS
        CLK              D5            CLK or HCLK

MONITOR:
    To monitor the scrolling display's activities on the NodeMCU serial ports while operating,
    connection the USB port to a Linux computer and execute the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter: CNTR-a :quit
    To stop the screen scrolling, enter: CNTR-a :

    To monitor using telnet, ececute the following:

        telnet ESP_24F9FB          # using esp8266 hostname ('ESP_' + chip ID)
        telnet 192.168.1.47        # using esp8266 ip address
        telnet scrolling-display.local

    To terminate telnet monitoring, enter: Ctrl-] quit

    To find the device on your LAN:

        sudo arp-scan 192.168.1.200/24 | grep Espressif

TESTING
    For testing, you may want to "see" the messages posted to the display.

    To test the MQTT capabilities, use a public MQTT broker site
    and Mosquitto clients like this:

        mosquitto_sub -v -h broker.mqtt-dashboard.com -t "outTopic-ntp-clock/time"
        mosquitto_sub -v -h broker.mqtt-dashboard.com -t "outTopic-test-scrolling-display/msg"

        mosquitto_pub -h broker.mqtt-dashboard.com -t "inTopic-test-scrolling-display/msg-queue" -m "test message"
        mosquitto_pub -h broker.mqtt-dashboard.com -t "inTopic-test-scrolling-display/command" -m "2"


USAGE:
    xxxxxxxxxx

REFERENCE MATERIALS:
    * MD_MAX72XX library can be found at https://github.com/MajicDesigns/MD_MAX72XX
    https://lastminuteengineers.com/esp8266-ota-updates-arduino-ide/
    https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/

SOURCES:
    Code adapted from https://github.com/MajicDesigns/MD_Parola/tree/master/examples/Parola_Scrolling_ESP8266
    Code adapted from https://randomnerdtutorials.com/esp8266-ota-updates-with-arduino-ide-over-the-air/
    https://tttapa.github.io/ESP8266/Chap13%20-%20OTA.html
    ~/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino

CREATED BY:
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------*/

#define TDEBUG  true       // activate trace message printing for debugging


// found in ESP32 libraries (~/.arduino15/packages/esp32)
//#include <Arduino.h>

// this project's include files
#include "DeBug.h"
#include "Application.h"


//---------------------------- Static Scoped Macros ----------------------------

// version stamp
#define VER "esp32-walkie-talkie" " - "  __DATE__ " at " __TIME__
static char version[] = VER;


//-------------------------- Static Scoped Variables ---------------------------

extern DeBug DB;         // declare object DB as external, and member of class DeBug
//extern OTAHandler OTA;   // declare object OTA as external, and member of class OTAHandler

// wifi and mqtt handlers / object constructor
//WiFiHandler WT = WiFiHandler();
//MQTTHandler handlerMQTT = MQTTHandler(mqtt_server, mqtt_port);

// creation of the application
Application *application;



//------------------------------- Main Routines --------------------------------

void setup() {
    // always start Serial first so it can be used by DeBug
    Serial.begin(9600);
    while (!Serial) {}                                   // wait for serial port to connect

    DEBUGSETUP();                                        // should be right after 'Serial.begin'
    DEBUGTRACE(HEADING, "------------------------------- Entered setup() --------------------------------");
    DEBUGTRACE(INFO, "Application Version = ", version);
    DEBUGSTATUS();                                       // provide information about debug status flags
    DEBUGINFO();                                         // provide some useful information about the microprocessor

    // start up the application
    //application = new Application();
    //application->begin();
    Serial.println("Application started");

    DEBUGTRACE(HEADING, "-------------------------------- Exited setup() --------------------------------");

}


void loop() {

    DEBUGLOOP();                         // place this anywhere in the loop() routine
    //OTA.loopOTA();                       // place this anywhere in the loop() routine
    //MQ.loop();                           // place this anywhere in the loop() routine

    // nothing to do - the application initiated in setup() is doing all the work
    vTaskDelay(pdMS_TO_TICKS(1000));

}

