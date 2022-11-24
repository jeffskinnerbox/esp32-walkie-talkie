
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/


#define TDEBUG  true       // activate trace message printing for debugging


#ifdef ESP32
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESPmDNS.h>
#include <ESPAsyncWebServer.h>
#else                            // ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncWebServer.h>
#endif

// Arduino libraries (~/src/arduino/libraries)

// Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// simple-display project's include files (~/src/scrolling-display/test/simple-display)
#include "DeBug.h"
#include "secrets.h"
#include "WiFiHandler.h"

#define BUF1 25
#define BUF2 50

extern DeBug DB;             // declare object DB as external, it is instantiated in DeBug.cpp

AsyncWebServer server(80);   // create AsyncWebServer object on port 80



// ------------------------ Constructors & Destructors -------------------------

// Constructor to create WiFiHandler
WiFiHandler::WiFiHandler(void) {

    ssid = new char[BUF1];
    password = new char[BUF1];
    timeout = 10000;              // time out for wifi access request

}


// Destructor to delete WiFiHandler
WiFiHandler::~WiFiHandler(void) {

    delete [] ssid;
    delete [] password;

}



//------------------------------ Private Methods -------------------------------

bool WiFiHandler::mDNSservices(void) {

    if(!MDNS.begin(HOSTNAME)) {
        DEBUGTRACE(ERROR, "Error encountered while starting mDNS");
        return false;
    }

    // route for '/hello' web page
    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Hello World");
    });

    // start the esp web server
    server.begin();

    return true;

}



//------------------------------- Public Methods -------------------------------

// connect to wifi
bool WiFiHandler::wifiConnect(char *id, char *pass, unsigned long tout) {

    ssid = id;
    password = pass;
    timeout = tout;

    DEBUGTRACE(HEADING, "---------------------------- Entered wifiConnect() -----------------------------");

    // attempt first connect to a WiFi network
    DEBUGTRACE(INFO, "Attempting connection to WiFi SSID ", ssid);
    WiFi.begin(ssid, password);
    WiFi.mode(WIFI_STA);                      // ESP-32 as wifi client / Station Mode (STA)

    // make subsequent connection attempts to wifi
    tout = timeout + millis();                // milliseconds of time given to making connection attempt
    while(WiFi.status() != WL_CONNECTED) {
        DEBUGPRINT(".");
        if (millis() > tout) {
            DEBUGTRACE(ERROR, "Failed to connect to WiFi!  WiFi status exit code is ", WiFi.status());
            DEBUGTRACE(ERROR, "Timed out after (milliseconds): ", timeout);
            return false;
        }
        delay(500);
    }

    DEBUGPRINT(".\n\r");
    DEBUGTRACE(INFO, "Successfully connected to WiFi!  IP address is ", WiFi.localIP());
    DEBUGTRACE(INFO, "WiFi status exit code is ", WiFi.status());

    mDNSservices();

    DEBUGTRACE(HEADING, "---------------------------- Leaving wifiConnect() -----------------------------");

    return true;
}


// terminate the wifi connect
void WiFiHandler::wifiTerminate(void) {

    DEBUGTRACE(INFO, "Disconnecting from WiFi with SSID ", WiFi.SSID());

    WiFi.disconnect();

}


// perfrom a single scan for nearby networks
void WiFiHandler::wifiScan(void) {

    unsigned char numSsid;
    char buffer[BUF2];
    String st;

    DEBUGTRACE(HEADING, "------------------------------ Entered wifiScan() -------------------------------");

    numSsid = WiFi.scanNetworks();
    if (numSsid == 0) {
        DEBUGTRACE(INFO, "\tNo WiFi networks found.");
        return;
    }

    // print the list of networks seen
    DEBUGTRACE(INFO, "Total number of SSIDs found: ", numSsid);
    snprintf(buffer, BUF2, "\t%-20s\t%s\t%s", "SSID", "RSSI", "Encryp_Type");
    DEBUGTRACE(INFO, buffer);

    // print the name and characteristics of each network found
    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        st = WiFi.SSID(thisNet);     // convert from sting object to character string
        switch(WiFi.encryptionType(thisNet)) {
            case 2:
                snprintf(buffer, BUF2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "TKIP (WPA)");
                break;
            case 4:
                snprintf(buffer, BUF2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "CCMP (WPA)");
                break;
            case 5:
                snprintf(buffer, BUF2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "WEP");
                break;
            case 7:
                snprintf(buffer, BUF2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "NONE");
                break;
            case 8:
                snprintf(buffer, BUF2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "AUTO");
                break;
            default:
                DEBUGTRACE(ERROR, "Returned improper encryption type during WiFi scan.");
                break;
        }

        DEBUGTRACE(INFO, buffer);
    }

    DEBUGTRACE(HEADING, "------------------------------ Leaving wifiScan() -------------------------------");
    delay(5000);   // TAKE THIS OUT!!

}


// perfrom multiple scan for nearby networks, remove redundent scans, delay a few
// milli-seconds between scans, and then return a structure containing all that you found
void WiFiHandler::wifiScan(int scan_cnt, int delay) {

    struct wifiAPlist {
        char ssid[20];
        int rssi;
        char encryp[15];
        wifiAPlist *next;
    };

    DEBUGTRACE(NOOP, "Not implemented yet");
}



//--------------------------------- UDP Methods --------------------------------

void WiFiHandler::udpSetPort(unsigned int port) {
    UDPport = port;
}


// start listening for UDP messages on port UDPport
bool WiFiHandler::udpStart() {

    if (udp.begin(UDPport)) {
        DEBUGTRACE(INFO, "Starting UDP connection.  Using local port ", UDPport);
        return true;
    } else {
        DEBUGTRACE(ERROR, "Failed to start UDP listener.");
        return false;
    }

}


// stop listening for UDP messages on port UDPport
void WiFiHandler::udpStop() {

    DEBUGTRACE(INFO, "Stopping listening UDP on local port ", UDPport);
    udp.stop();

}


bool WiFiHandler::udpCheck(void) {
    unsigned int rtn;

    rtn = udp.parsePacket();

    if (!rtn)
        DEBUGTRACE(WARN, "No UDP packets are available");

    return rtn;

}


//bool WiFiHandler::udpRequest(IPAddress& address, unsigned int port, byte *packetbuffer, unsigned int len) {
bool WiFiHandler::udpRequest(IPAddress& address, unsigned int port, unsigned char *packetbuffer, unsigned int len) {
    unsigned int rtn1, rtn2, bytes_rtn;

    rtn1 = udp.beginPacket(address, port);      // request IP address and port
    bytes_rtn = udp.write(packetbuffer, len);   // buffer containing the request return
    rtn2 = udp.endPacket();

    if (rtn1 == 0 || rtn2 == 0 || bytes_rtn == 0) {
        DEBUGTRACE(WARN, "UDP request failed");
        DEBUGTRACE(WARN, "trtn1 = ", rtn1);
        DEBUGTRACE(WARN, "trtn2 = ", rtn2);
        DEBUGTRACE(WARN, "tbytes_rtn = ", bytes_rtn);
        return false;
    }

    return true;

}


//int WiFiHandler::udpRead(byte *packetbuffer, unsigned int len) {
int WiFiHandler::udpRead(unsigned char *packetbuffer, unsigned int len) {
    int rtn;

    // This function can only be successfully called after WiFiUDP.parsePacket()
    rtn = udp.read(packetbuffer, len);      // read the packet into the buffer

    return rtn;

}

