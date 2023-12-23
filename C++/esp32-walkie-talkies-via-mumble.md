<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="https://raw.githubusercontent.com/jeffskinnerbox/blog/main/content/images/banners-bkgrds/work-in-progress.jpg" title="These materials require additional work and are not ready for general use." align="center" width=420px height=219px>
</div>


-----




Consider MicroPython / CircuitPython ...
* [Programming a NodeMCU with MicroPython: I2C Bus Part 1 (MPU6050 IMU)](https://www.youtube.com/watch?v=2ufkfd-oFrY)
* [Programming an ESP32 NodeMCU with MicroPython](https://www.youtube.com/playlist?list=PL4Dmmk1VXA5p1_b0cCsIU2uM5nR47Whn4)
* [Programming a NodeMCU with MicroPython](https://www.youtube.com/playlist?list=PL4Dmmk1VXA5rJDidEu4hcXum2EL7qVyo9)




Consider using and reading ...
* [Adafruit STEMMA Audio Amp - Mono 2.5W Class D - PAM8302](https://www.adafruit.com/product/5647)
* [Simple MP3 Audio Playback With Raspberry Pi Pico](https://embeddedcomputing.com/technology/processing/interface-io/simple-mp3-audio-playback-with-raspberry-pi-pico)
* [I2S Volume Control With Raspberry Pi Pico and CircuitPython](https://embeddedcomputing.com/technology/open-source/i2s-volume-control-with-raspberry-pi-pico-and-circuitpython)
* [Raspberry Pi Pico Audio Line Out Via PCM5102A I2S Breakout](https://embeddedcomputing.com/technology/open-source/development-kits/raspberry-pi-pico-audio-line-out-via-pcm5102a-i2s-breakout)
* [Home Assistant Voice Remote](https://www.youtube.com/watch?v=EeUG3Si9fZk)
* [ESP32 Battery Life Tips](https://www.youtube.com/watch?v=4EZYYPmQnJY)

* [DIY Walkie-Talkie Based on ESP-NOW](https://www.elektormagazine.com/articles/diy-walkie-talkie-based-on-esp-now)
    * [Walkie-Talkie with ESP-NOW](https://www.elektormagazine.com/labs/walkie-talkie-with-esp-now)





* [Lithium-Ion Battery Circuitry Is Simple](https://hackaday.com/2022/10/10/lithium-ion-battery-circuitry-is-simple/)

* [Build Your Own Smart Home Speaker With ESPHome!](https://www.youtube.com/watch?v=4DQ9iyV3G20)

* [SOCORAD32 ESP32 walkie-talkie board also supports data communication](https://www.cnx-software.com/2023/01/25/socorad32-esp32-walkie-talkie-board-also-supports-data-communication-crowdfunding/)
* [LILYGO T-TWR is an ESP32-S3 board with an SA868 Walkie-Talkie module](https://www.cnx-software.com/2023/03/01/lilygo-t-twr-is-an-esp32-s3-board-with-an-sa868-walkie-talkie-module/)


# User Interface
* [Create a modern user interface with the Tkinter Python library](https://opensource.com/article/23/2/user-interface-tkinter-python)

# Management of the ESP32 Walkie Talkie
* [ESP32 Web Updater and SPIFFS File Manager](https://hackaday.com/2023/02/17/esp32-web-updater-allows-file-system-management-and-ota-updates/)

* [ESP32 WiFiManager – Easy WiFi Provisioning](https://dronebotworkshop.com/wifimanager/)
* [WiFiManager with ESP32 - Stop Hard-coding WiFi Credentials](https://www.youtube.com/watch?v=VnfX9YJbaU8)

* [PersWiFiManager](http://ryandowning.net/PersWiFiManager/examples.html)
* [DoubleResetDetector](https://github.com/datacute/DoubleResetDetector/blob/master/examples/minimal/minimal.ino)

* [WiFiManager](https://github.com/tzapu/WiFiManager/tree/development#how-it-works)
* [Avoid Hard-Coding WiFi Credentials on Your ESP8266 Using the WiFiManager Library](https://www.instructables.com/id/Avoid-Hard-Coding-WiFi-Credentials-on-Your-ESP8266/)

# Make it Speak to Home Assistant
* [More Voice More Voice More Voice (Local Control)](https://www.mostlychris.com/more-voice-more-voice-more-voice/)
    * [LOCAL VOICE CONTROL of Home Assistant with the M5Stack Atom Echo](https://www.youtube.com/watch?v=U2rykdQlSgA)
* [Using Your VOIP Phone to Control Your House](https://www.mostlychris.com/using-your-voip-phone-to-control-your-house/?ref=mostlychris-newsletter)

# Other Walkie-Talkie Designs
* [Make a 500m Range #Walkie-Talkie in easiest way (part 1)](https://www.youtube.com/watch?v=fMFVc4c8yE4)
* [This is your #Walkie-Talkie #Transmitter & #Receiver !! (Part 2)](https://www.youtube.com/watch?v=jBUp8tYlQfM)
* [Easiest way to Make a Without Coil Walkie Talkie](https://www.youtube.com/watch?v=dDEH0JEmJ6Q)



-----




I have been thinking about creating a walkie-talkie for my young grand-children
such that they could talk to each other (or me!) over the Internet.
I did some web searching and found some existing projects that came close,
or in some ways, exactly what I was looking for.
Specifically the projects that inspired me where:

* To create a very simple walkie-talkie,
[greatscott][02] using an [arduino for control/audio sampling and nrf24l0+ for transceiver][03].
This walkie-talkie had poor sound quality but fixes were proposed by [andreas spiess][05] in his [video][18].
* It appears [andreas spiess][05] was inspired to create his own walkie-talkie but using a
[raspberry pi zero w and the mumble VoIP platform][19].
* The [esp32 based walkie-talkie][01], created by [atomic14][04],
can broadcast UDP packetized voice over a WiFi network, and  for free standing networking,
it uses the ESP-NOW protocol to broadcast to nearby walkie-talkies.
As a bonus, atomic14 has a series of video on [esp32 audio][20] that could be very helpful.

Armed with these projects,
I decide to pick the best design approaches from each of them to create my walkie-talkie.
I really like the idea of using [mumble][21] since it enables broadcasting across the Internet.
I also want to use the ESP32 hardware so I could use [ESP-NOW][22] when we don't have a WiFi,
and I could use broadcast UDP when I have WiFi.
You'll observe that the earlier projects referenced above,
the only component missing is a [mumble client][23] that will work on a ESP32,
which I found [here][???].
Finally, it might also be wish to provide a [small OLED display][24] to show status
since the design is getting more complex.



-----



# My Walkie-Talkie Build Approach
In atomic14's design, audio data is transmitted over either UDP broadcast or ESP-NOW.
Since a major portion of my design objective is contained in atomic14 design,
I'll start my build there and add the Mumble an OLED features in a subsequent step.

I used the following sources for this initial design:

Sources:

* Walkie-Talkie
    * [An ESP32 Walkie-Talkie, For Those Spy Radio Moments][01]
    * [ESP32 Walkie-Talkie using UDP Broadcast and ESP-NOW][42]
    * [atomic14 / esp32-walkie-talkie][09]
    * [atomic14 / esp32_audio][43]
    * [ESP32 Walkie Talkie - Part 1 - Fusion 360 case design][48]
* Getting Sound Out of ESP32
    * [ESP32 Sound - Working with I2S][44]
    * [ESP32 Audio Output Using I2S and built-in Digital to Analogue Converters (DACs)][45]
    * [ESP32 Audio][20]
* Arduino Audio Tools
    * [#419 ESP32 Audio Tutorial with lots of examples][50]
* Using MicroPhyon
    * [MicroPython I2S Audio with the ESP32][49]
* ESP-NOW
    * [#172 Hidden: ESP32 and ESP8266 point-to-point (ESP-Now): Fast and efficient. Comparison with LoRa][46]

Good sources for audio examples:
* [Phil Schatzmann: Machine Sound](https://www.pschatzmann.ch/home/category/machine-sound/)
* [ESP32 Sound - Working with I2S](https://www.youtube.com/watch?v=m-MPBjScNRk)
* [Atomic14: Audio Input](https://www.atomic14.com/tag/audio-input.html)

## ESP32 Walkie-Talkie: Bill of Materials (BOM)
To make and test two ESP32 Walkie-Talkie, I used the following materials:

| Item | Quantity | Description |
|:-----|:--------:|:------------|
* ESP32 - [TinyS2 (ESP32-S2 Board)][06] has onboard LiPo charging, 4MBs of flash, 320K SRAM & 2MB PSRAM, 700mA output 3.3V regulator, and 17 GPIO broken out, its a ESP32-S2 development board in the TinyPICO format
* I2S Microphone - [Adafruit SPH0645LM4H][10] I2S MEMS Microphone Breakout (potential alternative is [DAOKI INMP441][16])
* Audio Speaker - 4 Ohm / 3 Watts full range [audio speaker][07]
* Audio Amplifier - [Adafruit MAX98357A][08] (or [this][47] form factor) 3W Class D Amplifier takes standard I2S digital audio input, decodes it into analog, and amplifies to support a speaker (alternative is the [MakerHawk MAX98357][17])
* On/Off Switch - [Adafruit SPDT slide switch][14] suitable for breadboard and perfboard projects
* Press-to-Talk Switch - Large, round [tactile button momentary switch][15] suitable for breadboard and perfboard projects
* Battery -
* Firmware for Microcontroller - Software and instructions can be found on [GitHub][09].
* Custom PCB - A custom PCB was used to keep things organized and compact.  It was designed on [EsayEDS][11] and the schematic diagram / PCB layout are stored on [OSHWLab][12].  The PCB boards were manufactured by [PCBWay][13], but you can no long order the boards directly from them.  I needed to resubmit the OSHWLab PCB layout to PCBWay.

* [First KiCad board from an EasyEDA user](https://www.youtube.com/watch?v=RMPezTH0cck)

# ESP32-S2
ESP32-S2 only has a single 32-bit Xtensa core, reduced RAM and ROM,
and WiFi only (no Bluetooth) — compared to the traditional ESP32.
These tradeoffs mean the S2 draws significantly less power consumption, especially while active.

* [#339 The new ESP32-S2: How relevant is it for the Makers? First tests](https://www.youtube.com/watch?v=L6IoSVdKwNM)
* [EYE on NPI - Espressif ESP32-S2 SoC](https://www.youtube.com/watch?v=4F25y-P8krM)
* [ESP32-S2 Processor Datasheet Released, Development Boards Unveiled](https://www.cnx-software.com/2019/09/03/esp32-s2-processor-datasheet-released-development-boards-unveiled/)
* [FeatherS2 Board Brings ESP32-S2 to Adafruit Feather Form Factor](https://www.cnx-software.com/2020/10/05/feathers2-board-brings-esp32-s2-to-adafruit-feather-form-factor/)
* [ESP32-S2 board targets battery-powered applications with 30uA deep sleep power consumption](https://www.cnx-software.com/2020/10/28/esp32-s2-board-targets-battery-powered-applications-with-30ua-deep-sleep-power-consumption/)
* [Test of Battery operated ESP32 Boards (Olimex, TinyPICO, EzSBC, TTGO)](https://www.youtube.com/watch?v=ajt7vtgKNNM)
* [ESP32-S2 New Arduino USB Functionality: Cool Combination with WiFi](https://www.youtube.com/watch?v=XnqDEV21BSA)

## TinyPICO
TinyPICO is the world’s smallest, fully-featured ESP32 development board, designed to unlock the power of the ESP32’s dual-core 240MHz processor and internet connectivity, in a package smaller than your thumb!
This development board supports MicroPython and C, and is available in both Micro-B and USB-C line power or LiPo battery LiPo battery management on the board.

TinyPICO has exceptional power management.
TinyPICO has been designed with two isolated power paths: a 5 V path and a 3.3 V path. Any components that are not needed for operation via battery or via the 3.3 V power pin are isolated within the 5 V power path, and are totally shut down when no USB cable is plugged in.

Deep sleep has been optimised for all development platforms, and though we have seen it go as low as 10 uA, the official current rating in deep sleep is 20 uA.

Note: TinyPICO includes an on-board APA102 RGB LED that has a quiescent current of 1 mA.

Thankfully there is a solution for deep sleep. GPIO13 controls the power to the APA102 using a P-Channel MOSFET via a high-side switch that can (along with smoother IO settings) shut down the power to the APA102 for you. It’s essential to do this before going into deep sleep to ensure the lowest current draw possible.

Unexpected Maker have created some helper functions for you in our TinyPICO MicroPython & Arduino helper libraries to make this easy. Please check the code in the platform you use to understand how to shut down the APA102 correctly as just pulling GPIO13 high is not enough.

* [TinyPICO Getting Started](https://www.tinypico.com/gettingstarted)
* [TinyPICO Coding Examples](https://www.tinypico.com/code-examples)
* [MicroPython for TinyPICO](https://micropython.org/download/tinypico/)

* [Deep Sleep & Other Power Modes (ESP32 + Arduino Series)](https://www.youtube.com/watch?v=dyvpRYfWjkY&feature=youtu.be)
* [ESP32 Deep Sleep with Arduino IDE and Wake Up Sources](https://randomnerdtutorials.com/esp32-deep-sleep-arduino-ide-wake-up-sources/)
* [ESP32 Deep Sleep Tutorial](https://www.instructables.com/ESP32-Deep-Sleep-Tutorial/)
* [ESP32-S2 board targets battery-powered applications with 30uA deep sleep power consumption](https://www.cnx-software.com/2020/10/28/esp32-s2-board-targets-battery-powered-applications-with-30ua-deep-sleep-power-consumption/)
* [Deep Sleep: Quick and dirty guide to Lithium battery-powered Wemos D1 Mini](https://blog.smartere.dk/2022/12/quick-and-dirty-lithium-battery-powered-wemos-d1-mini/)
* [ESP8266 - 1+ Year 18650 battery lifetime](https://blog.sarine.nl/2020/01/01/1-year-sensor.html)
* [A DEEP DIVE INTO LOW POWER WIFI MICROCONTROLLERS](https://hackaday.com/2018/12/17/a-deep-dive-into-low-power-wifi-microcontrollers/)
* [ESP8266: Monitoring Power Consumption](https://thingpulse.com/esp8266-monitoring-power-consumption/)
* [Max deep sleep for ESP8266](https://thingpulse.com/max-deep-sleep-for-esp8266/)
* [5 Hacks to Prolong your ESPaper’s Battery Run-Time](https://thingpulse.com/5-hacks-prolong-espapers-battery-run-time/)

###TinyPICO V2
It's size is 18mm x 32mm, ESP32 PICO D4, 32Bit Dual Core 240Mhz, 2.4GHz WiFi - 802.11b/g/n, Bluetooth BLE 4.2, 4MB SPI FLash, 4MB Extra PSRAM, 14x GPIO broken out.

###TinyPICO V3
It's size is 18mm x 35mm, ESP32 PICO D4, 32Bit Dual Core 240Mhz, 2.4GHz WiFi - 802.11b/g/n, Bluetooth BLE 4.2, 4MB SPI FLash, 4MB Extra PSRAM, 14x GPIO broken out.

### TinyPICO V3 with u.FL Connector
This TinyPICO V3 has no antenna but comes with a iu.FL connector instead
so you can attach an external antenna of your choice.

* [TinyPICO V3 with u.FL connector](https://unexpectedmaker.com/)
* [TinyPICO V3 with u.FL connector](https://www.tindie.com/products/seonr/tinypico-v3/)
* [TinyPICO V3 USB-C u.FL](https://shop.pimoroni.com/products/tinypico-v3?variant=40313120456787)

## TinyS2
This is the ESP32-S2 development board in the TinyPICO format!
TinyS2 is 18mm wide and 35mm long.
The [TinyS2 has very low power consumption][39], especially when sleeping.

Just like it’s bigger brother TinyPICO, the TinyS2 has onboard LiPo battery charging, 4MBs of flash, an on-board RGB LED that can be shut down to reduce deep sleep current and a generous 700mA output 3.3V regulator.
TinyS2 also has 2MB of extra PSRAM on top of the 320K SRAM and 17 GPIO broken out.

There are 2 revisions of the TinyS2 that have slightly different default GPIO for the SPI pins. On an ESP32/S2 any IO can be hardware SPI pins, so though technically it doesn’t matter which ones you use, what matters is that most of the development platforms (Arduino, CircuitPython and MicroPython) have defaults assigned for the TinyS2 and the defaults are setup for the original P1 & P2 layout.

So what does that mean for folks that have a P3 or later board? Well it means that if you initialise SPI using the default pins, they will possibly be different to how you have wired up your SPI device. The SDI (MISO) and CLK pins are flipped on the later P3+ versions.

So instead of initialising the SPI bus using the defaults, you have to instead explicitly set the IO for MOSI, MISO & CLK.

* [TinyS2](https://unexpectedmaker.com/tinys2)

### TinyS2 Pin Out
There are at least 3 revisions (P1, P2, P3) of the [TinyS2 (ESP32-S2 Board)][06],
resulting in [two slightly different default GPIO for the SPI pins][34].
In my case, I had revision P5!

>**NOTE:** To find out what revision TinyS2 you have,
>look under the USB connector on the back of the board and you will find the P revision.
>If you have covered your P version with the JST connector,
>you can tell from the pinouts below if you have a P1/P2 or a P3/newer by looking at the SPI pin layout.

[TinyS2 Pin Out (Revision P3 or later)](https://images.squarespace-cdn.com/content/v1/5a8cc639a803bbfaa029615a/83545830-8ac4-4a12-85fb-9eb13fef4036/TinyS2_V1_P3_pinout_extended.jpg?format=1000w)

### NodeMCU-32S
The instructions for the ESP32 Walkie Talkie calls for the use of the [TinyS2 (ESP32-S2 Board)][06].
To start my initial development,
I chose to use the [NodeMCU-32S][41] (aka NodeMCU ESP-WROOM-32) which is better documented.



-----



# Arduino Audio Tools
I cam accross the Arduino Audio Tools project, specifiaclly via [this video][50],
after I was deep into investigating the repurposing [atomic14's][04] [esp32 based walkie-talkie][01].
I was impressed by the Audio Tools ease of use, use of Streams, large number of features, and [design goals][51].

Stream is the base class that Serial inherits. Serial is a type of Stream but there are other types of Stream as well.
write is different from print in one important way: write sends things as raw bytes and print sends things as ASCII. So, if I Serial.print(255), the Arduino will actually send out 3 bytes, the ASCII codes for all three digits.

Sources:

* [#419 ESP32 Audio Tutorial with lots of examples][50]
* [GitHub: pschatzmann/arduino-audio-tools](https://github.com/pschatzmann/arduino-audio-tools)
* [Audio Tools Wiki](https://github.com/pschatzmann/arduino-audio-tools/wiki/Introduction)
* [ArduinoAudioTools: Playing Movies ?](https://www.pschatzmann.ch/home/2023/05/06/arduinoaudiotools-playing-movies/)

## What are C++ Streams?
One of the standout features of C++ is its built-in support of streams.
In C++, a stream refers to a sequence of characters that are transferred between
the program and I/O devices.
So stream classes in C++ facilitate input and output operations on files and other I/O devices;
between the CPU and external devices.
These classes have specific features to handle program input and output,
making it easier to write portable code that can be used across multiple platforms.

The four essential C++ stream classes:

* `istream` is a general purpose input stream.
* `ostream` is a general purpose output stream.
* `ifstream` is an input file stream. It is a special kind of an istream that reads in data from a data file.
* `ofstream` is an output file stream. It is a special kind of ostream that writes data out to a data file.

C++ makes heavy use of a concept called inheritance,
in which some classes inherit the properties of previously written classes.
Audio Tools make use of `istream` and `ostream` as base classes to create classes with
properties useful for handling audio data from dwevices.

Sources

* [Introduction to C / C++ Programming File I/O](https://www.cs.uic.edu/~jbell/CourseNotes/CPlus/FileIO.html)

#### Step 1: Installation in Arduino IDE - DONE
You can download the AudioTools library as a zip file,
or you can `git clone` this project into the Arduino libraries folder:

```bash
# install AudioTools in the arduino library
cd  ~/Arduino/libraries/
git clone https://github.com/pschatzmann/arduino-audio-tools.git
```

I perfer this install method because you can easily update it to the latest
version just by executing the `git pull` command in the project folder `~/Arduino/libraries/arduino-audio-tools`.

#### Step X: Test AudioTools with Examples
Within the above installed library you'll find many example implementation
within `~/Arduino/libraries/arduino-audio-tools/examples`.
I focused on the following examples:

* `.../examples-stream/streams-generator-serial` - test of basic library functionality,
generated sine wave, convert to CSV format, send to the serial output
* `.../examples-stream/streams-generator-analog` - test for the ESP32 analog input sent to I2S output,
internally generated sine wave should produce a clean tone
* `.../examples-stream/streams-url_mp3-analog` - test of streaming from URL to analog output,
a [simple mp3 streaming audio player][52] streaming BBC World Service to the analog output pins

Just copy the examples into you test directory
Below is a specific description for in general what I needed to do for all the examples:

```bash
# move to your test directory

# copy your test program
cp -r ~/Arduino/libraries/arduino-audio-tools/examples/examples-stream/streams-generator-serial/ .

# move into directory you just created via the above copy
cd streams-generator-serial/

# copy your standard esp32 makefile into the directory (make sure makefile is set for the right type of esp32)
cp ../../Makefile .

# plug usb cable into the esp32 and run your make
make
```


#### Step X:
#### Step X:
#### Step X:




-----



#### Step 1: Clone the esp32-walkie-talkie Repository - DONE
Go to your [GitHub repository](https://github.com/jeffskinnerbox?tab=repositories)
and click the **New** button at the top right of the page.
Fill in the form to create your Github repository.

Next you do the commands below to clone atomic14's repository and push the clone to Github:

```bash
# clone the repository
cd ~/src/embedded-devices/
git clone https://github.com/atomic14/esp32-walkie-talkie.git

# remove referance to the clones github and build methodology
rm -r -f .git .github .vscode platformio*

# add you .gitignore file
cp ~/.bash/.gitignore .

# create a local repository and push the repository to your github
git init
git add --all
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/jeffskinnerbox/esp32-walkie-talkie.git
git push -u origin main
```

From here, I stripped out and moved around the repositories contents to conform
and compile successfully using my personal choose of build and trace message tools.
Specifically, I'm **not** using [PlatformIO][35], as atomic14 did,
but instead I'm using [Arduino IDE version 2.0.1][40], [Ardunio-CLI][36] and [Makefile][37].
You'll see how I did this via the Makefile below:

```makefile
#-------------------------------------------------------------------------------
#
# Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
# Version:      0.0.1
#
# USAGE
#   To build the executable
#       make build
#
#   To flash the device
#       make upload-usb
#       make upload-ota
#
#   To build the executable and flash the device
#       make
#       make all
#
#   To monitor execution of the device
#       screen /dev/ttyUSB0 9600,cs8cls
#       to terminate Ctrl-a :quit
#
#       telnet test-ota.local
#       to terminate Ctrl-a quit
#
#   Makefile Syntax
#       =  set the variable
#       ?= set the variable only if it's not set/doesn't have a value
#       := set variable as the output an executed shell script
#       += is used for appending more text to variables
#       $$ if you want a dollar sign to appear in your recipe, you must double it (‘$$’)
#
# SOURCES
#   The Makefile was created with the help of this website:
#   https://learn.sparkfun.com/tutorials/efficient-arduino-programming-with-arduino-cli-and-visual-studio-code/all
#
# DOCUMENTATION
#   https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5
#
#-------------------------------------------------------------------------------

# Use Bash shell instead of the default /bin/sh
SHELL := /bin/bash

# name of program being created
PROG = esp32-walkie-talkie

# type of package, architecture, and board in use
PACKAGE = esp32
ARCH =    esp32
BOARD =   nodemcu-32s

# serial port used by the board
PORT = /dev/ttyUSB0

#------------------------- names, tools, flags, paths --------------------------

# optional verbose compile/upload flag
#VERBOSE = -v

# fully qualified board name (FQBN)
FQBN = $(PACKAGE):$(ARCH):$(BOARD)

# location of the esptool used for usb flashing
ESPTOOL = /home/jeff/.arduino15/packages/esp32/tools/esptool_py/2.6.1/esptool.py

# string within names given to .bin and .elf files
VAR := $(shell echo $(FQBN) | tr ':' '.')

# path for temp-storage of binary, object, and core.a files
BUILD = /tmp/$(PROG)
BUILD_PATH = $(BUILD)/build

# paths to libraries and include files
LIBS = "/home/jeff/src/arduino/sketchbooks/libraries"

#---------------------------------- compiler -----------------------------------

# compiler and compiler flags
CC = arduino-cli compile
CC_FLAGS = $(VERBOSE) --fqbn $(FQBN) --build-path=$(BUILD_PATH) --build-cache-path=$(BUILD) --libraries $(LIBS)

#--------------------------------- usb upload ----------------------------------

# usb firmware flasher and flags
UPLOAD_USB = arduino-cli upload
UPLOAD_USB_FLAGS = $(VERBOSE) --fqbn $(FQBN) --port $(PORT) --input-dir $(BUILD_PATH)

#--------------------------------- ota upload ----------------------------------

# location of the espota.py used for ota flashing
ESPOTATOOL = /home/jeff/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py

# set ota password, ip address, and port for device
OTAHOSTNAME := $(shell grep OTAHOSTNAME secrets.h | cut -d" " -f3 | awk '{print substr($$0, 2, length($$0) - 2)}')
OTAPASS := $(shell grep OTAPASSWORD secrets.h | cut -d" " -f3 | awk '{print substr($$0, 2, length($$0) - 2)}')
OTAPORT := $(shell grep OTAPORT secrets.h | cut -d" " -f3)
OTAIP := $(shell ping -c1 $(OTAHOSTNAME).local | grep "bytes from" | cut -d " " -f5 | awk '{ print substr($$0, 2) }' | awk '{ print substr( $$0, 1, length($$0)-2 ) }')

# ota firmware flasher and flags
UPLOAD_OTA = python2 $(ESPOTATOOL)
UPLOAD_OTA_FLAGS = -d -i $(OTAIP) -p $(OTAPORT) -a $(OTAPASS) -f $(PROG).$(VAR).bin

#-------------------------------------------------------------------------------


# explicitly declare these target to be phony
.PHONY: help build upload upload-ota clean erase size

all: build upload                               # build and then upload via usb

all-ota: build upload-ota                       # build and then upload via ota

help:
	@echo ' '
	@echo 'Makefile for' $(PROG)
	@echo ' '
	@echo 'Usage:'
	@echo '   make              create all files and flash device via usb'
	@echo '   make all          create all files and flash device via usb'
	@echo '   make all-ota      create all files and flash device via ota'
	@echo '   make build        compile the code and create the *.elf file'
	@echo '   make upload       create the *.bin file and flash device via usb'
	@echo '   make upload-ota   create the *.bin file and flash device via ota'
	@echo '   make erase        erease the entire flash from the device and exit'
	@echo '   make size         print the flash size within the device and exit'
	@echo '   make clean        delete *.bin, *.elf, *.hex files'
	@echo '   make help         print this help message and exit'
	@echo ' '
	@echo 'Pass the option "--debug" to enable trace messaging (e.g. make --debug build)'
	@echo ' '

build:                                          # build the binary executable
	$(CC) $(CC_FLAGS) $(CURDIR)

upload:                                         # flash the binary executable via usb
	$(UPLOAD_USB) $(UPLOAD_USB_FLAGS) $(DURDIR)

upload-ota:                                     # flash the binary executable via ota
	@echo VAR = $(VAR)
	@echo OTAIP = $(OTAIP)
	@echo OTAPASS = $(OTAPASS)
	@echo OTAPORT = $(OTAPORT)
	@echo OTAHOSTNAME = $(OTAHOSTNAME)
	@echo UPLOAD_OTA_FLAGS = $(UPLOAD_OTA_FLAGS)
	$(CURDIR)/answerbot $(OTAIP) 23             # using telnet, reboot the device to do OTA
	sleep 10                                    # wait until device is ready for OTA start
	$(UPLOAD_OTA) $(UPLOAD_OTA_FLAGS)

erase:                                          # erase the entire flash
	$(ESPTOOL) erase_flash --port $(PORT)

size:                                           # determine the flash size
	$(ESPTOOL) flash_id --port $(PORT)

clean:                                          # delete all binaries and object files
	rm -r --force $(BUILD) *.bin *.elf *.hex
```

I modified the code until I got a clean compile.
At this point, I'll set aside this code and focus on assembling and testing the subsystems of the design much like atomic14 did within his series of videos.



-----



# Series of Test Builds
referance the list of sites in the "# My Walkie-Talkie Build Approach" section above

#### Step X: Breadboard the Microphone
Would this be of interest:
* [SparkFun Analog MEMS Microphone Breakout - VM2020](https://www.sparkfun.com/products/21537)

For my microphone, I chose the [Adafruit SPH0645LM4H][10] I2S MEMS Microphone.
Just like ['classic' electret microphones][25]
(a type of [condenser microphone][27]) which outputs an analog voltage,
[MEMS microphones][26] can detect sound and convert it to voltage but
doesn't have analog out, it's purely digital in format.
The I2S is a small, low-cost MEMS mic with a frequency range of about 50Hz - 15KHz.

Instead of an analog output, the I2S microphone has three digital pins:
Clock, Data, and Left-Right (Word Select) Clock.
When connected to your ESP32, the ["I2S Controller" on the ESP32][31] will drive
the clock and word-select pins at a high frequency and read out the data from the microphone
giving you a [pulse coded modulated (PCM)][28] signal
(specifically a [pulse width modulated][33] signal).
[I2S or Inter-IC Sound][30] (pronounced "eye-squared-ess"),
is an electrical serial bus interface standard,
introduced by Philips in 1986,
used for connecting digital audio devices together.
I2S has three signals, and an optional forth, to do its job:

* Continuious Serial Clock (SCK) and also called "Bit Clock" (BCLK)
* Word select (WS) and also called "Left-Right Clock" (LRCLK) or "Frame Sync" (FS)
* Serial Data (SD) sometimes called SDATA, SDIN, SDOUT, DACDAT, ADCDAT
* Aditional signal: Master Clock (MCLK) (256 times WS)

It is used to communicate PCM audio data between integrated circuits in an electronic device.
This approach resulting in simpler receiver amplifier to convert back to audio, called a [class D amplifier][32].
No binary words are created to represent the analog signal,
resulting in no need for [analog to digital conversion (ADC)][29]!

Sources:
* [ESP32 Audio Input - INMP441 and SPH0645 MEMS I2S Breakout Boards](https://www.youtube.com/watch?v=3g7l5bm7fZ8&list=PL5vDt5AALlRfGVUv2x7riDMIOX34udtKD&index=16)

#### Step X: Breadboard the Class D Amplifier
I purchased from Amazon the [Degraw DIY Speaker Kit][60].
This kit contains a [Dual-Channel PAM8403 5 Volt Class D Digital Audio Stereo Amplifier][61] + two [4 Ohm 3 watt Speakers][62].
This 30×22×16mm module can be powered by three AA batteries, or a USB Cell phone charger, or a rechargeable cell phone charge pack.
This module and speaker combination will allow me to test the ESP32's [I2S or Inter-IC Sound][30]
functionality with confidence that I have a working amplifier.

I purchased from Amazon the [Mini MP3 Player Audio Module][63] integrates with decoding module,
which supports common audio formats such as MP3, WAV, and WMA.
Besides, it also supports TF card with FAT16, FAT32 file system.

I purchased from Amazon the [Micro SD TF Card Adater Reader Module 6Pin SPI Interface Driver Module][64]
for reading & writing through the file system and the SPI interface driver.
This requires 5V VCC power.
The description of this item makes it seem like it will work with a 3v3 microcontroller, but it won't. The voltage regulator steps down 5v signals to 3v3 before going into the sd card, that's all it means. But if your signals are already at 3v3 going into this adapter, the level shifter/regulator seems to drop it too much to work.

[60]:https://www.amazon.com/dp/B07CRVRG83
[61]:https://www.amazon.com/HiLetgo-Amplifier-Dual-Channel-Amplifiers-Potentiometer/dp/B01DKAI51M
[62]:https://www.amazon.com/CQRobot-Speaker-Interface-Electronic-Projects/dp/B0822XCPT8?th=1
[63]:https://www.amazon.com/gp/product/B07Y2YKYRS?th=1
[64]:https://www.amazon.com/gp/product/B07BJ2P6X6/

#### Step X: Breadboard the Microphone and Amplifier
Next lets make sure we can get the I2S microphone and [class D amplifier][32] working together nicely.
Generally we

Sources:
* [#419 ESP32 Audio Tutorial with lots of examples](https://www.youtube.com/watch?v=a936wNgtcRA)
* [ESP32 Audio Output with I2S DMA and the MAX98357A Class D Amplifier](https://www.youtube.com/watch?v=At8PDQ3g7FQ&list=PL5vDt5AALlRfGVUv2x7riDMIOX34udtKD&index=17)
* [ESP32 Audio DMA Settings Explained - dma_buf_len and dma_buf_count](https://www.youtube.com/watch?v=ejyt-kWmys8&list=PL5vDt5AALlRfGVUv2x7riDMIOX34udtKD&index=7)

#### Step X: Breadboard the Microphone, Amplifier, and ESP32

Sources:
* [I2S Audio general](https://www.youtube.com/playlist?list=PLpyo4J4M9YqJLh0B8YXNB0zza0uWIeguO)
    * [ESP32 – Intro to I2S Part 1](https://www.xtronical.com/i2s-ep1/)
    * [I2S on ESP3 – Part 2, WAV’s](https://www.xtronical.com/i2s-ep2/)
    * [ESP32 I2S Part 3 – Playing Wavs from SD Cards](https://www.xtronical.com/i2s-ep3/)
    * [I2S Player (Part 4) : Adding volume control](https://www.xtronical.com/i2s_ep4/)
    * [Writing your own ESP32 I2S WAV Player – Part 5 – Mixing sounds](https://www.xtronical.com/i2s_ep5/)
* [ESP32 Sound - Working with I2S](https://www.youtube.com/watch?v=m-MPBjScNRk)
* [ESP32 Audio Input Using I2S and Internal ADC](https://www.youtube.com/watch?v=pPh3_ciEmzs&list=PL5vDt5AALlRfGVUv2x7riDMIOX34udtKD&index=16)
* [Sound with ESP32 – I2S Protocol](https://dronebotworkshop.com/esp32-i2s/)

#### Step X:
#### Step X:
#### Step X:
#### Step X:
#### Step X: ESP-NOW
* [ESP NOW - Peer to Peer ESP32 Communications](https://dronebotworkshop.com/esp-now/)
* [ESP-NOW | Getting Started With ESP8266 Including Demo Sketches](https://www.youtube.com/watch?v=_cNAsTB5JpM)
* [ESPNOW for beginners!](https://www.youtube.com/watch?v=Ydi0M3Xd_vs)
#### Step X:
#### Step X: WiFiManager
* [ESP32 WiFiManager – Easy WiFi Provisioning](https://dronebotworkshop.com/wifimanager/)
* [WiFiManager with ESP32 - Stop Hard-coding WiFi Credentials](https://www.youtube.com/watch?v=VnfX9YJbaU8)

* [PersWiFiManager](http://ryandowning.net/PersWiFiManager/examples.html)
* [DoubleResetDetector](https://github.com/datacute/DoubleResetDetector/blob/master/examples/minimal/minimal.ino)

* [WiFiManager](https://github.com/tzapu/WiFiManager/tree/development#how-it-works)
* [Avoid Hard-Coding WiFi Credentials on Your ESP8266 Using the WiFiManager Library](https://www.instructables.com/id/Avoid-Hard-Coding-WiFi-Credentials-on-Your-ESP8266/)
#### Step X:

#### What About MicroPython
* [MicroPython I2S Audio with the ESP32](https://www.youtube.com/watch?v=UXt27kOokh0&list=PL5vDt5AALlRfGVUv2x7riDMIOX34udtKD&index=5)



--------



# Raspberry Pi Zero Walkie-Talkie
The TalkiPi project is a headless mumble client for the Raspberry Pi,
utilizing static config and GPIO for status LEDs and a button for push to talk

* [Long-Range Walkie-Talkies using a Raspberry Pi Zero](https://www.youtube.com/watch?v=b9qIMWn8uyY)
* [I built a wifi "walkie" talkie for my kids; now you can too!](https://projectable.me/i-built-a-wifi-walkie-talkie-for-my-kids-now-you-can-too/)
* [Building an Internet Walkie Talkie using a Raspberry Pi v3](https://projectable.me/building-an-internet-walkie-talkie-using-a-raspberry-pi-v3/)
* [talkiepi Updates (November 2017)](https://projectable.me/talkiepi-nov2017-updates/)
* [talkiepi](https://github.com/dchote/talkiepi)
* [talkiepi case](https://www.thingiverse.com/thing:3214731)

* [Raspberry Pi Walkie Talkies](https://github.com/rydercalmdown/pi_walkie_talkie)

ReSpeaker 2-Mic Pi HAT: https://www.amazon.com/KEYESTUDIO-ReSpeaker-2-Mic-V1-0-Raspberry/dp/B07H3T8SQY
Raspberry Pi Zero W
Loudspeakers: https://www.amazon.com/gp/product/B0738NLFTG/
              https://www.amazon.com/MakerHawk-Full-Range-Advertising-Separating-JST-PH2-0mm-2/dp/B07FTB281F/
TalkiePi project: https://github.com/dchote/talkiepi
Setup instructions:
    * https://github.com/CustomMachines/talkiepi
    * https://github.com/mnoonan296/talkiepi
    * https://github.com/MarcusWolschon/RasPi_stage_intercom



--------



# ESP32 Walkie-Talkie Using Mumble Server
The idea is to extend the walkie-talkie's coverage to the enire Internet

* [mumble-client · GitHub Topics](https://github.com/topics/mumble-client)
    * [mumlib - simple Mumble client library](https://github.com/slomkowski/mumlib)
    * [mumble-client](https://github.com/tf2pickup-org/mumble-client)
    * [MumbleChannelESP](https://github.com/revilo196/MumbleChannelESP)
    * [libmumble_client](https://github.com/janhenke/libmumble_client)
    * [mumlib2 - simple Mumble client library](https://github.com/Mixaill/mumlib2)



--------


# Other Similar Platforms
* [ESP32-S3 board features 2.8-inch display, Blackberry-like keyboard, and optional LoRaWAN connectivity](https://www.cnx-software.com/2023/06/30/esp32-s3-board-features-2-8-inch-display-blackberry-like-keyboard-lorawan/)
    * [LILYGO® T-Deck ESP32-S3 LoRa Module 2.8 inch LCD Development Board LoRaWAN Long Range 433MHz 868MHz 915MHz With WIFI Bluetooth](https://www.aliexpress.us/item/3256805505920840.html?aff_fcid=e658ccdc09654a24bb28026bcae6fe58-1688225477037-09206-_Dd8eYNx&tt=CPS_NORMAL&aff_fsk=_Dd8eYNx&aff_platform=shareComponent-detail&sk=_Dd8eYNx&aff_trace_key=e658ccdc09654a24bb28026bcae6fe58-1688225477037-09206-_Dd8eYNx&terminal_id=68305b5097854f1caa43b9878a24fa29&afSmartRedirect=y&gatewayAdapt=glo2usa4itemAdapt)
    * [LilyGO T-Deck](https://github.com/Xinyuan-LilyGO/T-Deck/tree/master)



--------



## External Antenna
* [#291 External antennas and ESP32 Long-Range mode](https://www.youtube.com/watch?v=2rujjTOPIRU)
* [Suggested: #291 External antennas and ESP32 Long-Range mode](https://www.youtube.com/watch?v=PUppoaePi3A&list=RDCMUCu7_D0o48KbfhpEohoP7YSQ&index=2)
* [#368 How to build performing antennas for LoRa, WiFi, 433MHz, Airplanes etc.(NanoVNA, MMANA-GAL)](https://www.youtube.com/watch?v=6cVYsHCLKq8)
* [#191 Optimizing Antennas using a cheap N1201SA VNA (Vector Impedance Analyzer), LoRa, Review](https://www.youtube.com/watch?v=ZpKoLvqOWyc)

# Extending Range of ESP32
* [#291 External antennas and ESP32 Long-Range mode](https://www.youtube.com/watch?v=2rujjTOPIRU&t=0s)
* [#292 Wi-Fi Antennas with Gain and ESP32 Long-Range Mode (part2)](https://www.youtube.com/watch?v=PUppoaePi3A)

# Jami
* [Jami - An Open Source, Peer-to-Peer, Audio, Video, Conferencing, Chat, and Screen Sharing System](https://www.youtube.com/watch?v=2Wi99K33qrw)



------



# Mumble
This voice-over-internet protocol (VoIP) application available on Android, iOS, macOS, Windows and Linux.
Mumble is considered among the best voice chat applications for gamers.
Its a free and open-source platform with end-to-end encryption to protect the data across the servers.

* [Mumble Server](https://www.turnkeylinux.org/mumble)
* [Build your own Raspberry Pi Mumble Server](https://pimylifeup.com/raspberry-pi-mumble-server/)
* [Tutorial: How to setup & use Mumble VoIP Client (Alt to Teamspeak & Ventrilo)](https://www.youtube.com/watch?v=t8VbE478kaw)
* [talKKonnect - A Headless Mumble Client/Transceiver/Walkie Talkie/Intercom/Gateway for Single Board Computers, PCs or Virtual Environments (IP Radio/IP PTT)](https://github.com/talkkonnect/talkkonnect)

Mumble
    * https://www.mumble.com/
    * [Mumble][21]
    * [Mumble Push To Talk](https://www.mumble.com/support/mumble-server-push-to-talk.php)
    * Build your own Raspberry Pi Mumble Server: https://pimylifeup.com/raspberry-pi-mumble-server/
Free Mumble Server (iMumble): https://www.imumble.nl/

## Mumble Server
The `mumble` server is named `murmur`.

### Mumble Server in Docker Container
* [Complete mumble server using Docker-compose](https://raddinox.com/complete-mumble-server-using-docker)

## Mumble Client
* [Tutorial: How to setup & use Mumble VoIP Client](https://www.youtube.com/watch?v=t8VbE478kaw)
* [Android: Mumla - Mumble VoIP](https://play.google.com/store/apps/details?id=se.lublin.mumla)
* [General-purpose mumble client library written in python, for writing mumble bots](https://github.com/frymaster/mumbleclient)

## Via Ham Radio
* [Remote Ham Radio Rig | Mumble Server not RigPi](https://www.youtube.com/watch?v=DBfyQRvOmag)


------



# Antenna Improvements
* [NEW PI ZERO GAINS UNAPPROVED ANTENNAS YET AGAIN](https://hackaday.com/2021/12/13/new-pi-zero-gains-unapproved-antennas-yet-again/)

# Open-Source Cell Phone
* [Open-Source Cell Phone Based On ESP32](https://hackaday.com/2023/08/03/open-source-cell-phone-based-on-esp32/)







# WiFi Repeater (aka WiFi NAT Router)
* [18650 Brings ESP8266 WiFi Repeater Along For The Ride](https://hackaday.com/2021/04/08/18650-brings-esp8266-wifi-repeater-along-for-the-ride/)
* [esp_wifi_repeater](https://github.com/martin-ger/esp_wifi_repeater)
* [ESP32 NAT Router](https://github.com/martin-ger/esp32_nat_router)


* [Long-Distance Text Communication With LoRa](https://hackaday.com/2022/05/25/long-distance-text-communication-with-lora/)



[01]:https://hackaday.com/2021/04/07/an-esp32-walkie-talkie-for-those-spy-radio-moments/
[02]:https://www.youtube.com/c/greatscottlab
[03]:https://www.youtube.com/watch?v=SZYwvvh6m-s
[04]:https://www.youtube.com/c/atomic14
[05]:https://www.youtube.com/c/AndreasSpiess
[06]:https://www.amazon.com/gp/product/B093CJ4V3Y
[07]:https://www.amazon.com/gp/product/B0946T98GR/
[08]:https://www.adafruit.com/product/3006
[09]:https://github.com/atomic14/esp32-walkie-talkie
[10]:https://www.adafruit.com/product/3421
[11]:https://easyeda.com/editor#id=4a8eb22811c440fcb3db992a8feaf57a
[12]:https://oshwlab.com/chris_9044/esp32-walkie-talkie
[13]:https://www.pcbway.com/
[14]:https://www.adafruit.com/product/805
[15]:https://www.adafruit.com/product/1009
[16]:https://www.amazon.com/gp/product/B0821521CV
[17]:https://www.amazon.com/gp/product/B07PS653CD
[18]:https://www.youtube.com/watch?v=YJ25eQRbhaQ&t=0s
[19]:https://www.youtube.com/watch?v=b9qIMWn8uyY&t=95s
[20]:https://www.youtube.com/playlist?list=PL5vDt5AALlRfGVUv2x7riDMIOX34udtKD
[21]:https://www.mumble.info/
[22]:https://www.espressif.com/en/products/software/esp-now/overview
[23]:https://wiki.mumble.info/wiki/3rd_Party_Applications#Clients
[24]:https://www.amazon.com/HiLetgo-Serial-128X64-Display-Color/dp/B06XRBTBTB?th=1
[25]:https://en.wikipedia.org/wiki/Electret_microphone
[26]:https://mynewmicrophone.com/what-is-a-mems-micro-electro-mechanical-systems-microphone/
[27]:http://www.learningaboutelectronics.com/Articles/Condenser-microphones
[28]:https://www.tutorialspoint.com/digital_communication/digital_communication_pulse_code_modulation.htm
[29]:https://www.techtarget.com/whatis/definition/analog-to-digital-conversion-ADC
[30]:https://en.wikipedia.org/wiki/I%C2%B2S
[31]:https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html
[32]:https://www.youtube.com/watch?v=O1UagNkcxi4
[33]:https://www.analogictips.com/pulse-width-modulation-pwm/
[34]:https://unexpectedmaker.com/tinys2
[35]:https://platformio.org/
[36]:https://arduino.github.io/arduino-cli/0.27/
[37]:https://opensource.com/article/18/8/what-how-makefile[38]:
[39]:https://mjoldfield.com/atelier/2021/11/tiny-power.html
[40]:https://www.arduino.cc/en/software
[41]:https://www.amazon.com/gp/product/B0718T232Z/
[42]:https://www.youtube.com/watch?v=d_h38X4_eQQ
[43]:https://github.com/atomic14/esp32_audio
[44]:https://www.youtube.com/watch?v=m-MPBjScNRk
[45]:https://www.youtube.com/watch?v=lgDu88Y411o
[46]:https://www.youtube.com/watch?v=6NsBN42B80Q
[47]:https://www.adafruit.com/product/5770
[48]:https://www.youtube.com/watch?v=MilQztrItbA&list=PL5vDt5AALlRdyy0JPfn894z5WzduzAiKi&index=1
[49]:https://www.youtube.com/watch?v=UXt27kOokh0&list=PL5vDt5AALlRdMpeFUZo2dFimy6QLcWG7a&index=1
[50]:https://www.youtube.com/watch?v=a936wNgtcRA
[51]:https://github.com/pschatzmann/arduino-audio-tools/wiki/Design-Goals
[52]:https://www.pschatzmann.ch/home/2021/10/14/a-simple-streaming-mp3-player/
[53]:
[54]:
[55]:
[56]:
[57]:
[58]:
[59]:
[60]:

