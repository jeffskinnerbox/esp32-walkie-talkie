
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/

#pragma once                // compiler to skip subsequent includes of this file


// ESP8266 libraries (~/.arduino15/packages/esp8266)
//#include <WiFiUdp.h>
//#include <ESP8266WiFi.h>


class CardHandler {
  private:
    //-------------- private variables -------------
    short sck_pin = SCK_PIN;        // pin used to provide xxx (SCK)
    short miso_pin = MISO_PIN;      // pin used to provide xxx (MISO)
    short mosi_pin = MOSI_PIN;      // pin used to provide xxx (MOSI)
    short cs_pin = CS_PIN;          // pin used to provide xxx (CS)

    uint8_t cardType;               // type of SD Card: MMC, SDSC, SDHC
    uint64_t cardSize;              // SD Card's data capacity in mega-bytes

    //-------------- private methods ---------------
    void errorHandler(int, char *);
    bool initSDCard();

  public:
    //-- constructors & destructors for the class --
    CardHandler(void);
    ~CardHandler(void);

    //--------------- public methods ---------------
};

