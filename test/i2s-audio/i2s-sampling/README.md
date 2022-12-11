<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----


# DESCRIPTION
The `i2s-sampling` program uses the ESP32

12-bit ADC to sample a voltage level on GPIO25.
The expected voltage values will be printed on the USB serial port,
which can be compared to the voltage created via a power supply on ADC input pin..

# PHYSICAL DESIGN
I used a [NodeMCU][03], supplied power via the serial USB port,
and read voltage bench power supply.

## Hardware
I used a [HiLetgo nodemcu-32s][01] and supplied power via the serial USB port.
I used a bench power supply to apply voltage to the ADC input pin
and a Linux terminal, attached to the USB serial port,
to see the predicited value of that voltage.

## Wiring
Using the [pinout for HiLetgo nodemcu-32s][02],
I attached the power supply to the ADC input PIN9 (GPIO25)
and ground PIN38 (GRD).

## Software Settings
None

# MONITOR
To monitor the scrolling display's activities on the NodeMCU serial ports while operating,
connection the USB port to a Linux computer and execute the following:

    screen /dev/ttyUSB0 9600,cs8cls

To terminate monitoring, enter: `CNTR-a :quit`.
To stop the screen scrolling, enter: `CNTR-a :`.

# TESTING
Apply a voltage (2.4V or less) to GPIO Pin35 and compare that to what the program
prints on the USB serial port.

# REFERENCE MATERIALS
Key documentation for understand this design can be found here:

* [ESP32 I2S Audio](https://github.com/atomic14/esp32_audio)
* [ESP32 Audio Input - MAX4466, MAX9814, SPH0645LM4H, INMP441](https://atomic14.com/2020/09/12/esp32-audio-input.html)
* [Measuring analog inputs with ESP32 ADC (ESP-IDF)](https://embeddedexplorer.com/esp32-adc-esp-idf-tutorial/)
* [Analog to Digital Converter](https://docs.espressif.com/projects/esp-idf/en/v4.1.1/api-reference/peripherals/adc.html)

# SOURCES
Source code was taken from:

* [ESP32 I2S Audio](https://github.com/atomic14/esp32_audio)



[01]:https://www.amazon.com/dp/B010N1SPRK
[02]:https://forum.fritzing.org/t/esp32s-hiletgo-dev-boad-with-pinout-template/5357
[03]:https://www.nodemcu.com/index_en.html
[04]:
[05]:
[06]:
[07]:
[08]:
[09]:
[10]:

