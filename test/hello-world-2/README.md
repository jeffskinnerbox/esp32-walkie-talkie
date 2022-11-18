<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


---


# DESCRIPTION
This is a basic ESP32 test routine but using the DeBug tracing class.
Debug trace messages will be sent to the ESP32's USB serial interface.

Test of `DeBug.cpp` is perfromed without an active WiFi connection,
and therefore, `telnet` isn't supported.
This should work anyway but will given you a warning if you attempt to use `telnet` features.
This configuration is fine for any application that doesn't plan to use `telnet`.

If you want to use `telnet`, you must start-up WiFi.
For WiFi, the class `WiFiHandler.cpp` is preferred but not required.

# PHYSICAL DESIGN

## Hardware
ESP-32S (aka ESP-WROOM-32 ESP32) Development Board - https://www.amazon.com/gp/product/B0718T232Z/

## Wiring
Nothing required

## Software Settings
Nothing required

# MONITOR
To monitor the trace messages of the ESP32 development board,
connection the USB port to a Linux computer and execute the following:

        screen /dev/ttyUSB0 9600,cs8cls

To terminate monitoring, enter `CNTR-a :quit`.
To stop the screen scrolling, enter `CNTR-a`.

# TESTING
None can be done.

# REFERENCE MATERIALS

# SOURCES

