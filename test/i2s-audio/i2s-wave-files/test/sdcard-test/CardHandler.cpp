
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
char stringbuf[BUF3_SIZE];



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

    sprintf(stringbuf, "In errorHandler() and error_code = %d", error_code);
    DEBUGTRACE(INFO, stringbuf);

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

// Unmount SD card
void CardHandler::unMount(fs::SDFS &fs) {

  DEBUGTRACE(INFO, "Unmounting SD card ...");

  // Unmount SD card
  fs.end();

  DEBUGTRACE(INFO, "Unmount done.");

}


// The first reference to 'fs' ('fs::') is a namespace in which the class 'SDFS' is defined.
// So '::' is a scope qualifier.  '&fs' indicates that the argument required is a pointer.
// Also using 'fs' as the name of the argument.  So, if you call listDir(),
// the first argument has to be a pointer to an object of type 'fs::SDFS'.
bool CardHandler::listDir(fs::SDFS &fs, const char * dirname, uint8_t levels) {

    sprintf(stringbuf, "Listing directory: %s", dirname);
    DEBUGTRACE(INFO, stringbuf);

    File root = fs.open(dirname);

    if (!root) {
        DEBUGTRACE(WARN, "    Failed to open directory");
        return false;
    }

    if (!root.isDirectory()) {
        DEBUGTRACE(WARN, "    Not a directory");
        return false;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            sprintf(stringbuf, "    DIR: %s", file.name());
            DEBUGTRACE(INFO, stringbuf);
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            sprintf(stringbuf, "    FILE NAME: %s", file.name());
            DEBUGTRACE(INFO, stringbuf);
            sprintf(stringbuf, "    SIZE: %d", file.size());
            DEBUGTRACE(INFO, stringbuf);
        }
        file = root.openNextFile();
    }

    return true;

}


bool CardHandler::createDir(fs::SDFS &fs, const char * path) {

    if (fs.mkdir(path)) {
        sprintf(stringbuf, "Dir %s created", path);
        DEBUGTRACE(INFO, stringbuf);
        return true;
    } else {
        sprintf(stringbuf, "Dir %s creation failed", path);
        DEBUGTRACE(WARN, stringbuf);
        return false;
    }

}


bool CardHandler::removeDir(fs::SDFS &fs, const char * path) {

    if (fs.rmdir(path)) {
        sprintf(stringbuf, "Dir %s removed", path);
        DEBUGTRACE(INFO, stringbuf);
        return true;
    } else {
        sprintf(stringbuf, "Dir %s removal failed", path);
        DEBUGTRACE(WARN, stringbuf);
        return false;
    }

}


// Append data to the SD card
bool CardHandler::appendFile(fs::SDFS &fs, const char *path, const char *message) {

    bool rtn;
    File file = fs.open(path, FILE_APPEND);

    if (!file) {
        sprintf(stringbuf, "Failed to open file %s for appending", path);
        DEBUGTRACE(WARN, stringbuf);
        return false;
    }

    if (file.print(message)) {
        sprintf(stringbuf, "Message appended to file %s", path);
        DEBUGTRACE(INFO, stringbuf);
        rtn = true;
    } else {
        sprintf(stringbuf, "Message failed to appended to file %s", path);
        DEBUGTRACE(WARN, stringbuf);
        rtn = false;
    }

    file.close();
    return rtn;

}


bool CardHandler::readFile(fs::SDFS &fs, const char * path) {

    File file = fs.open(path);
    if (!file) {
        sprintf(stringbuf, "Failed to open file %s for reading", path);
        DEBUGTRACE(WARN, stringbuf);
        return false;
    }

    sprintf(stringbuf, "Reading text from file %s", path);
    DEBUGTRACE(INFO, stringbuf);
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();

}


// Write to the SD card
bool CardHandler::writeFile(fs::SDFS &fs, const char * path, const char * message) {

    File file = fs.open(path, FILE_WRITE);
    bool rtn;

    if (!file) {
        sprintf(stringbuf, "Failed to open file %s for writing", path);
        DEBUGTRACE(WARN, stringbuf);
        return false;
    }

    if (file.print(message)) {
        sprintf(stringbuf, "File %s written", path);
        DEBUGTRACE(INFO, stringbuf);
        rtn = true;
    } else {
        sprintf(stringbuf, "File %s write failed", path);
        DEBUGTRACE(WARN, stringbuf);
        rtn = false;
    }

    file.close();
    return rtn;

}


