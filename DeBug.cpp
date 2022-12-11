
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.6

DESCRIPTION:

PHYSICAL DESIGN:
    Just ESP8266 or ESP32 board, nothing else required

MONITOR:
    For full monitoring, make sure serial = true and telnet = true when you
    create the DeBug object (as done at the end of this file).

    sudo arp-scan 192.168.1.0/24 | grep Espressif
    sudo netdiscover -c 3 -s 10 -L -N -r 192.168.1.0/24 | grep Espressif

    screen /dev/ttyUSB0 9600,cs8cls

    to terminate `Ctrl-a :quit`

    telnet ntp-clock.local
    telnet 192.168.1.44
    telnet ESP_24F9FD.fios-router.home.
    telnet ESP_24F9FD

    to terminate `Ctrl-] quit`

REFERENCE MATERIALS:

SOURCES:

CREATED BY:
    jeffskinnerbox@yahoo.com

----------------------------------------------------------------------------- */

#define TDEBUG  false         // should we activate trace message printing for debugging

#define BUFFER 50

#ifdef ESP32                  // found in ESP32 libraries (~/.arduino15/packages/esp32/)
#include <WiFi.h>
#else                         // found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ESP8266WiFi.h>
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
    prefix = false;     // flag to turn on/off prefix for trace messages

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
    prefix = p;         // flag to turn on/off prefix for trace messages

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
    label[STAT] =       "\e[1;34mSTATUS:  \e[m";           // bold blue font
    label[WARN] =       "\e[1;33mWARNING: \e[m";           // bold yellow font
    label[ERROR] =      "\e[1;31mERROR:   \e[m";           // bold red font
    label[FATAL] =      "\e[1;37m\e[41mFATAL:\e[m   ";     // bold White font on red background
    label[NOOP] =       "\e[1;35mNO-OP:   \e[m";           // bold purple font
    label[HEADING] =    "\e[1;37m\e[40m\n\rHEADING:\e[m";  // bold white font on black background
    label[UNLABELED] =  "";                                // no labels

}


// check the status of wifi, is it on or off
bool DeBug::checkWiFi(void) {

    if (WiFi.status() != WL_CONNECTED)
        return false;
    else
        return true;

}


// parser the characters entered via serial or telnet and execute proper action
void DeBug::commandParser(char c) {

    switch (c) {
        case 'R':   // reboot the esp8266 / esp32
            traceMsg(INFO, "Rebooting ...");
            if (serial) Serial.flush();
            if (serial) Serial.end();
            if (telnet) TelnetStream.flush();
            if (telnet) TelnetStream.stop();
            delay(100);
            ESP.restart();
            break;
        case 'C':   // drop telnet connection to esp8266 / esp32
            traceMsg(INFO, "Dropping telnet connection ... bye bye");
            Serial.flush();
            TelnetStream.flush();
            TelnetStream.stop();
            break;
        case 's':   // toggle on/off serial trace messages
            traceMsg(INFO, "Toggle on/off serial trace messages");
            serialOnOff(!serial);
            break;
        case 't':   // toggle on/off telnet trace messages
            traceMsg(INFO, "Toggle on/off telnet trace messages");
            telnetOnOff(!telnet);
            break;
        case 'p':   // toggle on/off prefix for trace messages
            traceMsg(INFO, "Toggle on/off prefix for trace messages");
            prefixOnOff(!prefix);
            break;
        case 0xFF:
            break;
        default:
            traceMsg(WARN, "Unknown character in commandParser() = ", c);
            break;
    }

}



// ------------------------------- Public Methods ------------------------------

// print status of Debug object
void DeBug::printStatus(void) {

    traceMsg(STAT, "Current state of DeBug object:");

    traceMsg(STAT, "\tSerial trace message print = ", serial ? "true" : "false");
    traceMsg(STAT, "\tTelnet trace message print = ", telnet ? "true" : "false");
    traceMsg(STAT, "\tPrefix on trace message = ", prefix ? "true" : "false");

}


// print file name, function name, and line number
void DeBug::location(void) {
    traceMsg(NOOP, "NOT IMPLEMENTED YET: Plan to provide file name + function name + line number as a prefix to trace message");
}


void DeBug::SetupHandler(void) {

    printMsg("\n\n\r");   // make sure you have a clean line after reboot

    serialOnOff(true);
    telnetOnOff(true);
    prefixOnOff(false);

}


