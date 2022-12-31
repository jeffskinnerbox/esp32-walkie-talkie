
/* -----------------------------------------------------------------------------
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.7

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

#define BUFFER1 25            // array size for small buffer
#define BUFFER2 50            // array size for medium buffer
#define BUFFER3 100           // array size for large buffer

#ifdef ESP32                  // found in ESP32 libraries (~/.arduino15/packages/esp32/)
#include <WiFi.h>
#include <driver/i2s.h>       // i2s driver
#else                         // found in ESP8266 libraries (~/.arduino15/packages/esp8266)
#include <ESP8266WiFi.h>
#endif

#include <TelnetStream.h>
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
            //traceMsg(WARN, "Unknown character in commandParser() = ", c);
            break;
    }

}



// ------------------------------- Public Methods ------------------------------

// print status of Debug object
void DeBug::printStatus(void) {

    traceMsg(STAT, "Current state of DeBug object:");

    traceMsg(STAT, "\tSerial trace message print = %s", serial ? "true" : "false");
    traceMsg(STAT, "\tTelnet trace message print = %s", telnet ? "true" : "false");
    traceMsg(STAT, "\tPrefix on trace message = %s", prefix ? "true" : "false");

}


// print file name, function name, and line number
void DeBug::location(void) {
    traceMsg(NOOP, "NOT IMPLEMENTED YET: Plan to provide file name + function name + line number as a prefix to trace message");
    Serial.printf("%s, %s, %d: \t\n\r", __FILE__, __FUNCTION__, __LINE__);
}


void DeBug::SetupHandler(void) {

    printMsg("\n\n\r");   // make sure you have a clean line after reboot

    OnOff(true, true, false);

}


void DeBug::SetupHandler(bool ser, bool tel, bool pre) {

    printMsg("\n\n\r");   // make sure you have a clean line after reboot

    OnOff(ser, tel, pre);

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


void DeBug::OnOff(bool flag1, bool flag2, bool flag3) {

    serialOnOff(flag1);
    telnetOnOff(flag2);
    prefixOnOff(flag3);

}


void DeBug::printInfo(void) {

    uint32_t chipid = 0;
    char buf[BUFFER1];
    int rtn;

    rtn = snprintf(buf, BUFFER1, "%02X:%02X:%02X:%02X:%02X:%02X",
        WiFi.macAddress()[0], WiFi.macAddress()[1], WiFi.macAddress()[2],
        WiFi.macAddress()[3], WiFi.macAddress()[4], WiFi.macAddress()[5]);
    if (rtn <= 0 && rtn >= BUFFER1) {
        traceMsg(ERROR, "Buffer overflow condition in DeBug::traceMsg() - #5");
    }

#ifdef ESP32
	for (int i = 0; i < 17; i = i + 8) {
	  chipid |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	}

    traceMsg(INFO, "Information concerning ESP & flash memory chips:");
    traceMsg(INFO, "\tESP32 IP Address = %s",  WiFi.localIP());
    traceMsg(INFO, "\tESP32 MAC Address = %s", buf);
    traceMsg(INFO, "\tESP32 DHCP Hostname = %s", WiFi.getHostname());
    traceMsg(INFO, "\tESP32 Chip ID (HEX) = %X", chipid);
	traceMsg(INFO, "\tESP32 Chip Model = %s Rev %d", ESP.getChipModel(), ESP.getChipRevision());
	traceMsg(INFO, "\tESP32 Chi Cores = %d cores", ESP.getChipCores());

    traceMsg(INFO, "\tFlash Chip Size (what compiler set): %d bits", ESP.getFlashChipSize());
    traceMsg(INFO, "\tFlash Chip Speed: %d Hz", ESP.getFlashChipSpeed());
    traceMsg(INFO, "\tFlash Chip Mode: %d", ESP.getFlashChipMode());

    traceMsg(INFO, "\tFree Heap Memory: %d bytes", ESP.getFreeHeap());
#else
    traceMsg(INFO, "\tESP32 IP Address = %s",  WiFi.localIP());
    traceMsg(INFO, "\tESP32 MAC Address = %s", buf);
    traceMsg(INFO, "\tESP8266 DHCP Hostname = %s", WiFi.hostname());
    traceMsg(INFO, "\tESP8266 Chip ID (HEX) = %X", ESP.getChipId());

    traceMsg(INFO, "\tFlash Chip ID (HEX): %X", ESP.getFlashChipId());
    traceMsg(INFO, "\tFlash Chip Real Size (from chip): %d bits", ESP.getFlashChipRealSize());
    traceMsg(INFO, "\tFlash Chip Size (what compiler set): %d bits", ESP.getFlashChipSize());
    traceMsg(INFO, "\tFlash Chip Speed: %d Hz", ESP.getFlashChipSpeed());
    traceMsg(INFO, "\tFlash Chip Mode: %d", ESP.getFlashChipMode());

    traceMsg(INFO, "\tFree Heap Memory: %d bytes", ESP.getFreeHeap());
    traceMsg(INFO, "\tHeap Fragmentation: %d%%", ESP.getHeapFragmentation());  // 0% is clean, more than ~50% is not harmless
#endif

}


void DeBug::LoopHandler(void) {

    commandParser(TelnetStream.read());
    commandParser(Serial.read());

}


// perfrom a single scan for nearby wifi networks
void DeBug::wifiScan(void) {

    unsigned char numSsid;
    char buffer[BUFFER2];
    String st;

    traceMsg(HEADING, "---------------------------- Starting Network Scan -----------------------------");

    numSsid = WiFi.scanNetworks();
    if (numSsid == 0) {
        traceMsg(INFO, "No WiFi networks found.");
        return;
    }

    // print the list of networks seen
    traceMsg(INFO, "Total number of SSIDs found: %u", numSsid);
    snprintf(buffer, BUFFER2, "\t%-20s\t%s\t%s", "SSID", "RSSI", "Encryp_Type");
    traceMsg(INFO, buffer);

    // print the name and characteristics of each network found
    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        st = WiFi.SSID(thisNet);     // convert from sting object to character string
        switch(WiFi.encryptionType(thisNet)) {
            case 2:
                snprintf(buffer, BUFFER2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "TKIP (WPA)");
                break;
            case 4:
                snprintf(buffer, BUFFER2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "CCMP (WPA)");
                break;
            case 5:
                snprintf(buffer, BUFFER2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "WEP");
                break;
            case 7:
                snprintf(buffer, BUFFER2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "NONE");
                break;
            case 8:
                snprintf(buffer, BUFFER2, "\t%-20s\t%d\t%s", st.c_str(), WiFi.RSSI(thisNet), "AUTO");
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


bool DeBug::traceMsg(int lev, char *str) {

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

    return true;

}


/*------------------------------------------------------------------------------

Description:
    DEBUGTRACE is making use of snprintf.
    snprintf stands for “String print”. Instead of printing on console,
    it store output on character buffer which are specified in snprintf.

    The size of the buffer should be large enough to contain the entire
    resulting string (see snprintf for a safer version).

    A terminating null character is automatically appended after the content written.

Syntax:
    int snprintf(char *buffer, int buffer_size, const char *format-specifier, ...);

    A format specifier follows this prototype:
    %[flags][width][.precision][length]specifier

Return:
    If successful, it returns the total number of characters written excluding
    null-character (aka '\0') appended in the string, in case of failure a
    negative number is returned.

Sources:
    Cplusplus.com - https://cplusplus.com/reference/cstdio/snprintf/
    Variadic function templates in C++ - https://www.geeksforgeeks.org/variadic-function-templates-c/
    Variadic templates in C++ - https://eli.thegreenplace.net/2014/variadic-templates-in-c/

------------------------------------------------------------------------------*/