bool CardHandler::renameFile(fs::SDFS &fs, const char * path1, const char * path2) {


    if (fs.rename(path1, path2)) {
        sprintf(stringbuf, "Renaming file %s to %s: Successfully renamed", path1, path2);
        DEBUGTRACE(INFO, stringbuf);
        return true;
    } else {
        sprintf(stringbuf, "Renaming file %s to %s: Failed to rename", path1, path2);
        DEBUGTRACE(WARN, stringbuf);
        return false;
    }

}


bool CardHandler::deleteFile(fs::SDFS &fs, const char * path) {

    if (fs.remove(path)) {
        sprintf(stringbuf, "Deleting file %s: File deleted successfully", path);
        DEBUGTRACE(INFO, stringbuf);
        return true;
    } else {
        sprintf(stringbuf, "Deleting file %s: File deletion failed", path);
        DEBUGTRACE(WARN, stringbuf);
        return false;
    }

}


bool CardHandler::testFileIO(fs::SDFS &fs, const char * path) {
    DEBUGTRACE(INFO, "Method \"CardHandler::testFileIO()\" not implemented yet");
    return false;
}
/*bool CardHandler::testFileIO(fs::SDFS &fs, const char * path) {*/

    /*//File file = fs.open(path);*/
    /*File file;*/
    /*static uint8_t buf[BUFSIZE];*/

    /*size_t len = 0;*/
    /*uint32_t start = millis();*/
    /*uint32_t end = start;*/

    /*DEBUGTRACE(INFO, "SD Card File I/O Test:");*/

    /*// check if the file already exists*/
    /*if (fs.exists(path)) {*/
        /*sprintf(stringbuf, "    FILE: %s exists and will be writen over", path);*/
        /*DEBUGTRACE(WARN, stringbuf);*/
    /*} else {*/
        /*sprintf(stringbuf, "    FILE: %s does not exists but will be created", path);*/
        /*DEBUGTRACE(INFO, stringbuf);*/
    /*}*/
    /*file = fs.open(path);*/

    /*if (file) {*/
        /*len = file.size();*/
        /*size_t flen = len;*/
        /*start = millis();*/
        /*while (len) {*/
            /*size_t toRead = len;*/
            /*if(toRead > BUFSIZE) {*/
                /*toRead = BUFSIZE;*/
            /*}*/
            /*file.read(buf, toRead);*/
            /*len -= toRead;*/
        /*}*/
        /*end = millis() - start;*/
        /*sprintf(stringbuf, "    READ TEST: %u bytes read for %u ms", flen, end);*/
        /*DEBUGTRACE(INFO, stringbuf);*/
        /*DEBUGTRACE(INFO, "    FILE SIZE: %d\n\r", file.size(), DEC);*/
        /*file.close();*/
    /*} else {*/
        /*DEBUGTRACE(WARN, "    Failed to open file for reading");*/
        /*return false;*/
    /*}*/

    /*file = fs.open(path, FILE_WRITE);*/
    /*if (!file) {*/
        /*DEBUGTRACE(WARN, "    Failed to open file for writing");*/
        /*return false;*/
    /*}*/

    /*start = millis();*/
    /*for (size_t i = 0; i < 4 * BUFSIZE; i++) {*/
        /*file.write(buf, BUFSIZE);*/
    /*}*/
    /*DEBUGTRACE(INFO, "I MADE IT TO HERE - 1");*/
    /*end = millis() - start;*/
    /*sprintf(stringbuf, "    WRITE TEST: %u bytes written for %u ms", 4 * BUFSIZE * BUFSIZE, end);*/
    /*DEBUGTRACE(INFO, stringbuf);*/
    /*DEBUGTRACE(INFO, "I MADE IT TO HERE - 2");*/
    /*//DEBUGTRACE(INFO, "    FILE SIZE: %d\n\r", file.size(), FLOAT);*/
    /*sprintf(stringbuf, "    FILE SIZE: %f bytes", file.size());*/
    /*DEBUGTRACE(INFO, stringbuf);*/
    /*DEBUGTRACE(INFO, "I MADE IT TO HERE - 3");*/
    /*file.close();*/
    /*DEBUGTRACE(INFO, "I MADE IT TO HERE - 4");*/

    /*return true;*/

/*}*/

