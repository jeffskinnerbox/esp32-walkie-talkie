
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5

DESCRIPTION:

PHYSICAL DESIGN:
    Just ESP8266, nothing else required

MONITOR:
    For full monitoring, make sure serial = true and relnet = true when you
    create the DeBug object (as done at the end of this file).

    sudo arp-scan 192.168.1.200/24
    sudo arp-scan 192.168.1.200/24 | grep Espressif
    nslookup 192.168.1.44

    screen /dev/ttyUSB0 9600,cs8cls
    to terminate Ctrl-a :quit

    telnet ntp-clock.local
    telnet 192.168.1.44
    telnet ESP_24F9FD.fios-router.home.
    telnet ESP_24F9FD
    to terminate Ctrl-] quit

REFERENCE MATERIALS:

SOURCES:

CREATED BY:
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

#define TDEBUG  false         // should we activate trace message printing for debugging

#include <Arduino.h>          // found in ESP32 libraries (~/.arduino15/packages/esp32/hardware/esp32)
#ifdef ESP32
#include <WiFi.h>         // found in ESP32 libraries (~/.arduino15/packages/esp32/hardware/esp32/2.0.5/tools/sdk)
#else
#include <ESP8266WiFi.h>    // found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#endif

// found in Arduino libraries (~/Arduino/libraries)
#include <TelnetStream.h>

// found in Arduino libraries (~/src/arduino/libraries)

// found in Arduino Sketchbooks libraries (~/src/arduino/sketchbooks/libraries)

// for trace messages/debugging, found in this directory
#include "DeBug.h"


// ------------------------ Constructors & Destructors -------------------------

// default constructors for the class
DeBug::DeBug(void) {

    serial = true;      // flag to turn on/off serial trace messages
    telnet = false;     // flag to turn on/off telnet trace messages
    preamble = false;   // flag to turn on/off preamble for trace messages
    cols = LABEL_COLS;  // max characters in labels
    rows = LABEL_ROWS;  // number of labels

    // create matrix used to store trace message labels
    label = new char*[rows];
    if (rows) {
        label[0] = new char[rows * cols];
        for (int i = 1; i < rows; ++i)
            label[i] = label[0] + i * cols;
    }

    // initialize trace message labels
    setLables();

}


// constructors for the class
DeBug::DeBug(bool s, bool t, bool p) {

    serial = s;         // flag to turn on/off serial trace messages
    telnet = t;         // flag to turn on/off telnet trace messages
    preamble = p;       // flag to turn on/off preamble for trace messages
    cols = LABEL_COLS;  // max characters in labels
    rows = LABEL_ROWS;  // number of labels

    // create matrix used to store trace message labels
    label = new char*[rows];
    if (rows) {
        label[0] = new char[rows * cols];
        for (int i = 1; i < rows; ++i)
            label[i] = label[0] + i * cols;
    }

    // initialize trace message labels
    setLables();

}


// destructors for the class
DeBug::~DeBug(void) {
    // delete array used to store trace message labels
    if (rows) delete [] label[0];
    delete [] label;
};



// ------------------------------ Private Methods ------------------------------

// setup lables
void DeBug::setLables(void) {

    // initialize trace message labels
    label[INFO] =       "\e[1;32mINFO:    \e[m";           // bold green font
    label[WARN] =       "\e[1;33mWARNING: \e[m";           // bold yellow font
    label[ERROR] =      "\e[1;31mERROR:   \e[m";           // bold red font
    label[FATAL] =      "\e[1;37m\e[41mFATAL:\e[m   ";     // bold White font on red background
    label[NOOP] =       "\e[1;35mNO-OP:   \e[m";           // bold purple font
    label[HEADING] =    "\e[1;37m\e[40m\n\rHEADING:\e[m";  // bold white font on black background
    label[UNLABELED] =  "";                                // no labels

}


// print status of Debug object
void DeBug::printStatus(void) {

    traceMsg(INFO, "Current state of DeBug object:");

    traceMsg(INFO, "\tSerial trace message print = ", serial ? "true" : "false");
    traceMsg(INFO, "\tTelnet trace message print = ", telnet ? "true" : "false");
    traceMsg(INFO, "\tPreamble on trace message = ", preamble ? "true" : "false");

}


// print file name, function name, and line number
void DeBug::location(void) {
    traceMsg(NOOP, "NOT IMPLEMENTED YET: Plan to provide file name + function name + line number as a preamble to trace message");
}



// ------------------------------- Public Methods ------------------------------

