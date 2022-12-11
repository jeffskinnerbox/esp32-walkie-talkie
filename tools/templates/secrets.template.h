
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.6

DESCRIPTION:
    This is a template for the secrets.h file that this program needs to operate.

    Use this use secrets.h to store all of the private credentials and connection
    details, such as, WiFi credentials and passwords

REFERENCE MATERIALS:
    How to safely store Arduino secrets - https://www.andreagrandi.it/2020/12/16/how-to-safely-store-arduino-secrets/

WARNING:
   Make sure to reference 'secrets.h' in .gitignore so that this
   information isn't posted to the web.

CREATED BY:
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */


#pragma once                           // compiler to skip subsequent includes of this file

//------------------------------- WiFi Parameters ------------------------------

// wifi ssid, password, and parameters
#define WIFISSID  "<my-wifi-ssid>"     // wifi ssid
#define WIFIPASS  "<my-wifi-password>" // wifi password key
#define WIFITIME  10000                // attempt to connect with wifi for 10 seconds then abandon

//------------------------------- MQTT Parameters ------------------------------

// parameters used by mqtt
#define MQTTSERVER "<mqtt-broker>"     // mqtt broker (aka server)
#define MQTTCLIENT "<mqtt-client>"     // mqtt client name
#define MQTTPORT   1883                // port used by the mqtt broker

// topics used by mqtt client for publishing and subscribing
#define OUTTOPIC   "pubTopic"          // mqtt broker topic for publishing
#define INTOPIC    "subTopic"          // mqtt broker topic for subscribing
#define MSGTOPIC   OUTTOPIC "/msg"     // mqtt broker topic current message displayed
#define ERRORTOPIC OUTTOPIC "/error"   // mqtt broker topic on application errors
#define MSGTOPIC   INTOPIC  "/msg"     // mqtt broker topic to add new message for display
#define CMDTOPIC   INTOPIC  "/command" // mqtt broker topic for sending commands to application
#define TIMETOPIC  INTOPIC  "/time"    // mqtt broker topic for recieving time
#define DATETOPIC  INTOPIC  "/date"    // mqtt broker topic for recieving date
#define DDAYTOPIC  INTOPIC  "/day"     // mqtt broker topic for recieving day

//------------------------------- OTA Parameters -------------------------------

// ota upload parameters
#ifdef ESP32
#define OTAPORT 3232                   // ota port used for upload on ESP32
#define OTAPASSWORD "<ota-password>"   // ota password
#else
#define OTAPORT 8266                   // ota port used for upload on ESP8266
#define OTAPASSWORD "<ota-password>"   // ota password
#endif

// -----------------------------------------------------------------------------

