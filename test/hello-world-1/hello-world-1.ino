
/*------------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION
    The most basic of ESP32 test routines.  All that there is to observe is a
    blinking LED on the development board.

PHYSICAL DESIGN
    Hardware
        ESP-32S (aka ESP-WROOM-32 ESP32) Development Board - https://www.amazon.com/gp/product/B0718T232Z/

    Wiring
        Nothing required

    Software Settings
        Nothing required

MONITOR
    Observing the blinking LED is the only monitor that can be done.

TESTING
    None can be done.

REFERENCE MATERIALS

SOURCES

------------------------------------------------------------------------------*/


void setup() {
    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
    Serial.println("Hello World 1");

    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
}