void DeBug::SetupHandler(void) {

    printMsg("\n\n\r");   // make sure you have a clean line after reboot

    if (telnet) {
        TelnetStream.begin();
        traceMsg(INFO, "TelnetStream enabled for telnet");
    }

}


void DeBug::SetupHandler(bool ser, bool tel, bool pre) {

    serial = ser;
    telnet = tel;
    preamble = pre;

    printMsg("\n\n\r");   // make sure you have a clean line after reboot

}


void DeBug::debugOnOff(bool flag) {
    serial = flag;
}


void DeBug::telnetOnOff(bool flag) {
    telnet = flag;
}


void DeBug::preambleOnOff(bool flag) {
    preamble = flag;
}


void DeBug::printInfo(void) {

    traceMsg(INFO, "Information concerning ESP & flash memory chips:");

#ifdef ESP32
#else
    traceMsg(INFO, "\tESP8266 IP Address = ", WiFi.localIP());
    traceMsg(INFO, "\tESP8266 MAC Address = ", WiFi.macAddress());
    traceMsg(INFO, "\tESP8266 DHCP Hostname = ", WiFi.hostname());
    traceMsg(INFO, "\tESP8266 Chip ID (HEX) = %X\n\r", ESP.getChipId(), HEX);

    traceMsg(INFO, "\tFlash Chip ID (HEX): %X\n\r", ESP.getFlashChipId(), HEX);
    traceMsg(INFO, "\tFlash Chip Real Size (from chip): %d bits\n\r", ESP.getFlashChipRealSize(), DEC);
    traceMsg(INFO, "\tFlash Chip Size (what compiler set): %d bits\n\r", ESP.getFlashChipSize(), DEC);
    traceMsg(INFO, "\tFlash Chip Speed: %d Hz\n\r", ESP.getFlashChipSpeed(), DEC);
    traceMsg(INFO, "\tFlash Chip Mode: %d\n\r", ESP.getFlashChipMode(), DEC);

    traceMsg(INFO, "\tFree Heap Memory: %d bytes\n\r", ESP.getFreeHeap(), DEC);
    traceMsg(INFO, "\tHeap Fragmentation: %d%%\n\r", ESP.getHeapFragmentation(), DEC);  // 0% is clean, more than ~50% is not harmless
#endif

}


void DeBug::LoopHandler(void) {

    switch (TelnetStream.read()) {
        case 'R':   // reboot the esp8266
            traceMsg(INFO, "Rebooting ...");
            if (serial) Serial.flush();
            if (serial) Serial.end();
            if (telnet) TelnetStream.flush();
            if (telnet) TelnetStream.stop();
            delay(100);
            //ESP.reset();
            ESP.restart();
            break;
        case 'C':   // drop telnet connection to esp8266
            traceMsg(INFO, "Dropping telnet connection ... bye bye");
            Serial.flush();
            TelnetStream.flush();
            TelnetStream.stop();
            break;
        case 's':   // toggle on/off serial trace messages
            traceMsg(INFO, "Toggle on/off serial trace messages");
            serial = !serial;
            printStatus();
            break;
        case 't':   // toggle on/off telnet trace messages
            traceMsg(INFO, "Toggle on/off telnet trace messages");
            telnet = !telnet;
            printStatus();
            break;
        case 'p':   // toggle on/off preamble for trace messages
            traceMsg(INFO, "Toggle on/off preamble for trace messages");
            preamble = !preamble;
            printStatus();
            break;
        default:
            break;
    }

    switch (Serial.read()) {
        case 'R':   // reboot the esp8266
            traceMsg(INFO, "Rebooting ...");
/*            Serial.flush();*/
            //TelnetStream.flush();
            //Serial.end();
            /*TelnetStream.stop();*/
            if (serial) Serial.flush();
            if (serial) Serial.end();
            if (telnet) TelnetStream.flush();
            if (telnet) TelnetStream.stop();
            delay(100);
            //ESP.reset();
            ESP.restart();
            break;
        case 'C':   // drop telnet connection to esp8266
            traceMsg(INFO, "Dropping telnet connection ... bye bye");
            Serial.flush();
            TelnetStream.flush();
            TelnetStream.stop();
            break;
        case 's':   // toggle on/off serial trace messages
            traceMsg(INFO, "Toggle on/off serial trace messages");
            serial = !serial;
            printStatus();
            break;
        case 't':   // toggle on/off telnet trace messages
            traceMsg(INFO, "Toggle on/off telnet trace messages");
            telnet = !telnet;
            printStatus();
            break;
        case 'p':   // toggle on/off preamble for trace messages
            traceMsg(INFO, "Toggle on/off preamble for trace messages");
            preamble = !preamble;
            printStatus();
            break;
        default:
            break;
    }

}



