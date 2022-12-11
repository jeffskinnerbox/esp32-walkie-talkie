
<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----


[ESP32 Pinout – How to use GPIO pins?](https://microcontrollerslab.com/esp32-pinout-use-gpio-pins/)
[Adafruit I2S MEMS Microphone Breakout](https://learn.adafruit.com/adafruit-i2s-mems-microphone-breakout/)
[ESP32 Audio Input - INMP441 and SPH0645 MEMS I2S Breakout Boards](https://www.youtube.com/watch?v=3g7l5bm7fZ8)

[Sound with ESP32 – I2S Protocol](https://dronebotworkshop.com/esp32-i2s/)
[I2S Sound Tutorial for ESP32](https://diyi0t.com/i2s-sound-tutorial-for-esp32/)

[ESP32 Audio Input Using I2S and Internal ADC](https://www.youtube.com/watch?v=pPh3_ciEmzs)
[ESP32 Audio Output Using I2S and built-in Digital to Analogue Converters (DACs)](https://www.youtube.com/watch?v=lgDu88Y411o)



# DESCRIPTION

# PHYSICAL DESIGN
I used a [NodeMCU][12], supplied power via the serial USB port,

and read voltage using a bench voltmeter.

## Hardware
I used a [HiLetgo nodemcu-32s][13] and supplied power via the serial USB port.

## Wiring
Using the [pinout for HiLetgo nodemcu-32s][14]

### SPH0645 Pinouts
* **3V** - is the power in pin and must be powered from as low as 1.6V to 3.6V.
* **GND** - is ground for both power and data
* **BCLK** - is the bit clock, also known as the data clock or just 'clock'.
Thes clock ticks comes from the I2S to tell the microphone its time to transmit data.  This should run at 2-4 MHz.
* **DOUT** - is the data output from the microphone.
* **LRCLK** - is the left/right clock, also known as WS (word select),
and it tells the microphone when to start transmitting.
When the LRCLK is low, the left channel will transmit. When LRCLK is high, the right channel will transmit.
* **SEL** - is the channel select pin. By default this pin is low, so that it will transmit on the left channel mono.
If you connect this to high logic voltage, the microphone will instantly start transmitting on the right channel.

# MONITOR
To monitor the scrolling display's activities on the NodeMCU serial ports while operating,
connection the USB port to a Linux computer and execute the following:

    screen /dev/ttyUSB0 9600,cs8cls

To terminate monitoring, enter: `CNTR-a :quit`.
To stop the screen scrolling, enter: `CNTR-a :`.

# TESTING

# REFERENCE MATERIALS
Key documentation for understand this design can be found here:

* [ESP32 I2S Audio](https://github.com/atomic14/esp32_audio)
* [ESP32 Audio Input - MAX4466, MAX9814, SPH0645LM4H, INMP441](https://atomic14.com/2020/09/12/esp32-audio-input.html)
* [Measuring analog inputs with ESP32 ADC (ESP-IDF)](https://embeddedexplorer.com/esp32-adc-esp-idf-tutorial/)
* Analog to Digital Converter (ADC)
    * [How good are the ADCs inside Arduinos, ESP8266, and ESP32? And external ADCs (ADS1115)](https://www.youtube.com/watch?v=UAJMLTzrM9Q)
    * [Analog to Digital Converter](https://docs.espressif.com/projects/esp-idf/en/v4.1.1/api-reference/peripherals/adc.html)

# SOURCES
Source code was taken from:

* [ESP32 I2S Audio](https://github.com/atomic14/esp32_audio)





-----







[12]:https://www.nodemcu.com/index_en.html
[13]:https://www.amazon.com/dp/B010N1SPRK
[14]:https://forum.fritzing.org/t/esp32s-hiletgo-dev-boad-with-pinout-template/5357

