
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5

DESCRIPTION

SOURCES

DOCUMENTATION
    For design patterns used, check out ~/src/arduino/sketchbooks/libraries/PubSubClient/src/*

CREATED BY
    jeffskinnerbox@yahoo.com

------------------------------------------------------------------------------ */


#define TDEBUG  true       // activate trace message printing for debugging



//------------------------------- Include Files --------------------------------

#ifdef ESP32
#include <WiFi.h>
#include <WiFiUdp.h>
#else                     // found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#endif

#include <PubSubClient.h> // Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

#include "DeBug.h"
#include "configuration.h"
#include "MQTTHandler.h"



//---------------------- Static Scoped Macros & Variables ----------------------



//--------------------------- Global Scoped Variables --------------------------

extern DeBug DB;            // declare object DB as external, it is instantiated in DeBug.cpp
//extern MQTTHandler MQ;      // declare object MQ as external, and member of class MQTTHandler



// ------------------------ Constructors & Destructors -------------------------

// Constructor to create MQTTHandler
MQTTHandler::MQTTHandler(char *server, int port) {

    this->client = NULL;
    this->server = NULL;
    setCallback(NULL);

    mqtt_client = PubSubClient(espClient);
    setServer(server, port);

}


// Destructor to delete MQTTHandler
MQTTHandler::~MQTTHandler(void) {
}



// ------------------------------ Private Methods ------------------------------



// ------------------------------- Public Methods ------------------------------

// see https://pubsubclient.knolleary.net/api#state
int MQTTHandler::state(void) {
    int rtn;

    rtn = mqtt_client.state();
    DEBUGTRACE(INFO, "Current state of the MQTT client = %d", rtn);

    switch(rtn) {
        case -4:
            DEBUGTRACE(WARN, "MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time");
            break;
        case -3:
            DEBUGTRACE(WARN, "MQTT_CONNECTION_LOST - the network connection was broken");
            break;
        case -2:
            DEBUGTRACE(WARN, "MQTT_CONNECT_FAILED - the network connection failed");
            break;
        case -1:
            DEBUGTRACE(INFO, "MQTT_DISCONNECTED - the client is disconnected cleanly");
            break;
        case 0:
            DEBUGTRACE(INFO, "MQTT_CONNECTED - the client is connected");
            break;
        case 1:
            DEBUGTRACE(ERROR, "MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT");
            break;
        case 2:
            DEBUGTRACE(ERROR, "MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier");
            break;
        case 3:
            DEBUGTRACE(ERROR, "MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection");
            break;
        case 4:
            DEBUGTRACE(ERROR, "MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected");
            break;
        case 5:
            DEBUGTRACE(ERROR, "MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect");
            break;
        default:
            DEBUGTRACE(ERROR, "MQTT client state unknown!");
    }

    return rtn;

}


// called regularly to allow the client to process incoming messages and maintain its connection to the server
// false = the client is no longer connected    true = the client is still connected
boolean MQTTHandler::loop(void) {
    boolean rtn;

    rtn = mqtt_client.loop();

    if (rtn == false)
        DEBUGTRACE(INFO, "The MQTT client is no longer connected to the broker.");

    return rtn;
}


boolean MQTTHandler::connected(void) {
    return mqtt_client.connected();
}


boolean MQTTHandler::connect(char *clientID) {
    return mqtt_client.connect(clientID);
}


boolean MQTTHandler::reconnect(void) {
    boolean rtn;

    if (!mqtt_client.connected()) {
        DEBUGTRACE(INFO, "Attempting broker reconnection.");

        // Create a random client ID
        String clientId = MQTTCLIENT;
        DEBUGTRACE(INFO, "MQTT Client ID (clientId) = %s", clientId);

        // Attempt to connect to mqtt broker
        if (mqtt_client.connect((char *)clientId.c_str())) {
            DEBUGTRACE(INFO, "Reconnected to MQTT broker");

            // Once connected, publish an announcement
            mqtt_client.publish(ERROR_TOPIC, "reconnected to broker", false);

            // and then resubscribe
            mqtt_client.subscribe(CMD_TOPIC);

            rtn = true;
        } else {
            DEBUGTRACE(WARN, "Reconnect failed,  return code = %d", mqtt_client.state());
            rtn = false;
        }
    } else {
        DEBUGTRACE(INFO, "Already connected to broker");
        rtn = true;
    }

    return rtn;

}


// returns the client instance, allowing the function to be chained
PubSubClient& MQTTHandler::setServer(char *s, int p) {

    port = p;
    server = s;

    return mqtt_client.setServer(server, port);  // set your mqtt broker to be used

}


// set the callback for subscribe topic
PubSubClient& MQTTHandler::setCallback(MQTT_CALLBACK_SIGNATURE) {
    return mqtt_client.setCallback(callback);
}


// set your mqtt client to be used
PubSubClient& MQTTHandler::setClient(WiFiClient c) {
    return mqtt_client.setClient(c);
}


boolean MQTTHandler::subscribe(const char* topic) {
    return mqtt_client.subscribe(topic);
}


// qos is 0 or 1 only
boolean MQTTHandler::subscribe(const char* topic, uint8_t qos) {
    return mqtt_client.subscribe(topic, qos);
}


boolean MQTTHandler::unsubscribe(const char* topic) {
    return mqtt_client.subscribe(topic);
}


boolean MQTTHandler::publish(const char* topic, const char* payload) {
    return mqtt_client.publish(topic, payload);
}


boolean MQTTHandler::publish(const char* topic, const char* payload, boolean retained) {
    return mqtt_client.publish(topic, payload, retained);
}


boolean MQTTHandler::publish(const char* topic, const uint8_t* payload, unsigned int plength) {
    return mqtt_client.publish(topic, payload, plength);
}


boolean MQTTHandler::publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    return mqtt_client.publish(topic, payload, plength, retained);
}


boolean MQTTHandler::publish_P(const char* topic, const char* payload, boolean retained) {
    return mqtt_client.publish_P(topic, payload, retained);
}


boolean MQTTHandler::publish_P(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained) {
    return mqtt_client.publish_P(topic, payload, plength, retained);
}

