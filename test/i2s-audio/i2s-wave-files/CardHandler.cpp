
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION:

REFERENCE MATERIALS:

CREATED BY:
    jeffskinnerbox@yahoo.com
------------------------------------------------------------------------------*/


#define TDEBUG  true       // activate trace message printing for debugging

# define INIT_FAILED 1
# define BUFSIZE 512

// arduino and esp include files
#include <SPI.h>
#include <FS.h>
#include <SD.h>

// this project's include files
#include "DeBug.h"
#include "configuration.h"
#include "CardHandler.h"



//--------------------------- Global Scoped Variables --------------------------

extern DeBug DB;           // object is external, and instantiated in DeBug.cpp



// ------------------------ Constructors & Destructors -------------------------

// Constructor to create CardHandler
CardHandler::CardHandler(void) {

    if (!initSDCard())
        errorHandler(INIT_FAILED, "SD Card initialization failure");

}


// Destructor to delete CardHandler
CardHandler::~CardHandler(void) {
}


//------------------------------ Private Methods -------------------------------

// blink led to show 'sign of life' and other status
bool CardHandler::blinkLED(unsigned long rate) {

    unsigned long currentTime = millis();          // initialized everytime you enter this routine
    static unsigned long previousTime = millis();  // initialized first time you enter this routine

    // blink LED at the rate specified
    if (currentTime - previousTime >= rate) {
        previousTime = currentTime;                            // save the last time you blinked the LED
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  // toggle led state
        return true;
    }

    return false;

}


// handle errors by displaying a code and then taking action (e.g. restart)
void CardHandler::errorHandler(int error_code, char *msg) {

    int i = 0;
    unsigned long tout;                      // time-out time

    DEBUGTRACE(INFO, "In errorHandler() and error_code = %d\n\r", error_code, DEC);

    switch(error_code) {
        case INIT_FAILED:
            DEBUGTRACE(ERROR, msg);
            DEBUGTRACE(FATAL, "An automatic restart will be requested.");

            tout = FIVE_SECOND + millis();    // milliseconds of time to display message
            while (millis() < tout) {
                // any activity you want to do while waiting
                //yield();                     // prevent the watchdog timer from doing a reboot
                blinkLED(FAST_BLKRATE);
            }

            // nothing can be done so restart
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.restart();
            break;
        default:
            // nothing can be done so restart
            DEBUGTRACE(ERROR, "Unknown error code in errorHandler: ", error_code);
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.restart();
    }
}


// initialize the SD card for use
bool CardHandler::initSDCard(void) {

    uint8_t cardType;
    char buffer[BUF1_SIZE];

    // initialize spi communication protocol on the pins
    pinMode(CS_PIN, OUTPUT);      // the cs pin must be set to output
    digitalWrite(CS_PIN, HIGH);
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);

    // initiate the sd card reader
    if (!SD.begin(CS_PIN)) {
        DEBUGTRACE(ERROR, "SD Card mount failure");
        return false;
    } else {
        cardType = SD.cardType();
        DEBUGTRACE(INFO, "SD.cardType = %d\n\r", cardType, DEC);
    }

    switch(cardType) {
        case CARD_NONE:
            DEBUGTRACE(WARN, "No SD Card attached");
            return false;
            break;
        case CARD_MMC:
            DEBUGTRACE(INFO, "SD Card type: MMC");
            break;
        case CARD_SD:
            DEBUGTRACE(INFO, "SD Card type: SDSC");
            break;
        case CARD_SDHC:
            DEBUGTRACE(INFO, "SD Card type: SDHC");
            break;
        default:
            DEBUGTRACE(WARN, "SD Card type: unknown");
            return false;
    }

    sprintf(buffer, "SD Card size: %lluMB", SD.cardSize() / bytes_per_megabytes);
    DEBUGTRACE(INFO, buffer);

    sprintf(buffer, "SD Card used space: %lluMB", SD.usedBytes() / bytes_per_megabytes);
    DEBUGTRACE(INFO, buffer);

    return true;

}

//------------------------------- Public Methods -------------------------------

// The first reference to 'fs' ('fs::') is a namespace in which the class 'SDFS' is defined.
// So '::' is a scope qualifier.  '&fs' indicates that the argument required is a pointer.
// Also using 'fs' as the name of the argument.  So, if you call listDir(),
// the first argument has to be a pointer to an object of type 'fs::SDFS'.
void CardHandler::listDir(fs::SDFS &fs, const char * dirname, uint8_t levels) {

    Serial.printf("Listing directory: %s\n\r", dirname);
    File root = fs.open(dirname);

    if (!root) {
        Serial.println("Failed to open directory");
        return;
    }

    if (!root.isDirectory()) {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("DIR : ");
            Serial.println(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            Serial.print("   FILE NAME: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }

}


void CardHandler::createDir(fs::SDFS &fs, const char * path) {

    Serial.printf("Creating Dir: %s\n\r", path);

    if (fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("Dir creation failed");
    }

}


void CardHandler::removeDir(fs::SDFS &fs, const char * path) {

    Serial.printf("Removing Dir: %s\n\r", path);

    if (fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("Dir removal failed");
    }

}


// Append data to the SD card
void CardHandler::appendFile(fs::SDFS &fs, const char *path, const char *message) {

    Serial.printf("Appending to file: %s\n\r", path);
    File file = fs.open(path, FILE_APPEND);

    if (!file) {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message)) {
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();

}


void CardHandler::readFile(fs::SDFS &fs, const char * path) {

    Serial.printf("Reading from file: %s\n\r", path);

    File file = fs.open(path);
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read text from file: ");
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();

}


// Write to the SD card
void CardHandler::writeFile(fs::SDFS &fs, const char * path, const char * message) {

    Serial.printf("Writing file: %s\n\r", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message)) {
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();

}


void CardHandler::renameFile(fs::SDFS &fs, const char * path1, const char * path2) {

    Serial.printf("Renaming file %s to %s\n\r", path1, path2);

    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }

}


void CardHandler::deleteFile(fs::SDFS &fs, const char * path) {

    Serial.printf("Deleting file: %s\n\r", path);

    if (fs.remove(path)) {
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }

}


void CardHandler::testFileIO(fs::SDFS &fs, const char * path) {

    File file = fs.open(path);
    static uint8_t buf[BUFSIZE];

    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;

    if (file) {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len) {
            size_t toRead = len;
            if(toRead > BUFSIZE) {
                toRead = BUFSIZE;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n\r", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }

    file = fs.open(path, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }

    start = millis();
    for (size_t i=0; i<2048; i++) {
        file.write(buf, BUFSIZE);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n\r", 2048 * BUFSIZE, end);
    file.close();

}