void DeBug::SetupHandler(bool ser, bool tel, bool pre) {

    printMsg("\n\n\r");   // make sure you have a clean line after reboot

    serialOnOff(ser);
    telnetOnOff(tel);
    prefixOnOff(pre);

}


void DeBug::serialOnOff(bool flag) {

    if (flag) {
        traceMsg(STAT, "Serial DeBug printing is enabled");
    } else {
        traceMsg(STAT, "Serial DeBug printing is disabled");
        Serial.flush();
    }

    serial = flag;                // update DeBug object's flag

}


void DeBug::telnetOnOff(bool flag) {

    if (flag)
        if (!checkWiFi()) {
            traceMsg(WARN, "WiFi isn't connected, therefore, use of telnet (TelnetStream) could cause crash");
            traceMsg(STAT, "Telnet DeBug printing is disabled");
            telnet = false;       // update DeBug object's flag
            return;
    }

    if (flag) {
        TelnetStream.begin();
        traceMsg(STAT, "Telnet DeBug printing is enabled");
    } else {
        TelnetStream.flush();
        traceMsg(STAT, "Telnet DeBug printing is disabled");
    }

    telnet = flag;                // update DeBug object's flag

}


void DeBug::prefixOnOff(bool flag) {

    if (flag)
        traceMsg(STAT, "Prefix DeBug printing is enabled");
    else
        traceMsg(STAT, "Prefix DeBug printing is disabled");

    prefix = flag;                // update DeBug object's flag

}


void DeBug::printInfo(void) {

    uint32_t chipid = 0;

    traceMsg(INFO, "Information concerning ESP & flash memory chips:");

#ifdef ESP32
	for (int i = 0; i < 17; i = i + 8) {
	  chipid |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}

    traceMsg(INFO, "\tESP32 IP Address = ", WiFi.localIP());
    traceMsg(INFO, "\tESP32 MAC Address = ", WiFi.macAddress());
    traceMsg(INFO, "\tESP32 DHCP Hostname = ", WiFi.getHostname());
    traceMsg(INFO, "\tESP32 Chip ID (HEX) = %X\n\r", chipid, HEX);
	traceMsg(INFO, "\tESP32 Chip Model = %s Rev %d\n\r", ESP.getChipModel(), ESP.getChipRevision());
	traceMsg(INFO, "\tESP32 Chi Cores = %d cores\n\r", ESP.getChipCores(), DEC);

    //traceMsg(INFO, "\tFlash Chip Real Size (from chip): %d bits\n\r", ESP.getFlashChipRealSize(), DEC);
    traceMsg(INFO, "\tFlash Chip Size (what compiler set): %d bits\n\r", ESP.getFlashChipSize(), DEC);
    traceMsg(INFO, "\tFlash Chip Speed: %d Hz\n\r", ESP.getFlashChipSpeed(), DEC);
    traceMsg(INFO, "\tFlash Chip Mode: %d\n\r", ESP.getFlashChipMode(), DEC);

    traceMsg(INFO, "\tFree Heap Memory: %d bytes\n\r", ESP.getFreeHeap(), DEC);
    //traceMsg(INFO, "\tHeap Fragmentation: %d%%\n\r", ESP.getHeapFragmentation(), DEC);  // 0% is clean, more than ~50% is not harmless
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

    commandParser(TelnetStream.read());
    commandParser(Serial.read());

/*    switch (TelnetStream.read()) {*/
        /*case 'R':   // reboot the esp8266 / esp32*/
            /*traceMsg(INFO, "Rebooting ...");*/
            /*if (serial) Serial.flush();*/
            /*if (serial) Serial.end();*/
            /*if (telnet) TelnetStream.flush();*/
            /*if (telnet) TelnetStream.stop();*/
            /*delay(100);*/
            /*ESP.restart();*/
            /*break;*/
        /*case 'C':   // drop telnet connection to esp8266 / esp32*/
            /*traceMsg(INFO, "Dropping telnet connection ... bye bye");*/
            /*Serial.flush();*/
            /*TelnetStream.flush();*/
            /*TelnetStream.stop();*/
            /*break;*/
        /*case 's':   // toggle on/off serial trace messages*/
            /*traceMsg(INFO, "Toggle on/off serial trace messages");*/
            /*serialOnOff();*/
            /*break;*/
        /*case 't':   // toggle on/off telnet trace messages*/
            /*traceMsg(INFO, "Toggle on/off telnet trace messages");*/
            /*telnetOnOff();*/
            /*break;*/
        /*case 'p':   // toggle on/off prefix for trace messages*/
            /*traceMsg(INFO, "Toggle on/off prefix for trace messages");*/
            /*prefixOnOff();*/
            /*break;*/
        /*default:*/
            /*break;*/
    /*}*/

/*    switch (Serial.read()) {*/
        /*case 'R':   // reboot the esp8266 / esp32*/
            /*traceMsg(INFO, "Rebooting ...");*/
            /*if (serial) Serial.flush();*/
            /*if (serial) Serial.end();*/
            /*if (telnet) TelnetStream.flush();*/
            /*if (telnet) TelnetStream.stop();*/
            /*delay(100);*/
            /*ESP.restart();*/
            /*break;*/
        /*case 'C':   // drop telnet connection to esp8266 / esp32*/
            /*traceMsg(INFO, "Dropping telnet connection ... bye bye");*/
            /*Serial.flush();*/
            /*TelnetStream.flush();*/
            /*TelnetStream.stop();*/
            /*break;*/
        /*case 's':   // toggle on/off serial trace messages*/
            /*traceMsg(INFO, "Toggle on/off serial trace messages");*/
            /*serial = !serial;*/
            /*printStatus();*/
            /*break;*/
        /*case 't':   // toggle on/off telnet trace messages*/
            /*traceMsg(INFO, "Toggle on/off telnet trace messages");*/
            /*telnet = !telnet;*/
            /*printStatus();*/
            /*break;*/
        /*case 'p':   // toggle on/off prefix for trace messages*/
            /*traceMsg(INFO, "Toggle on/off prefix for trace messages");*/
            /*prefix = !prefix;*/
            /*printStatus();*/
            /*break;*/
        /*default:*/
            /*break;*/
    /*}*/

}


