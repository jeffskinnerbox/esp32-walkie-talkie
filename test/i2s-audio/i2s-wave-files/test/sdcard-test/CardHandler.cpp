
/*------------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.1.0

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
//char stringbuf[BUF3_SIZE];



// ------------------------ Constructors & Destructors -------------------------

// Constructor to create CardHandler
CardHandler::CardHandler(void) {

    DEBUGTRACE(INFO, "Creating CardHandler object ...");

    if (!initSDCard())
        errorHandler(INIT_FAILED, "SD Card initialization failure");
    else
        DEBUGTRACE(INFO, "CardHandler object successfully created");

}


// Destructor to delete CardHandler
CardHandler::~CardHandler(void) {
}


//------------------------------ Private Methods -------------------------------

// initialize the SD card for use
bool CardHandler::initSDCard(void) {

    uint8_t cardType;

    // initialize spi communication protocol on the pins
    pinMode(CS_PIN, OUTPUT);      // the cs pin must be set to output
    digitalWrite(CS_PIN, HIGH);
    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN);

    // initializes the SD library and card. this begins using the SPI bus and the chip select pin
    if (!SD.begin(CS_PIN)) {
        DEBUGTRACE(ERROR, "SD Card mount failure");
        return false;
    } else {
        cardType = SD.cardType();
        DEBUGTRACE(INFO, "SD.cardType = %d", cardType);
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
        case CARD_UNKNOWN:
            DEBUGTRACE(INFO, "SD Card type: UNKNOWN");
            break;
        default:
            DEBUGTRACE(WARN, "SD Card type: The value returned is not expected!");
            return false;
    }

    DEBUGTRACE(INFO, "SD Card size: %lluMB", SD.cardSize() / bytes_per_megabytes);
    DEBUGTRACE(INFO, "SD Card file space enabled: %lluMB", SD.totalBytes() / bytes_per_megabytes);
    DEBUGTRACE(INFO, "SD Card file space used: %lluMB", SD.usedBytes() / bytes_per_megabytes);

    // get file pointer to the root directory of the filesystem
    root = SD.open("/");

    return true;

}


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

    DEBUGTRACE(INFO, "In errorHandler() and error_code = %i", error_code);

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
            DEBUGTRACE(ERROR, "Unknown error code in errorHandler: %i", error_code);
            DEBUGTRACE(FATAL, "Nothing can be done.  Doing an automatic restart.");
            Serial.flush();                  // make sure serial messages are posted
            ESP.restart();
    }
}



//------------------------------- Public Methods -------------------------------

// Unmount SD card
//void CardHandler::unMount(fs::SDFS &fs) {
void CardHandler::unMount() {

  DEBUGTRACE(INFO, "Unmounting SD card ...");

  // Unmount SD card
  SD.end();

  DEBUGTRACE(INFO, "Unmount done.");

}


// The first reference to 'fs' ('fs::') is a namespace in which the class 'SDFS' is defined.
// So '::' is a scope qualifier.  '&fs' indicates that the argument required is a pointer.
// Also using 'fs' as the name of the argument.  So, if you call listDir(),
// the first argument has to be a pointer to an object of type 'fs::SDFS'.
//bool CardHandler::listDir(fs::SDFS &fs, const char *path, uint8_t levels) {
/*bool CardHandler::listDir(const char *path, uint8_t levels) {*/

    /*File cur_dir = SD.open(path, FILE_READ);*/

    /*DEBUGTRACE(INFO, "Listing for directory: %s (aka %s)", path, cur_dir.name());*/

    /*if (!cur_dir) {*/
    /*//if (cur_dir == false) {*/
        /*DEBUGTRACE(WARN, "Failed to open directory: %s", path);*/
        /*return false;*/
    /*}*/

    /*if (!cur_dir.isDirectory()) {*/
        /*DEBUGTRACE(WARN, "%s is not a directory");*/
        /*//return false;*/
    /*}*/

    /*File file = cur_dir.openNextFile();*/
    /*while (file) {*/
        /*if (file.isDirectory()) {*/
            /*DEBUGTRACE(INFO, "    DIR: %s", file.name());*/
            /*if (levels > 0) {*/
                /*if (!listDir(file.name(), levels - 1))*/
                    /*return false;*/
            /*}*/
        /*} else {*/
            /*DEBUGTRACE(INFO, "    FILE NAME: %20s    SIZE: %5d Bytes", file.name(), file.size());*/
        /*}*/
        /*file = cur_dir.openNextFile();*/
    /*}*/

    /*return true;*/

