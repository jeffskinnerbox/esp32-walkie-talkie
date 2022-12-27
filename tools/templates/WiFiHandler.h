
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.7

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

#pragma once                        // compiler to skip subsequent includes of this file


// ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <WiFiUdp.h>


class WiFiHandler {
  private:
    //-------------- private variables -------------
    char *ssid = NULL;               // memory array used to store wifi ssid
    char *password = NULL;           // memory array used to store wifi password
    unsigned long timeout;           // time out for wifi access request

    // udp parameters
    unsigned int UDPport = 2390;     // local port to listen for UDP packets
    WiFiUDP udp;                     // A UDP object to send/receive packets

    //-------------- private methods ---------------
    bool mDNSservices(void);
    bool Webservices(void);

  public:
    //-- constructors & destructors for the class --
    WiFiHandler(void);
    ~WiFiHandler(void);

    //--------------- public methods ---------------
    // public methods for wifi
    void wifiScan(void);
    void wifiScan(int, int);
    void wifiTerminate(void);
    bool wifiConnect(char *, char *, unsigned long);

    // public methods for udp
    bool udpStart(void);
    void udpStop(void);
    bool udpCheck(void);
    void udpSetPort(unsigned int);
    //int udpRead(byte *, unsigned int);
    int udpRead(unsigned char *, unsigned int);
    //bool udpRequest(IPAddress&, unsigned int, byte *, unsigned int);
    bool udpRequest(IPAddress&, unsigned int, unsigned char *, unsigned int);
};

// -----------------------------------------------------------------------------