// perfrom a single scan for nearby wifi networks
void DeBug::wifiScan(void) {

    unsigned char numSsid;
    char buffer[BUFFER];
    String st;

    traceMsg(HEADING, "---------------------------- Starting Network Scan -----------------------------");

    numSsid = WiFi.scanNetworks();
    if (numSsid == 0) {
        traceMsg(INFO, "No WiFi networks found.");
        return;
    }

    // print the list of networks seen
    traceMsg(INFO, "Total number of SSIDs found: ", numSsid);
    snprintf(buffer, BUFFER, "\t%-20s\t%s\t%s", "SSID", "RSSI", "Encryp_Type");
    traceMsg(INFO, buffer);

    // print the name and characteristics of each network found
    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        st = WiFi.SSID(thisNet);     // convert from sting object to character string
        switch(WiFi.encryptionType(thisNet)) {
            case 2:
                snprintf(buffer, BUFFER, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "TKIP (WPA)");
                break;
            case 4:
                snprintf(buffer, BUFFER, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "CCMP (WPA)");
                break;
            case 5:
                snprintf(buffer, BUFFER, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "WEP");
                break;
            case 7:
                snprintf(buffer, BUFFER, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "NONE");
                break;
            case 8:
                snprintf(buffer, BUFFER, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "AUTO");
                break;
            default:
                traceMsg(ERROR, "Returned improper encryption type during WiFi scan.");
                break;
        }

        traceMsg(INFO, buffer);
    }

    traceMsg(HEADING, "---------------------------- Network Scan Completed ----------------------------");

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

    if (prefix) location();

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

    if (prefix) location();

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

    if (prefix) location();

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
template void DeBug::traceMsg<bool>(int, char*, bool);
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

#ifdef ESP32
template void DeBug::traceMsg<StringSumHelper>(int, char*, StringSumHelper);
#else
#endif

//template void DeBug::traceMsg(int, char*, T) [with T = char* (*)(const char*, int)];
//template void DeBug::traceMsg<char* (*)(char const*, int)>(int, char*, char* (*)(char const*, int));


// --------------------------- Construct DeBug Object --------------------------

// for trace messages/debugging, construct object DB as class DeBug
//DeBug DB = DeBug();
DeBug DB = DeBug(true, true, false);

