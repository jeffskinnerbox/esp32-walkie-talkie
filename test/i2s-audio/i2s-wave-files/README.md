<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----



# DESCRIPTION
The `i2s-music-player` program uses the ESP32 I2
reads a WAV file for  from a
Micro SD TF Card Adapter
and plays the music via a head phone

Sources:
* [Building a Digital Music Player with I2S?! What is I2S! EB#45](https://www.youtube.com/watch?v=qNLvoSQCx60)
* [ESP32 Sound - Working with I2S](https://www.youtube.com/watch?v=m-MPBjScNRk)
* [I2S Sound Tutorial for ESP32](https://diyi0t.com/i2s-sound-tutorial-for-esp32/)
* [Guide to SD Card Module with Arduino](https://randomnerdtutorials.com/guide-to-sd-card-module-with-arduino/)
* [ESP32: Guide for MicroSD Card Module using Arduino IDE](https://randomnerdtutorials.com/esp32-microsd-card-arduino/)

## SD Card Module
Embeded devices can use a [Micro SD (aka TF or TransFlash) card][08],
typically used in phones, for mass storage.
The SD card module can be very useful for projects that require data logging
or when data needs to be retrived for use with your program.
The [Arduino IDE has a SD library][09] that helps you
create directories and file so you can read/write on the SD card.
There are different SD models from different suppliers,
but they all work in a similar way,
using the [SPI communication protocol][10].
SD card module are very inexpensive and I purchased a [package of 5 from Amazon][06].

## I2S Decoder/Amplifier
Adafruit [MAX98357][07] I2S decoder/amplifier

# PHYSICAL DESIGN
I used a ESP32 [NodeMCU-32s][03], suppling it power via the serial USB port,
including the SD card module, Adafruit [MAX98357][07] I2S decoder/amplifier, perpherial devices.

## Hardware
I used a [HiLetgo nodemcu-32s][01] and supplied power via the serial USB port.
I used a bench power supply to apply voltage to the ADC input pin
and a Linux terminal, attached to the USB serial port,
to see the predicited value of that voltage.

### Micro SD Card Adater Reader Module
[HiLetgo Micro SD TF Card Adater Reader Module](https://www.amazon.com/dp/B07BJ2P6X6)
6 Pin SPI Interface Driver Module

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

# CALIBRATION
Due to variation in internal reference voltage of different ESP32 chips,
the ESP32 ADCs needs to be calibrated before using.
One way of doing calibration is using the true ADC reference voltage stored in [eFuse][04].
This value is measured and written to the ESP32 when it was made at factory.

>**NOTE:** If you choose do the calibration yourself,
>check out this [ADC calibration tool][05].

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
[03]:https://www.teachmemicro.com/using-esp32-for-the-first-time/
[04]:https://en.wikipedia.org/wiki/EFuse
[05]:https://github.com/tommag/ESP32_ADC_Calibration_tool
[06]:https://www.amazon.com/dp/B07BJ2P6X6
[07]:https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp
[08]:https://appuals.com/what-is-tf-transflash-card-and-how-is-it-different-from-micro-sd/
[09]:https://www.arduino.cc/reference/en/libraries/sd/
[10]:https://www.circuitbasics.com/basics-of-the-spi-communication-protocol/
[11]:
[12]:
[13]:
[14]:
[15]:
[16]:
[17]:
[18]:
[19]:
[20]:


