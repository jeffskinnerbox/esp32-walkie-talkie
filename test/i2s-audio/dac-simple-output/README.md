<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----


**DO THIS**
* Add feature to turn on & off the WiFi radio.  You are likely to see the RF interference impacting the voltage reading.
* Very the VCC from 5V to =/- 0.2V and see the imact on the readings
* Do multiple sample and average the value to get a sense of the noise present
* [How good are the ADCs inside Arduinos, ESP8266, and ESP32? And external ADCs (ADS1115)](https://www.youtube.com/watch?v=UAJMLTzrM9Q)


# DESCRIPTION
The `dac-simple-output`program passes a sequence of 8-bit numbers to the ESP32 DAC.
The expected voltage values will be printed on the USB serial port,
which can be compared to the voltage created via the DAC.

# PHYSICAL DESIGN
I used a [NodeMCU][03], supplied power via the serial USB port,
and read voltage using a bench voltmeter.

## Hardware
I used a [HiLetgo nodemcu-32s][01] and supplied power via the serial USB port.
I used a bench voltmeter to read the voltage output of the DAC
and a Linux terminal, attached to the USB serial port,
to see the predicited value of that voltage.

## Wiring
Using the [pinout for HiLetgo nodemcu-32s][02],
I attached the voltmeter to the DAC output PIN9 (GPIO25)
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
Using a voltmeter, you then can read the voltage created by the DAC and compare it to its expected value,
which is printed on the USB serial port.

# REFERENCE MATERIALS
Key documentation for understand this design can be found here:

* [ESP32 I2S Audio](https://github.com/atomic14/esp32_audio)
* [ESP32 Audio Input - MAX4466, MAX9814, SPH0645LM4H, INMP441](https://atomic14.com/2020/09/12/esp32-audio-input.html)

# SOURCES
Source code was taken from:

* [ESP32 I2S Audio](https://github.com/atomic14/esp32_audio)



[01]:https://www.amazon.com/dp/B010N1SPRK
[02]:https://forum.fritzing.org/t/esp32s-hiletgo-dev-boad-with-pinout-template/5357
[03]:https://www.nodemcu.com/index_en.html