// ------------------ Explicitly Declare All Needed Functions ------------------

template<typename T>
void DeBug::printMsg(T var) {

    if (serial) Serial.print(var);

    if (!telnet) return;
    else TelnetStream.print(var);
}


template<typename T, typename U>
void DeBug::printMsg(T str, U var) {

    if (serial) {
        Serial.print(str);
        Serial.print(var);
    }

    if (!telnet) return;
    else {
        TelnetStream.print(str);
        TelnetStream.print(var);
    }
}


template<typename T, typename U, typename Z>
void DeBug::printMsg(T *str, U var, Z format) {

    if (serial) {
        Serial.print(str);
        Serial.print(var, format);
    }

    if (!telnet) return;
    else {
        TelnetStream.print(str);
        TelnetStream.print(var, format);
    }
}


void DeBug::traceMsg(int lev, char *str) {

    if (preamble) location();

    if (lev != UNLABELED) {
        if (serial) {
            Serial.print(label[lev]);
            Serial.println(str);
        }
        if (telnet) {
            TelnetStream.print(label[lev]);
            TelnetStream.println(str);
        }
    } else
        printMsg(str);

}


template<typename T>
void DeBug::traceMsg(int lev, char *str, T var) {

    if (preamble) location();

    if (lev != UNLABELED) {
        if (serial) {
            Serial.print(label[lev]);
            Serial.print(str);
            Serial.println(var);
        }
        if (telnet) {
            TelnetStream.print(label[lev]);
            TelnetStream.print(str);
            TelnetStream.println(var);
        }
    } else {
        printMsg(str);
        printMsg(var);
    }

}


template<typename T, typename U>
void DeBug::traceMsg(int lev, char *str, T var, U format) {

    if (preamble) location();

    if (lev != UNLABELED) {
        if (serial) {
            Serial.print(label[lev]);
            Serial.printf(str, var, format);
        }
        if (telnet) {
            TelnetStream.print(label[lev]);
            TelnetStream.printf(str, var, format);
        }
    } else {
        printMsg(str);
        printMsg(var, format);
    }

}



// ----------------- Templatize Additionally Needed Functions ------------------

template void DeBug::printMsg<int>(int);
template void DeBug::printMsg<char*>(char*);
template void DeBug::printMsg<wl_status_t>(wl_status_t);
template void DeBug::printMsg<char const*>(char const*);
template void DeBug::printMsg<unsigned int, int>(unsigned int, int);

template void DeBug::traceMsg<int>(int, char*, int);
template void DeBug::traceMsg<char*>(int, char*, char*);
template void DeBug::traceMsg<String>(int, char*, String);
template void DeBug::traceMsg<int, int>(int, char*, int, int);
template void DeBug::traceMsg<IPAddress>(int, char*, IPAddress);
template void DeBug::traceMsg<char const*>(int, char*, char const*);
template void DeBug::traceMsg<wl_status_t>(int, char*, wl_status_t);
template void DeBug::traceMsg<unsigned int>(int, char*, unsigned int);
template void DeBug::traceMsg<unsigned char>(int, char*, unsigned char);
template void DeBug::traceMsg<unsigned long>(int, char*, unsigned long);
template void DeBug::traceMsg<FlashMode_t, int>(int, char*, FlashMode_t, int);
template void DeBug::traceMsg<int, char const*>(int, char*, int, char const*);
template void DeBug::traceMsg<unsigned int, int>(int, char*, unsigned int, int);
template void DeBug::traceMsg<unsigned char, int>(int, char*, unsigned char, int);
template void DeBug::traceMsg<char*, char const*>(int, char*, char*, char const*);
template void DeBug::traceMsg<char const*, char*>(int, char*, char const*, char*);

//template void DeBug::traceMsg<StringSumHelper>(int, char*, StringSumHelper);
//template void DeBug::traceMsg(int, char*, T) [with T = char* (*)(const char*, int)];
//template void DeBug::traceMsg<char* (*)(char const*, int)>(int, char*, char* (*)(char const*, int));



// --------------------------- Construct DeBug Object --------------------------

// for trace messages/debugging, construct object DB as class DeBug
DeBug DB = DeBug(true, true, false);