// for printing of IPAdress type variables
bool DeBug::traceMsg(int lev, const char *format, IPAddress ip) {

    char buf2[BUFFER1];
    int rtn;

    // create printable string in buffer
    rtn = snprintf(buf2, BUFFER1, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    // check if you have an error - only when this returned value is non-negative
    // and less than n, the string has been completely written
    if (rtn <= 0 && rtn >= BUFFER1) {
        traceMsg(ERROR, "Buffer overflow condition in DeBug::traceMsg() - #1");
        return false;
    }

    rtn = traceMsg(lev, format, buf2);

    return rtn;

}


// Variadic function Template that takes variable number of arguments and prints all of them.
template<typename... Types>
bool DeBug::traceMsg(int lev, const char *format, Types... var) {

    char buf[BUFFER3];
    int rtn;

    // create printable string in buffer
    rtn = snprintf(buf, BUFFER3, format, var...);

    // check if you have an error - only when this returned value is non-negative
    // and less than n, the string has been completely written
    if (rtn <= 0 && rtn >= BUFFER3) {
        traceMsg(ERROR, "Buffer overflow condition in DeBug::traceMsg() - #3");
        return false;
    }

    if (prefix) location();

    if (lev != UNLABELED) {
        if (serial) {
            Serial.print(label[lev]);
            Serial.println(buf);
        }
        if (telnet) {
            TelnetStream.print(label[lev]);
            TelnetStream.println(buf);
        }
    } else
        printMsg(buf);

    return true;

}



// ----------------- Templatize Additionally Needed Functions ------------------

template void DeBug::printMsg<char*>(char*);
template void DeBug::printMsg<char const*>(char const*);
template bool DeBug::traceMsg<char>(int, char const*, char);

template void DeBug::printMsg<int>(int);
template bool DeBug::traceMsg<int>(int, char const*, int);
template void DeBug::printMsg<unsigned int, int>(unsigned int, int);

template bool DeBug::traceMsg<long>(int, char const*, long);
template bool DeBug::traceMsg<unsigned long>(int, char const*, unsigned long);

template bool DeBug::traceMsg<float>(int, char const*, float);
template bool DeBug::traceMsg<float*>(int, char const*, float*);

template void DeBug::printMsg<wl_status_t>(wl_status_t);
template bool DeBug::traceMsg<wl_status_t>(int, char const*, wl_status_t);

template bool DeBug::traceMsg<unsigned long long>(int, char const*, unsigned long long);

// --------------------------- Construct DeBug Object --------------------------

// for trace messages/debugging, construct object DB as class DeBug
DeBug DB = DeBug(true, true, false);

