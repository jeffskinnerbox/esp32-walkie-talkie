
#define SUBROUTINE_TEST true      // used to see if SD code can be move from setup() to subroutine

#define CS_PIN   GPIO_NUM_5        // chip select (aka ss = slave select)
#define SCK_PIN  GPIO_NUM_18       // clock (aka sclk)
#define MOSI_PIN GPIO_NUM_23       // master output / slave input
#define MISO_PIN GPIO_NUM_19       // master input / slave output
#define TEN_SECOND 10000UL         // milliseconds in ten second
#define SERIAL_SPEED 9600          // bit rate speed of serial interface

// arduino and esp include files
#include <FS.h>
#include <SD.h>
#include <SPI.h>


#if SUBROUTINE_TEST
// initialize the SD card for use
bool initSDCard(void) {

    const uint64_t bytes_per_megabytes = 1024 * 1024;
    uint8_t cardType;

    // initialize spi communication protocol on the pins
    if (!SD.begin(CS_PIN)) {
        Serial.println("SD Card mount failure");
        return false;
    } else {
        cardType = SD.cardType();
        Serial.printf("cardType = %d\n\r", cardType);
    }

    // echck for errors
    switch(cardType) {
        case CARD_NONE:
            Serial.println("No SD Card attached");
            return false;
            break;
        case CARD_MMC:
            Serial.println("SD Card type: MMC");
            break;
        case CARD_SD:
            Serial.println("SD Card type: SDSC");
            break;
        case CARD_SDHC:
            Serial.println("SD Card type: SDHC");
            break;
        default:
            Serial.println("SD Card type: unknown");
            return false;
    }

    Serial.printf("SD Card size: %lluMB\n\r", (int64_t)SD.cardSize() / bytes_per_megabytes);
    Serial.printf("SD Card used space: %lluMB\n\r", (int64_t)SD.usedBytes() / bytes_per_megabytes);

    return true;

}
#else
#endif

//------------------------------- Main Routines --------------------------------

void setup() {

    const uint64_t bytes_per_megabytes = 1024 * 1024;
    uint8_t cardType;

    // initialize spi communication protocol on the pins
    pinMode(CS_PIN, OUTPUT);      // the cs pin must be set to output
    digitalWrite(CS_PIN, HIGH);
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);

    Serial.begin(SERIAL_SPEED);   // always start Serial first so it can be used by DeBug
    while (!Serial) {}            // wait for serial port to connect

    Serial.printf("\n\n\rSUBROUTINE_TEST = %d\n\n\r", SUBROUTINE_TEST);

#if SUBROUTINE_TEST
    initSDCard();
#else
    // initialize spi communication protocol on the pins
    if (!SD.begin(CS_PIN)) {
        Serial.println("SD Card mount failure");
    } else {
        cardType = SD.cardType();
        Serial.printf("SD.cardType = %d\n\r", cardType);
    }

    // check for card type errors
    switch(cardType) {
        case CARD_NONE:
            Serial.println("No SD Card attached");
            break;
        case CARD_MMC:
            Serial.println("SD Card type: MMC");
            break;
        case CARD_SD:
            Serial.println("SD Card type: SDSC");
            break;
        case CARD_SDHC:
            Serial.println("SD Card type: SDHC");
            break;
        default:
            Serial.println("SD Card type: unknown");
    }

    Serial.printf("SD Card size: %lluMB\n\r", (int64_t)SD.cardSize() / bytes_per_megabytes);
    Serial.printf("SD Card used space: %lluMB\n\r", (int64_t)SD.usedBytes() / bytes_per_megabytes);
#endif

}


void loop() {

    Serial.println("In the loop()");
    delay(TEN_SECOND);

}
