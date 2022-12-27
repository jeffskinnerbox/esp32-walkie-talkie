
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.7

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
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#else                            // ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif

#include <Arduino.h>
#include "DeBug.h"
#include "configuration.h"
#include "WiFiHandler.h"

extern DeBug DB;             // declare object DB as external, it is instantiated in DeBug.cpp

AsyncWebServer server(80);   // create AsyncWebServer object on port 80
                             //
// ESP32/ESP8266 Web Server: Control Outputs with Momentary Switch - https://randomnerdtutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/
// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP Pushbutton Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1>ESP Pushbutton Web Server</h1>
    <button class="button" onmousedown="toggleCheckbox('on');" ontouchstart="toggleCheckbox('on');" onmouseup="toggleCheckbox('off');" ontouchend="toggleCheckbox('off');">LED PUSHBUTTON</button>
   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "404 Error: Page Not Found");
}



// ------------------------ Constructors & Destructors -------------------------

// Constructor to create WiFiHandler
WiFiHandler::WiFiHandler(void) {

    ssid = new char[BUF1_SIZE];
    password = new char[BUF1_SIZE];
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

    return true;

}


bool WiFiHandler::Webservices(void) {

    // receive a GET request for '/hello' web page
    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", "<h1>Hello World</h1>");
    });

    // receive a GET request for '/reset' api
    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "ESP32 is being reset");
        ESP.restart();
    });

    // send web page to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    // when receiving a request that isn't understood
    server.onNotFound(notFound);

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
    DEBUGTRACE(INFO, "Attempting connection to WiFi SSID: %s", ssid);
    WiFi.begin(ssid, password);
    WiFi.mode(WIFI_STA);                      // ESP-32 as wifi client / Station Mode (STA)

    // make subsequent connection attempts to wifi
    tout = timeout + millis();                // milliseconds of time given to making connection attempt
    while(WiFi.status() != WL_CONNECTED) {
        DEBUGPRINT(".");
        if (millis() > tout) {
            DEBUGTRACE(ERROR, "Failed to connect to WiFi!  WiFi status exit code is %d", WiFi.status());
            DEBUGTRACE(ERROR, "Timed out after (milliseconds): %d", timeout);
            return false;
        }
        delay(500);
    }

    DEBUGPRINT(".\n\r");
    DEBUGTRACE(INFO, "Successfully connected to WiFi!  IP address is %s", WiFi.localIP());
    DEBUGTRACE(INFO, "WiFi status exit code is %d", WiFi.status());

    mDNSservices();
    Webservices();

    DEBUGTRACE(HEADING, "---------------------------- Leaving wifiConnect() -----------------------------");

    return true;
}


// terminate the wifi connect
void WiFiHandler::wifiTerminate(void) {

    String st = WiFi.SSID();     // convert from sting object to character string

    DEBUGTRACE(INFO, "Disconnecting from WiFi with SSID %s", st.c_str());

    WiFi.disconnect();

}


// perfrom a single scan for nearby networks
void WiFiHandler::wifiScan(void) {

    unsigned char numSsid;
    char buffer[BUF2_SIZE];
    String st;

    DEBUGTRACE(HEADING, "------------------------------ Entered wifiScan() -------------------------------");

    numSsid = WiFi.scanNetworks();
    if (numSsid == 0) {
        DEBUGTRACE(INFO, "\tNo WiFi networks found.");
        return;
    }

    // print the list of networks seen
    DEBUGTRACE(INFO, "Total number of SSIDs found: %d", numSsid);
    DEBUGTRACE(INFO, "\t%-20s\t%s\t%s", "SSID", "RSSI", "Encryp_Type");

    // print the name and characteristics of each network found
    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        st = WiFi.SSID(thisNet);     // convert from sting object to character string
        switch(WiFi.encryptionType(thisNet)) {
            case 2:
                snprintf(buffer, BUF2_SIZE, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "TKIP (WPA)");
                break;
            case 4:
                snprintf(buffer, BUF2_SIZE, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "CCMP (WPA)");
                break;
            case 5:
                snprintf(buffer, BUF2_SIZE, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "WEP");
                break;
            case 7:
                snprintf(buffer, BUF2_SIZE, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "NONE");
                break;
            case 8:
                snprintf(buffer, BUF2_SIZE, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "AUTO");
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
        DEBUGTRACE(INFO, "Starting UDP connection.  Using local port %d", UDPport);
        return true;
    } else {
        DEBUGTRACE(ERROR, "Failed to start UDP listener.");
        return false;
    }

}


// stop listening for UDP messages on port UDPport
void WiFiHandler::udpStop() {

    DEBUGTRACE(INFO, "Stopping listening UDP on local port %d", UDPport);
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
        DEBUGTRACE(WARN, "trtn1 = %u", rtn1);
        DEBUGTRACE(WARN, "trtn2 = %u", rtn2);
        DEBUGTRACE(WARN, "tbytes_rtn = %u", bytes_rtn);
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

