
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5

DESCRIPTION

SOURCES

DOCUMENTATION
    For design patterns used, check out ~/src/arduino/sketchbooks/libraries/PubSubClient/src

CREATED BY
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

#pragma once                         // compiler to skip subsequent includes of this file


// WiFiClient is not a built-in type like int or char. It's a class that must be
// defined via the header file before you can create an instance of it.
#include <WiFiClient.h>              // ESP8266 libraries (~/.arduino15/packages/esp8266)

class MQTTHandler {
  private:
    //-------------- private variables -------------
    int port = NULL;
    char *server = NULL;             // MQTT brokers (aka server)
    char *client = NULL;             // MQTT client (aka subscriber)
    WiFiClient espClient;            // WiFi client object
    PubSubClient mqtt_client;        // MQTT client object

    //--------------- private methods --------------

  public:
    //-- constructors & destructors for the class --
    MQTTHandler(char *, int);
    ~MQTTHandler(void);

    //--------------- public methods ---------------
    int state(void);
    boolean loop(void);
    boolean connect(char*);
    boolean connected(void);
    boolean reconnect(void);
    PubSubClient& setClient(WiFiClient);
    PubSubClient& setServer(char *server, int port);
    PubSubClient& setCallback(MQTT_CALLBACK_SIGNATURE);
    boolean subscribe(const char* topic);
    boolean subscribe(const char* topic, uint8_t qos);
    boolean unsubscribe(const char* topic);
    boolean publish(const char* topic, const char* payload);
    boolean publish(const char* topic, const char* payload, boolean retained);
    boolean publish(const char* topic, const uint8_t* payload, unsigned int plength);
    boolean publish(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained);
    boolean publish_P(const char* topic, const char* payload, boolean retained);
    boolean publish_P(const char* topic, const uint8_t* payload, unsigned int plength, boolean retained);
};

// -----------------------------------------------------------------------------