/*}*/

bool CardHandler::listDir(const char *path, uint8_t levels) {

    File cur_dir = SD.open(path, FILE_READ);

    DEBUGTRACE(INFO, "Listing for directory: %s", path);

    if (!cur_dir) {
    //if (cur_dir == false) {
        DEBUGTRACE(WARN, "Failed to open directory: %s", path);
        return false;
    }

    if (!cur_dir.isDirectory()) {
        DEBUGTRACE(WARN, "%s is not a directory");
        return false;
    }

/*    File file = cur_dir.openNextFile();*/
    /*Serial.print(cur_dir.name());*/
    /*Serial.println("/");*/
    /*descendDir(file, levels);*/

    //descendDir(cur_dir, levels);
    descendDir(cur_dir, 0);

    return true;
}


void CardHandler::descendDir(File dir, int numIndents) {

    const char *initial_indent = "         ";
    const char *step_indent = "   ";

    while (true) {
        File entry = dir.openNextFile();
        if (! entry) {
            // no more files in this directory
            break;
        }
        Serial.print(initial_indent);
        for (uint8_t i = 0; i < numIndents; i++) {
            Serial.print(step_indent);
        }
            Serial.print(entry.name());
            if (entry.isDirectory()) {
                Serial.println("/");
                descendDir(entry, numIndents + 1);
            } else {
                // files have sizes, directories do not
                Serial.print(step_indent);
                Serial.println(entry.size(), DEC);
            }
        entry.close();
    }

}


// Tests whether a file or directory exists on the SD card
// returns true if the file or directory exists, false if not
bool CardHandler::existsFile(const char *file) {

    DEBUGTRACE(INFO, "Checking if the %s file or directory exists", file);

    if (SD.exists(file)) {
        DEBUGTRACE(INFO, "The %s file or directory exists", file);
        return true;
    } else {
        DEBUGTRACE(INFO, "The %s file or directory does NOT exist", file);
        return false;
    }

}


//bool CardHandler::createDir(fs::SDFS &fs, const char *path) {
bool CardHandler::createDir(const char *path) {

    if (SD.mkdir(path)) {
        DEBUGTRACE(INFO, "Directory %s created", path);
        return true;
    } else {
        DEBUGTRACE(WARN, "Directory %s creation failed", path);
        return false;
    }

}


//bool CardHandler::removeDir(fs::SDFS &fs, const char *path) {
bool CardHandler::removeDir(const char *path) {

    if (SD.rmdir(path)) {
        DEBUGTRACE(INFO, "Directory %s removed", path);
        return true;
    } else {
        DEBUGTRACE(WARN, "Directory %s removal failed", path);
        return false;
    }

}


// Write to the SD card
//bool CardHandler::writeFile(fs::SDFS &fs, const char *path, const char *message) {
bool CardHandler::writeFile(const char *path, const char *message) {

    File file = SD.open(path, FILE_WRITE);
    bool rtn;
    int bytes = 0;

    if (file) {
        DEBUGTRACE(INFO, "File %s successfully opened for writing", path);
    } else {
        DEBUGTRACE(WARN, "Failed to open file %s for writing", path);
        return false;
    }

    bytes = file.print(message);
    DEBUGTRACE(INFO, "Number of bytes writen to the file %s is %i", path, bytes);
    if (bytes > 0) {
        DEBUGTRACE(INFO, "File %s has been written", path);
        rtn = true;
    } else {
        DEBUGTRACE(WARN, "Failed to write to file %s", path);
        rtn = false;
    }

/*    if (!file) {*/
        /*DEBUGTRACE(WARN, "Failed to open file %s for writing", path);*/
        /*return false;*/
    /*}*/

    /*if (!file) {*/
    /*if (file.print(message)) {*/
        /*DEBUGTRACE(INFO, "File %s has been written", path);*/
        /*rtn = true;*/
    /*} else {*/
        /*DEBUGTRACE(WARN, "Failed to write to file %s", path);*/
        /*rtn = false;*/
    /*}*/

    file.close();
    return rtn;

}


