<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----



[ESP32 Pinout – How to use GPIO pins?](https://microcontrollerslab.com/esp32-pinout-use-gpio-pins/)
[Adafruit I2S MEMS Microphone Breakout](https://learn.adafruit.com/adafruit-i2s-mems-microphone-breakout/)
[ESP8266 Pinout Reference: Which GPIO pins should you use?](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)
[ESP32 Pinout Reference: Which GPIO pins should you use?](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)



# DESCRIPTION
This set of test programs demonstrates how to use the ESP32 built-in
Digital to Analog Converter (DAC), Analog to Digital Converter (ADC),
and Inter-IC Sound (I2S - pronounced "eye-squared-ess")
for capturing audio data and for audio output.
There are four test programs in this directory:

* `dac-simple-output` - This program passes a sequence of 8-bit numbers to the ESP32 DAC.
Using a voltmeter, you then can read the voltage created by the DAC and compare it to its expected value.
* `loop_sampling` - This program shows how to use the Arduino `analogRead`
function and the Espressif `adc1_get_raw` function.
It also demonstrates how to get a calibrated value received from the ADC
to give you the actual voltage at the input.
Using a power supply, you can read its voltage via the programs ADC function.

* `i2s-mic` - This program takes the [SPH0645][05]
* `i2s_sampling` - This program handles both analogue devices
(such as the [MAX4466][03] and the [MAX9814][04]) and I2S devices (such as the [SPH0645][05] and [INMP441][06]).
It demonstrates how to use the I2S peripheral for high-speed sampling
using DMA to transfer samples directly to RAM.
We can read these samples from the internal ADC or from the I2S peripheral directly.
Samples are read from the DMA buffers and sent to a server running on your desktop machine.
* `i2s_output` - This program shows how to drive an I2S output device.
Its tested against the Adafruit [MAX98357][02] breakout board.
You can play a WAV file (from a ESP32 SPIFFS file system)
or you can play a simple sin wave through the output.
* `server` - This is a node server that writes the samples received from the ESP32 to a file.

## Audio
For many microcontrollers, adding audio input is easy with an analog microphone feed into a ADC.
But as you get to bigger and better microcontrollers and microcomputers,
you'll find that you don't always have an analog input,
or maybe you want to avoid the noise that can seep in with an analog microphone system.
You will often find I2S, which can take digital audio data directly in!

## Inter-IC Sound (I2S)
[I2S (Inter-IC Sound)][09] is a [digital audio][11], serial data, synchronous communication protocol
that is usually used for transmitting audio data between two digital audio devices.
ESP32 contains two I2S peripherals.
These peripherals can be configured as input or output controllers, sampling data via the I2S driver.
Each controller can operate in simplex communication mode, and therefore,
the two controllers can be combined to establish full-duplex communication.

I2S is not the same as I2C (Inter-IC communication or Inter-IC bus).
There are [several differences between I2C and I2S][10], but basically,
I2C is used for general inter IC communication (e.g. EEPROM or sensors)
and I2S is used only for audio devices.

* With I2C, it is easy to add a new slave device to the bus.
Just add the a new device without adding a new select like, unlike SPI.
* I2S can deliver a full audio chain, eliminating the need for any preamplifier, DAC, and ADC.

Sources
* [The I2S Protocol and Why Digital Audio is Everywhere][11]
* [Introduction to the I2S Interface](https://www.allaboutcircuits.com/technical-articles/introduction-to-the-i2s-interface/)
* [Inter-IC Sound (I2S)][09]
* [Basic Electronics: High-Quality Audio with Inter-IC Sound(I2S) Bus](https://www.seeedstudio.com/blog/2020/05/11/basic-electronics-high-quality-audio-with-inter-ic-soundi2s-bus/)
* [I2S Protocol: Working, Differences & Its Applications](https://www.elprocus.com/i2s-protocol/)
* [I2S Sound Tutorial for ESP32](https://diyi0t.com/i2s-sound-tutorial-for-esp32/)
* [Sound with ESP32 – I2S Protocol](https://dronebotworkshop.com/esp32-i2s/)
* [Building a Digital Music Player with I2S?! What is I2S! EB#45](https://www.youtube.com/watch?v=qNLvoSQCx60)

# PHYSICAL DESIGN
I used a [NodeMCU][12], supplied power via the serial USB port,

and read voltage using a bench voltmeter.



## Hardware
I used a [HiLetgo nodemcu-32s][13] and supplied power via the serial USB port.

I used a bench voltmeter to read the voltage output of the DAC
and a Linux terminal, attached to the USB serial port,
to see the predicited value of that voltage.

### MAX98357 I2S Audio Breakout Board
The I2S communication protocol lets us get the sound data
out of the microcontroller without any reduction in quality.
The next step is to decode the I2S signals into analog signals and amplifier for a speaker.
We need to decoder from I2S signal to analog signal, because speakers only work with analog signals.
Amplifier increases the power of the analog signal to increase the sound intensity.
The [MAX98357][02] is a digital pulse-code modulation (PCM) device that decodes the I2S signal
into an analog signal and the amplifies that signal.

## Wiring
Using the [pinout for HiLetgo nodemcu-32s][14]

I attached the voltmeter to the DAC output PIN9 (GPIO25)
and ground PIN38 (GRD).

Wiring up the MA4466 is very straightforward, you just need to connect VCC to 3v3, GND to GND and Out to the GPIO pin that corresponds to the ADC channel you are sampling from.

### ADC Sampling
There are two 12-bit [Successive Approximation Register (SAR) Analog to Digital Converter (ADC)][08]
on the ESP32 called ADC1 and ADC2.
ADC2 is also used by the WiFi sub-system
and some of the pins are also used strapping pins that control boot behavior.
This means it’s safest to stick with ADC1 for projects.

ADC1 has 8 channels (GPIOs 32 - 39):

|    Channel     |  GPIO  |
|:--------------:|:------:|
| ADC1_CHANNEL_0 | GPIO36 |
| ADC1_CHANNEL_1 | GPIO37 |
| ADC1_CHANNEL_2 | GPIO38 |
| ADC1_CHANNEL_3 | GPIO39 |
| ADC1_CHANNEL_4 | GPIO32 |
| ADC1_CHANNEL_5 | GPIO33 |
| ADC1_CHANNEL_6 | GPIO34 |
| ADC1_CHANNEL_7 | GPIO35 |

ADC2 has 10 channels and is shared with the WiFi (GPIOs 0, 2, 4, 12 - 15 and 25 - 27):

|    Channel     |  GPIO  |
|:--------------:|:------:|
| ADC2_CHANNEL_0 | GPIO4  |
| ADC2_CHANNEL_1 | GPIO0  |
| ADC2_CHANNEL_2 | GPIO2  |
| ADC2_CHANNEL_3 | GPIO15 |
| ADC2_CHANNEL_4 | GPIO13 |
| ADC2_CHANNEL_5 | GPIO12 |
| ADC2_CHANNEL_6 | GPIO14 |
| ADC2_CHANNEL_7 | GPIO27 |
| ADC2_CHANNEL_8 | GPIO25 |
| ADC2_CHANNEL_9 | GPIO26 |

The ESP32 ADC can be quite inaccurate,
and if you want to have an accurate readings,
you should use calibration settings.
Calibrations should have been done at the factory,
so your ESP32 should already have some calibration settings already.
If you choose do the calibration yourself,
check out this [ADC calibration tool][07].

Wiring up the MA4466 is very straightforward, you just need to connect `VCC` to ESP32 `3v3`,
`GND` to ESP32 `GND` and `Out` to the GPIO pin that corresponds to the ADC channel you are sampling from.

The same wiring is used for the MAX9814.
You can also play with the gain on the MAX9814 by connecting the Gain pin to either ESP32 `VCC` or `GND`.

### I2S Sampling
Directly reading from the built-in ADCs is fine for infrequent or slower readings,
but not suitable for high sampling rates typically found in audio.
A better approach is to use the built-in ADC of a I2S peripheral to read samples directly into memory.
Once you’ve read the samples from the I2S peripheral,
you can do whatever processing you need to do.
The I2S peripheral will continue in the background reading samples with its ADC
and sending them into the ESP32 DMA buffers.

## Software Settings
[I2S Protocol : Working, Differences & Its Applications](https://www.elprocus.com/i2s-protocol/)

* **SCK -** The SCK or Serial Clock is the first line of the I2S protocol which is also known as BCLK or bit clock line which is used to obtain the data on a similar cycle. The serial clock frequency is simply defined by using the formula like Frequency = Sample Rate x Bits for each channel x no. of channels.
* **WS -** In the I2S communication protocol, the WS or word select is the line which is also known as FS (Frame Select) wire that separates the right or left channel.
If WS = 0 then left channel or channel-1 is used.
If WS = 1 then the right channel or channel-2 is used.
* **SD -** The Serial Data or SD is the last wire where the payload is transmitted within 2 complements. So, it is very significant that the MSB is first transferred, because both the transmitter & receiver may include different word lengths. Thus, the transmitter or the receiver has to recognize how many bits are transmitted.

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



#### Step 1: Clone the ESP32 I2S Audio Repository
I'm going to clone atomic14's [ESP32 I2S Audio][01] repository
and use it as part of my testing.

```bash
# clone the repository
cd ~/src/embedded-devices/esp32-walkie-talkie/test
git clone https://github.com/atomic14/esp32_audio.git ./i2s-audio

# remove references to the clones github and build methodology
cd i2s-audio
rm -r -f .git .github platformio*

# in sub-directories, add Makefile and do a test build

# in sub-directories, remove the unneeded stuff
rm -r -f include/ test/ lib/ platformio.ini .vscode

# in sub-directories, add Makefile and do a test build
```



[01]:https://github.com/atomic14/esp32_audio
[02]:https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp
[03]:https://www.adafruit.com/product/1063
[04]:https://www.adafruit.com/product/1713
[05]:https://www.adafruit.com/product/3421
[06]:https://invensense.tdk.com/wp-content/uploads/2015/02/INMP441.pdf
[07]:https://github.com/tommag/ESP32_ADC_Calibration_tool
[08]:https://en.wikipedia.org/wiki/Successive-approximation_ADC
[09]:https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html
[10]:https://prodigytechno.com/i2c-vs-i2s/
[11]:https://blogs.keysight.com/blogs/tech/bench.entry.html/2022/04/29/i2s_the_digital_audiointerfacethatrockstoday-6CGE.html
[12]:https://www.nodemcu.com/index_en.html
[13]:https://www.amazon.com/dp/B010N1SPRK
[14]:https://forum.fritzing.org/t/esp32s-hiletgo-dev-boad-with-pinout-template/5357
[15]:
[16]:
[17]:
[18]:
[19]:
[20]:

