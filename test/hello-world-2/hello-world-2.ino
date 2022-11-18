
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION
    This is a basic ESP32 test routine but using the DeBug tracing class.
    Debug trace messages will be sent to the ESP32's USB serial interface.

PHYSICAL DESIGN
    Hardware
        ESP-32S (aka ESP-WROOM-32 ESP32) Development Board - https://www.amazon.com/gp/product/B0718T232Z/

    Wiring
        Nothing required

    Software Settings
        Nothing required

MONITOR
    To monitor the trace messages of the ESP32 development board,
    connection the USB port to a Linux computer and execute the following:

        screen /dev/ttyUSB0 9600,cs8cls

    To terminate monitoring, enter `CNTR-a :quit`.
    To stop the screen scrolling, enter `CNTR-a`.

TESTING
    None can be done.

REFERENCE MATERIALS

SOURCES

------------------------------------------------------------------------------*/


#define TDEBUG  true              // activate trace message printing for debugging

// this project's include files
#include "DeBug.h"

//-------------------------- Static Scoped Variables ---------------------------
// version stamp
#define VER "hello-world-2" " - "  __DATE__ " at " __TIME__
static char version[] = VER;

extern DeBug DB;                  // declare object DB as external, and member of class DeBug



//------------------------------- Main Routines --------------------------------

void setup() {

    Serial.begin(9600);           // always start Serial first so it can be used by DeBug
    while (!Serial) {}            // wait for serial port to connect

    DEBUGSETUP();                 // should be right after 'Serial.begin' and before any debug message
    DEBUGON(true, true, false);   // turn on serial and telnet tracing ... no error since "TelnetStream.begin()" isn't executed
    DEBUGTRACE(HEADING, "------------------------------- Entered setup() --------------------------------");
    DEBUGTRACE(INFO, "Application Version = ", version);
    DEBUGLOCATION();              // just a test stub for the "location" feature
    DEBUGSTATUS();                // provide information about debug status flags
    DEBUGINFO();                  // provide some useful information about the microprocessor

    pinMode (LED_BUILTIN, OUTPUT);

    DEBUGON(true, true, false);   // turn on serial and telnet tracing ... throws an error?? -- No
    DEBUGSTATUS();                // provide information about debug status flags

    DEBUGTRACE(HEADING, "------------------------------- Leaving setup() --------------------------------");

}


void loop() {

    DEBUGLOOP();

    DEBUGTRACE(INFO, "Hello World 2");

    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);

}