// Append data to the SD card
//bool CardHandler::appendFile(fs::SDFS &fs, const char *path, const char *message) {
bool CardHandler::appendFile(const char *path, const char *message) {

    File file;
    bool rtn;
    int bytes = 0;

    DEBUGTRACE(INFO, "I make it to here -- #1");
    file = SD.open(path, FILE_APPEND);
    DEBUGTRACE(INFO, "I make it to here -- #2");

    if (file) {
        DEBUGTRACE(INFO, "File %s successfully opened for appending", path);
    } else {
        DEBUGTRACE(WARN, "Failed to open file %s for appending", path);
        return false;
    }

    bytes = file.print(message);
    DEBUGTRACE(INFO, "Number of bytes appended to the file %s is %i", path, bytes);
    if (bytes > 0) {
        DEBUGTRACE(INFO, "File %s has been appended", path);
        rtn = true;
    } else {
        DEBUGTRACE(WARN, "Failed to append to file %s", path);
        rtn = false;
    }

    file.close();
    return rtn;

/*    bool rtn;*/
    /*File file = SD.open(path, FILE_APPEND);*/

    /*if (!file) {*/
        /*DEBUGTRACE(WARN, "Failed to open file %s for appending", path);*/
        /*return false;*/
    /*}*/

    /*if (file.print(message)) {*/
        /*DEBUGTRACE(INFO, "Message appended to file %s", path);*/
        /*rtn = true;*/
    /*} else {*/
        /*DEBUGTRACE(WARN, "Message failed to appended to file %s", path);*/
        /*rtn = false;*/
    /*}*/

    /*file.close();*/
    /*return rtn;*/

}


//bool CardHandler::readFile(fs::SDFS &fs, const char *path) {
bool CardHandler::readFile(const char *path) {

    File file;

    DEBUGTRACE(INFO, "I make it to here -- #3");
    file = SD.open(path, FILE_READ);
    DEBUGTRACE(INFO, "I make it to here -- #4");

/*    if (!file) {*/
        /*DEBUGTRACE(WARN, "Failed to open file %s for reading", path);*/
        /*return false;*/
    /*}*/

    if (file) {
        DEBUGTRACE(INFO, "File %s successfully opened for reading", path);
    } else {
        DEBUGTRACE(WARN, "Failed to open file %s for reading", path);
        return false;
    }

    DEBUGTRACE(INFO, "Reading text from file %s", path);
    while (file.available()) {
        Serial.write(file.read());
    }
    DEBUGTRACE(INFO, "Done reading text from file %s", path);

    file.close();

}


//bool CardHandler::renameFile(fs::SDFS &fs, const char *path1, const char *path2) {
bool CardHandler::renameFile(const char *path1, const char *path2) {


    if (SD.rename(path1, path2)) {
        DEBUGTRACE(INFO, "Renaming file %s to %s: Successfully renamed", path1, path2);
        return true;
    } else {
        DEBUGTRACE(WARN, "Renaming file %s to %s: Failed to rename", path1, path2);
        return false;
    }

}


// This routine removes a file from the sd card.
//bool CardHandler::deleteFile(fs::SDFS &fs, const char *path) {
bool CardHandler::deleteFile(const char *path) {

    int rtn;

    DEBUGTRACE(INFO, "Deleting file %s", path);

    // check if the file exists, and if so, remove
    if (existsFile(path))
        rtn = SD.remove(path);
    else {
        DEBUGTRACE(WARN, "File %s does not exist", path);
        return false;
    }

/*    if (rtn == true) {*/
        /*DEBUGTRACE(INFO, "File %s deleted successfully", path);*/
        /*return true;*/
    /*} else if (rtn == false)  {*/
        /*DEBUGTRACE(WARN, "File %s deletion failed", path);*/
        /*return false;*/
    /*} else {*/
        /*DEBUGTRACE(WARN, "File %s does not exist", path);*/
        /*return false;*/
    /*}*/

    switch(rtn) {
        case true:
            DEBUGTRACE(INFO, "File %s deleted successfully", path);
            return true;
            break;
        case false:
            DEBUGTRACE(WARN, "File %s deletion failed", path);
            return false;
            break;
        default:
            DEBUGTRACE(WARN, "File %s does not exist", path);
            return false;
    }

}


//bool CardHandler::testFileIO(fs::SDFS &fs, const char *path) {
bool CardHandler::testFileIO(const char *path) {
    DEBUGTRACE(WARN, "Method \"CardHandler::testFileIO()\" not implemented yet");
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

