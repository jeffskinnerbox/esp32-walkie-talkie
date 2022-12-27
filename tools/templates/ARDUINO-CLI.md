<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----


################################ JUNK  #########################################
Had problems with the /dev/ttyUSB* dive never being created when plugging in a ESP.
Procedure below was suggested but didn't work

# make the device file
sudo mknod /dev/ttyUSB0 c 188 0

# list permission on the device file
ls -l /dev/ttyUSB0
crw-r--r-- 1 root root 188, 0 Nov 10 11:52 /dev/ttyUSB0

# make it rw for all
sudo chmod a+rw /dev/ttyUSB0

# list permission on the device file
ls -l /dev/ttyUSB0
crw-rw-rw- 1 root root 188, 0 Nov 10 11:52 /dev/ttyUSB0

# change the group for the file to dialout
sudo chown root:dialout /dev/ttyUSB0

# list permission on the device file
s -l /dev/ttyUSB0
crw-rw-rw- 1 root dialout 188, 0 Nov 10 11:52 /dev/ttyUSB0

# list members of group dialout
getent group dialout
dialout:x:20:jeff
################################ JUNK  #########################################
running dmesg I spotted the message "usbfs: interface 0 claimed by cp210x while 'brltty' sets config #1"

Using that message as a search, I found this - https://bugs.launchpad.net/ubuntu/+source/brltty/+bug/874181
This bug is somewhat hardware specific, affecting all users of the USB serial controller, device 10c4:ea60.
ESP32 = device 10c4:ea60.

# check if brltty deamon is running
$ ps aux | grep brltty

# delete configuration and/or data files of brltty and it's dependencies
sudo apt-get purge --auto-remove brltty
################################ JUNK  #########################################





# Arduino On The Command Line
For someone like myself, who is at home with Linux as my OS and Vim as my editor,
using the [Arduino IDE][03] for Arduino coding is a step back into the stone age.
If you are used to doing these things yourself and controlling the organization of your code
then the Arduino IDE does some really arbitrary and annoying things.
For example, try sharing a common file between your sketch and modules (that is, Arduino libraries)
which you may or not use depending on what sketch you are compiling.
The Arduino will compile everything in the directory with the included file.
You have no say in what gets compiled and in what context.

I'm not the first to lament about the this topic, but more importantly,
some people have stepped forward and done something about it!
The folks at Arduino have given us [Installing Arduino on Linux][01], which gives us
a Arduino IDE, but more importantly, the foundation of this package are executables
and libraries that can be called executed outside of the IDE.
Others like [Martin Oldfield and Sudar Muthu][02] have given us the Linux package `arduino-mk`
which allows you to use the core tools that form the foundation for the Arduino IDE.
Those foundational tools are within the package `arduino-core`.
The actual Arduino IDE is in the package `arduino`.
But these Makefile tools don't easily support the ESP8266/ESP32 boards,
which prompted the creation of [Esp8266-Arduino-Makefile][17]
to get support for the ESP architecture but abandoned the Arduino boards.

I wanted to avoid all this confusion and get on a path that will remain compatible
with the future evolution of the Arduino IDE.
I discovered that [Arduino has released a command line interface (CLI)][13],
called `arduino-cli`,
that performs most of the features present in the Arduino IDE.
The Arduino CLI aims to replace the majority of features the Arduino IDE has without the graphical UI.
The CLI will allow you to Install new libraries, create new projects,
and compile projects directly from the command line.
And it appears the goal of the Arduino CLI in any kind of script for the command line.
So in principle, it should work in a standard Makefile too.

>**NOTE:** As of January 2, 2012 `arduino-cli` is **not** considered stable
>and is being offered as a alpha version.
>See the [`arduino-cli` github site][14] for more information.
>At this time, the "offical" recommendation for a Arduino command line tool is
>[`arduino`][16] and the not so "offical" [PlatformIO][22].

One of the things that comes free with the Arduino IDE is a serial monitor.
Of course, this is an indispensable tool for both testing and debugging,
and I don't want to loss this capability.
We haven't abandoned this capability since there are Linux alternatives.
The post [Arduino and Linux TTY][04] provides a series of solutions.
(What is listed below isn't always functional for every need, so consult the posting for more ideas.)

Some of your best monitoring options are:

* Assuming the Arduino's USB is plugged into `/dev/ACM0` and the port speed is set to `57600`,
`cu -l /dev/ttyACM0 -s 57600` will get you simple connect.
For more commands, consult the [`cu` manual page][07].
* You can use [`screen`][08] to provide a more interactive serial monitor session with an Arduino.
The command `screen /dev/ttyACM0 57600` is similar to the above.
See [Linux Screen Tutorial and How To][05]
[Screen User's Manual][06] for more information about the power of `screen`.

What follows is how I installed the [Arduino IDE][03], the [Arduino CLI][14]
and used it with [GNU Make (aka Makefile)][15].
Doing this, I can create an all command line (or nearly all)
software development experance while still leveraging the popularity of the Arduino IDE platform.

-----

## Installing the Arduino IDE
[Arduino][11] is an open-source platform used for building electronics projects.
Arduino consists of both a physical programmable circuit board (often referred to as a microcontroller)
and a piece of software, or IDE (Integrated Development Environment) that runs on your computer,
used to write and upload computer code to the physical board.
There is [Linux build of the Arduino IDE][09],
as well as [alternatives to the standard Arduino IDE][10].

### Step 1: Installing Arduino IDE - DONE
You could install the Arduino IDE via the Ubuntu Software Center and search for Arduino.
Alternatively, you can install via the command line by running the following:

```bash
# install arduino from software repositories (not recommended)
sudo apt-get update && sudo apt-get install arduino arduino-core
```

The above installs a package from the Ubuntu software repositories,
which likely to be an older Arduino IDE version.
The newest version of the Arduino can be downloaded from the [Arduino download page][12].
(**NOTE:** Using this method Arduino software won't automatically be updated,
so you should check Arduino website every few months and download
a new version if one is available.)

```bash
# download the software - arduino-nightly-linux64.tar.xz
# from https://www.arduino.cc/en/Main/Software
#                    or
# ARDUINO 1.8.8 official release at
# https://www.arduino.cc/en/Main/Donate

# uncompress the tarball
cd ~/Downloads
tar -xvf arduino-1.8.8-linux64.tar.xz

# move any old install of arduino to a safe place
mv ~/src/arduino ~/tmp/old-arduino

# move the result folder to /opt directory for global use
mv ~/Download/arduino-1.8.8 ~/Download/arduino
mv ~/Downloads/arduino ~/src

# run the script to install both desktop shortcut and launcher icon
cd ~/src/arduino
./install.sh
ln -s ~/src/arduino/arduino ~/bin/arduino

# brltty (braille device) which will conflict with the Arduino
sudo apt-get remove brltty

# print version and build information for the arduino ide and exit
$ arduino --version
Picked up JAVA_TOOL_OPTIONS:
Loading configuration...
Initializing packages...
Preparing boards...
Arduino: 1.8.8
```

This last step will also create two directories with some content:
`$HOME/Arduino` and `$HOME/.arduino15`.
We'll "reposition" the former below but the later will remain
and will play an important role in preferences, boards,
and libraries within the Arduino environment.

### Step 2: Quick Test - DONE
When the Arduino Software IDE is properly installed you can execute
the IDE via the command `arduino &>/dev/null &` or the desktop icon.

>**NOTE:** It might happen that when you upload a sketch
>(after you have selected your board and serial port),
>you get an error Error opening serial port.
>If you get this error, you need to [set serial port permission][23].

### Step 3: Setup Your Arduino IDE Preferences - DONE
Normally, running the `arduino` command starts the IDE,
optionally loading any `.ino` files specified on the commandline.
Also, it normally puts all your Arduino sketches and project libraries within
a common directory defaulting to `$HOME/Arduino`.
Lets change this location:

1. Start the Arduino IDE, select the menu button **File** > **Preferences**
2. Change the "Sketchbook location" to `$HOME/src/arduino/sketchbooks`
(make sure you do this via the **Browse** button) and save it.
3. Restart the Arduino and the directory will be prepared automatically for you.

>**NOTE:** Some preferences, but not all,
>can be controlled from the Preferences dialog within the Arduino environment (see [here][19]).
>More preferences can be edited directly in the file `$HOME/.arduino15/preferences.txt`
>but only edit when the Arduino IDE is **not** running.

### Step 4: Remove Old Sketch Directory - DONE
The new `sketchbook` directory create above contains a subdirectory `libraries`
(created the next time you start the Arduino IDE).
`$HOME/src/arduino/sketchbooks/libraries` is where all your libraries will go
(both downloads and personal ones you create).
Improperly installing libraries can be a major source of confusion within the Arduino world.
Check out Adafruit's "[All About Arduino Libraries][18]" for their proper care and feeding.

No that your sketches and libraries have a new location,
make sure to remove the default location created during the Arduino IDE install:

```bash
# remove the old sketchbook location
rm -r -f Arduino
```

### Step 5: Move Back Saved Sketches - DONE
If you saved any sketch from a previous install, you can simply copy them into the
`$HOME/src/arduino/sketchbooks` directory.
For example:

```bash
# goto you sketchbook location
cd ~/src/arduino/sketchbooks/

# move your ald sketches into you new sketchbook
cp -r ~/tmp/arduino-Nov-5-2018/sketch-folder/display-test .
cp -r ~/tmp/arduino-Nov-5-2018/sketch-folder/i2c-port-scanner .
cp -r ~/tmp/arduino-Nov-5-2018/sketch-folder/NTPClient-2 .
cp -r ~/tmp/arduino-Nov-5-2018/sketch-folder/temp-hum .
cp -r ~/tmp/arduino-Nov-5-2018/sketch-folder/esp8266-dht22-losant .
cp -r ~/tmp/arduino-Nov-5-2018/sketch-folder/i2c-scanner .
cp -r ~/tmp/arduino-Nov-5-2018/sketch-folder/NTPClient .
cp -r ~/tmp/arduino-Nov-5-2018/sketch-folder/ntp-clock .
cp -r ~/tmp/arduino-Nov-5-2018/sketch-folder/timer-test .
```

### Step 6: Load Needed Boards - DONE
The ESP8266 addon for the Arduino IDE
is provided by the ESP8266 community.
([lead by Ivan Grokhotkov of Espressif][36]).
Check out the [ESP8266 Arduino GitHub repository][37] for more information.
To see more detailed instructions on installing  ESP8266 support on your Arduino IDE,
see [SparkFun's ESP8266 Thing Hookup Guide][38].
[ESP8266 Arduino libraries documentation][39] can be found online.

Here are the abbreviated installation steps:

1. Start the Arduino Software IDE via the command `arduino &>/dev/null &`.
2. Open the preferences window from the Arduino IDE.
Go to **File** > **Preferences**
3. Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into
the **Additional Board Manager URLs** field as shown in the figure below. Then, click the **OK** button.
(**NOTE:** If you already have a URL in there, and want to keep it,
you can separate multiple URLs by placing a comma between them.)
4. Navigate to the Board Manager by going to **Tools** > **Board** menu > **Boards Manager**.
There should be a couple new entries in addition to the standard Arduino boards.
Look for ESP8266. Click on that entry, then select **Install**.
5. While your at it, you should repeat steps 2,3 & 4 for the ESP32 (see [arduino-esp32 GitHub][40]).
These libraries are located at https://dl.espressif.com/dl/package_esp32_index.json

If you need additional libraries and this can be done via repeating the process above.

>**NOTE: The Arduino command line tool, `arduino-cli`, appears to be able to install boards
>(via `arduino-cli core install esp32:esp32` and `arduino-cli core install esp8266:esp8266`)
>but using the Arduino IDE may be the simplest way for now.

### Step X: Install SPIFFS Tools
* [ESP8266 Web Server Files With SPIFFS Flash Memory Using Arduino IDE](https://www.youtube.com/watch?v=pfJROpQg-Is&feature=youtu.be)
* [#121 SPIFFS and JSON to save configurations on an ESP8266](https://www.youtube.com/watch?v=jIOTzaeh7fs)
* [Using ESP8266 SPIFFS](https://www.instructables.com/id/Using-ESP8266-SPIFFS/)
* [ESP8266 Arduino Core: File System](http://esp8266.github.io/Arduino/versions/2.0.0/doc/filesystem.html#flash-layout)
* [Example of ESP8266 Flash File System (SPIFFS)](https://circuits4you.com/2018/01/31/example-of-esp8266-flash-file-system-spiffs/)

### Step X: Uninstall Arduino IDE
The Arduino IDE package comes with an uninstall script but it doesn't do a complete removal
of all the configuration files and modifications made above.
The procedure below should do the trick.

Copy any of your sketch and personal libraries you may wish to keep.
Don't worry about public libraries since they can be reinstalled for sources.

```bash
# move to the location of the arduino ide and run the uninstall script
cd ~/src/arduino
./uninstall.sh

# remove your symbolic link to the arduino ide executable
rm ~/bin/arduino

# remove the entire arduino ide directory
cd ~/src
rm -r -f arduino

# remove the arduino ide confguration files
cd ~
rm -r -f .arduino15
```

-----

## Installing the Arduino Commond Line Interface - `arduino-cli`
[Arduino has released a command line interface (CLI)][13], called `arduino-cli`,
that performs most of the features present in the Arduino IDE.
The CLI will allow you to Install new libraries, create new projects,
and compile projects directly from the command line.
The goal of the Arduino CLI is to be including it in Makefile
or in any kind of script for the command line.
The Arduino CLI aims to replace the majority of features
the Arduino IDE has without the graphical UI.

There isn't a great deal of documentation on `arduino-cli`
but I found these sites useful:

* [Arduino now has a command line interface (CLI)](https://hub.packtpub.com/arduino-now-has-a-command-line-interface-cli/)
* [Tutorial on the Arduino Command Line Interface (CLI)][21]
* [Arduino Command Line Interface](https://github.com/arduino/arduino-cli)
* [Arduino CLI Command Line Interface Getting Started (Mac and Windows)](https://www.youtube.com/watch?v=8LPSjucQoso)
* [Arduino Gets Command Line Interface Tools That Let You Skip the IDE](https://hackaday.com/2018/08/26/arduino-gets-command-line-interface-tools-that-let-you-skip-the-ide/)

>**NOTE:** As of January 2, 2012 `arduino-cli` is **not** considered stable
>and is being offered as a alpha version.
>See the [`arduino-cli` github site][14] for more information.

### Step 1: Install arduino-cli - DONE
The the [`arduino-cli` github site][14] give you a choose of installing the "stable"
version or the latest source.
I'm using the latest source and will update frequently as it evolves.

```bash
# install the go language
sudo apt install golang-go

# install GOPATH variable and restart your shell
echo "export GOPATH=$HOME/src/go_code" >> ~/.bashrc
echo "export PATH=$PATH:$GOPATH/bin" >> ~/.bashrc
source ~/.bashrc

# install the arduino-cli tool
go get -u github.com/arduino/arduino-cli

# put the arduino-cli executable in your PATH
ln -s $GOPATH/bin/arduino-cli ~/bin/arduino-cli
```

## Step 2: Create the Default .cli-config.yml File (Failed) - DONE
I found out the hard way about the **non-existance** of the `.cli-config.yml` file.
There is no good documentation on this file or what it does within `arduino-cli`
but I found some hints on the video "[Tutorial on the Arduino Command Line Interface (CLI)][21]"
(at around 2:37 minutes)

But the most insight came when I ran the following commands:

```bash
# dump out the configuration file but with debug on for more information
$ arduino-cli config dump --debug
INFO[0000] Initiating configuration
INFO[0000] Unserializing configurations from /home/jeff/src/go_code/bin/.cli-config.yml
WARN[0000] Error reading config, using default configuration  error="open /home/jeff/src/go_code/bin/.cli-config.yml: no such file or directory"
WARN[0000] Did not manage to get config file, using default configuration  error="open /home/jeff/src/go_code/bin/.cli-config.yml: no such file or directory"
INFO[0000] Checking if CLI is Bundled into the IDE
INFO[0000] Candidate IDE Directory: /home/jeff/src/go_code/bin
INFO[0000] CLI is not bundled into the IDE
INFO[0000] Configuration set
INFO[0000] arduino-cli-0.3.3-alpha.preview
INFO[0000] Starting root command preparation (`arduino`)
INFO[0000] Formatter set
INFO[0000] Executing `arduino config dump`
proxy_type: auto
sketchbook_path: /home/jeff/Arduino
arduino_data: /home/jeff/.arduino15
board_manager: null
```

This says the `.cli-config.yml` doesn't exist and it uses "sketchbook_path: /home/jeff/Arduino".
If I don't change this,
loading of libraries will be within `/home/jeff/Arduino`
and not `/home/jeff/src/arduino` where I want them.

Checking out some of the `.cli-config.yml` examples, I concluded that I need to create a file
`$HOME/src/go_code/bin/.cli-config.yml` with the following contents:

```yml
Example1:
   - paths:
     - sketchbook_path: /home/jeff/src/arduino
     - arduino_data: /home/bcmi/.arduino15
   - proxy:
     - type: auto
```

**But this didn't work!!**
Just going to have to live with `$HOME/Arduino` for now.

### Step 3: Update & Upgrade arduino-cli - DONE
If you are running a fresh install of the `arduino-cli`,
you probably need to update the platform indexes
(you'll see these indexes in `~/.ardunio15`) by running:

```bash
# update package index of cores
$ arduino-cli core update-index
Updating index: package_index.json downloaded

# shows the list of installed platforms
$ arduino-cli core list
ID                   	Installed	Latest	Name
esp32:esp32@1.0.0    	1.0.0    	1.0.0
esp8266:esp8266@2.4.2	2.4.2    	2.4.2

# list all known boards and their corresponding FQBN
$ arduino-cli board listall
Board Name                     	FQBN
"WeMos" WiFi&Bluetooth Battery 	esp32:esp32:WeMosBat
4D Systems gen4 IoD Range      	esp8266:esp8266:gen4iod
ALKS ESP32                     	esp32:esp32:alksesp32
Adafruit ESP32 Feather         	esp32:esp32:featheresp32
Adafruit Feather HUZZAH ESP8266	esp8266:esp8266:huzzah
Amperka WiFi Slot              	esp8266:esp8266:wifi_slot
Arduino                        	esp8266:esp8266:arduino-esp8266
DOIT ESP32 DEVKIT V1           	esp32:esp32:esp32doit-devkit-v1
Digistump Oak                  	esp8266:esp8266:oak
Dongsen Tech Pocket 32         	esp32:esp32:pocket_32
ESP32 Dev Module               	esp32:esp32:esp32
ESP32 Pico Kit                 	esp32:esp32:pico32
ESP32 Wrover Module            	esp32:esp32:esp32wrover
ESP32vn IoT Uno                	esp32:esp32:esp32vn-iot-uno
ESPDuino (ESP-13 Module)       	esp8266:esp8266:espduino
ESPea32                        	esp32:esp32:espea32
ESPectro32                     	esp32:esp32:espectro32
ESPino (ESP-12 Module)         	esp8266:esp8266:espino
ESPresso Lite 1.0              	esp8266:esp8266:espresso_lite_v1
ESPresso Lite 2.0              	esp8266:esp8266:espresso_lite_v2
Electronic SweetPeas - ESP320  	esp32:esp32:esp320
FireBeetle-ESP32               	esp32:esp32:firebeetle32
Generic ESP8266 Module         	esp8266:esp8266:generic
Generic ESP8285 Module         	esp8266:esp8266:esp8285
Heltec_WIFI_Kit_32             	esp32:esp32:heltec_wifi_kit_32
Heltec_WIFI_LoRa_32            	esp32:esp32:heltec_wifi_lora_32
Hornbill ESP32 Dev             	esp32:esp32:hornbill32dev
Hornbill ESP32 Minima          	esp32:esp32:hornbill32minima
IntoRobot Fig                  	esp32:esp32:intorobot-fig
LOLIN D32                      	esp32:esp32:d32
LOLIN D32 PRO                  	esp32:esp32:d32_pro
LOLIN(WEMOS) D1 R2 & mini      	esp8266:esp8266:d1_mini
LOLIN(WEMOS) D1 mini Lite      	esp8266:esp8266:d1_mini_lite
LOLIN(WEMOS) D1 mini Pro       	esp8266:esp8266:d1_mini_pro
M5Stack-Core-ESP32             	esp32:esp32:m5stack-core-esp32
M5Stack-FIRE                   	esp32:esp32:m5stack-fire
MH ET LIVE ESP32DevKIT         	esp32:esp32:mhetesp32devkit
MH ET LIVE ESP32MiniKit        	esp32:esp32:mhetesp32minikit
Microduino-CoreESP32           	esp32:esp32:CoreESP32
Nano32                         	esp32:esp32:nano32
Node32s                        	esp32:esp32:node32s
NodeMCU 0.9 (ESP-12 Module)    	esp8266:esp8266:nodemcu
NodeMCU 1.0 (ESP-12E Module)   	esp8266:esp8266:nodemcuv2
NodeMCU-32S                    	esp32:esp32:nodemcu-32s
Noduino Quantum                	esp32:esp32:quantum
ODROID ESP32                   	esp32:esp32:odroid_esp32
OLIMEX ESP32-EVB               	esp32:esp32:esp32-evb
OLIMEX ESP32-GATEWAY           	esp32:esp32:esp32-gateway
Olimex MOD-WIFI-ESP8266(-DEV)  	esp8266:esp8266:modwifi
Onehorse ESP32 Dev Module      	esp32:esp32:onehorse32dev
Phoenix 1.0                    	esp8266:esp8266:phoenix_v1
Phoenix 2.0                    	esp8266:esp8266:phoenix_v2
Seeed Wio Link                 	esp8266:esp8266:wiolink
SparkFun ESP32 Thing           	esp32:esp32:esp32thing
SparkFun ESP8266 Thing         	esp8266:esp8266:thing
SparkFun ESP8266 Thing Dev     	esp8266:esp8266:thingdev
SweetPea ESP-210               	esp8266:esp8266:esp210
TTGO LoRa32-OLED V1            	esp32:esp32:ttgo-lora32-v1
ThaiEasyElec's ESPino          	esp8266:esp8266:espinotee
ThaiEasyElec's ESPino32        	esp32:esp32:espino32
WEMOS LOLIN32                  	esp32:esp32:lolin32
WeMos D1 R1                    	esp8266:esp8266:d1
WiFiduino                      	esp8266:esp8266:wifiduino
WiPy 3.0                       	esp32:esp32:wipy3
Widora AIR                     	esp32:esp32:widora-air
WifInfo                        	esp8266:esp8266:wifinfo
XinaBox CW01                   	esp8266:esp8266:cw01
XinaBox CW02                   	esp32:esp32:cw02
u-blox NINA-W10 series (ESP32) 	esp32:esp32:nina_w10
```

### Step 4: Find the Board's FQBN (Fully Qualified Board Name) - DONE
First step is to just connect ESP8266/NodeMCU board to your PCs
via a USB cable and run a command to sense the board:

```bash
# sense the board
$ arduino-cli board list
FQBN    Port        	ID       	Board Name
    	/dev/ttyUSB0	10c4:ea60	unknown
```

This wasn't much help since `arduino-cli` couldn't detect the board type.
We know the board is some type of NodeMCU with a ESP8266.
Let's list available cores matching 'nodemcu':

```bash
# list all boards matching 'nodemcu'
$ arduino-cli board listall nodemcu
Board Name                  	FQBN
NodeMCU 0.9 (ESP-12 Module) 	esp8266:esp8266:nodemcu
NodeMCU 1.0 (ESP-12E Module)	esp8266:esp8266:nodemcuv2
NodeMCU-32S                 	esp32:esp32:nodemcu-32s
```

My board is the "NodeMCU 1.0 (ESP-12E Module)", and so,
we now have its coresponding Fully Qualified Board Name (FQBN)
of `esp8266:esp8266:nodemcuv2`.
This gives us the needed information to complile the sketch.

### Step 5: Load Needed Libraries - DONE
You can do this via **Tools** > **Manage Libraries...** > enter "arduino-timer" and install.
Or you can search for and install the library via a`arduino-cli`.

```bash
# help message for arduino-cli lib
$ arduino-cli lib help
Arduino commands about libraries.

Usage:
  arduino-cli lib [command]

Examples:
  arduino-cli lib install AudioZero
  arduino-cli lib update-index

Available Commands:
  download     Downloads one or more libraries without installing them.
  install      Installs one of more specified libraries into the system.
  list         Shows a list of all installed libraries.
  search       Searchs for one or more libraries data.
  uninstall    Uninstalls one or more libraries.
  update-index Updates the libraries index.
  upgrade      Upgrades installed libraries.

Flags:
  -h, --help   help for lib

Global Flags:
      --config-file string   The custom config file (if not specified ./.cli-config.yml will be used).
      --debug                Enables debug output (super verbose, used to debug the CLI).
      --format string        The output format, can be [text|json]. (default "text")

Use "arduino-cli lib [command] --help" for more information about a command.
```

```bash
# search for a library with 'arduino-timer' in its name
$ arduino-cli lib search arduino-timer
Name: "arduino-timer"
  Author:  Michael Contreras
  Maintainer:  Michael Contreras
  Sentence:  Timer library for delaying function calls
  Paragraph:  Simple non-blocking timer library for calling functions in / at / every specified units of time. Supports millis, micros, time rollover, and compile time configurable number of tasks.
  Website:  https://github.com/contrem/arduino-timer
  Category:  Timing
  Architecture:  *
  Types:  Contributed
  Versions:  [0.0.1, 1.0.0]
Name: "arduino-timer-api"
  Author:  sadr0b0t
  Maintainer:  sadr0b0t
  Sentence:  Simple cross-platform API for multitasking on timer interrupt handlers
  Paragraph:  Simple cross-platform API for multitasking on Arduino based on timer interrupt handlers. Works with AVR/Arduino, PIC32/ChipKIT platforms.
  Website:  https://github.com/sadr0b0t/arduino-timer-api
  Category:  Timing
  Architecture:  *
  Types:  Contributed
  Versions:  [0.1.0]

# install the desired library
$ arduino-cli lib install "arduino-timer"
arduino-timer@1.0.0 downloaded
Installed arduino-timer@1.0.0
```

I'll use this non-trivial example sketch, [ntp-clock.ino][20],
which makes use of a ESP8266/NodeMCU + 7-segment LED display board.
Let's install the required libraries for the sketch `ntp-clock.ino`:

```bash
# install all your required libraries
arduino-cli lib install "arduino-timer"
arduino-cli lib install "Adafruit GFX Library"
arduino-cli lib install "Adafruit LED Backpack Library"
arduino-cli lib install "Time"
arduino-cli lib install "Timezone"
```

To remove all these libraries, just use the following command:
`arduino-cli lib uninstall Adafruit_GFX_Library Adafruit_LED_Backpack_Library arduino-timer Time Timezone`.

>**NOTE:** To my great disapointment, and implied by the discussion above concerning
>`$HOME/src/go_code/bin/.cli-config.yml`, it appears that libraries are put in `$HOME/Arduino/libraries`.
>Need to check out if using the Arduino IDE to install libraries would put them
>with my sketches in `$HOME/src/arduino/libraries`.

### Step 6: Compiling the Sketch - DONE
The `arduino-cli` tool also supports compiling and loading of the sketch.
hear is more information on compiling:

```bash
# help message for arduino-cli compiler
$ arduino-cli help compile
Compiles Arduino sketches.

Usage:
  arduino-cli compile [flags]

Examples:
  arduino-cli compile -b arduino:avr:uno /home/user/Arduino/MySketch

Flags:
      --build-cache-path string    Builds of 'core.a' are saved into this path to be cached and reused.
      --build-path string          Path where to save compiled files. If omitted, a directory will be created in the default temporary path of your OS.
      --build-properties strings   List of custom build properties separated by commas. Or can be used multiple times for multiple properties.
  -b, --fqbn string                Fully Qualified Board Name, e.g.: arduino:avr:uno
  -h, --help                       help for compile
  -o, --output string              Filename of the compile output.
      --preprocess                 Print preprocessed code to stdout instead of compiling.
      --quiet                      Optional, supresses almost every output.
      --show-properties            Show all build properties used instead of compiling.
  -v, --verbose                    Optional, turns on verbose mode.
      --vid-pid string             When specified, VID/PID specific build properties are used, if boards supports them.
      --warnings string            Optional, can be "none", "default", "more" and "all". Defaults to "none". Used to tell gcc which warning level to use (-W flag). (default "none")

Global Flags:
      --config-file string   The custom config file (if not specified ./.cli-config.yml will be used).
      --debug                Enables debug output (super verbose, used to debug the CLI).
      --format string        The output format, can be [text|json]. (default "text")
```

To compile the sketch we have to run the compile command
with the proper board's FQBN (Fully Qualified Board Name) we just got in the previous command.
Now we are going to compile the [ntp-clock.ino][20] sketch.

```bash
# compile the sketch
arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 ~/src/ntp-clock
```

This will create two file,
`ntp-clock.esp8266.esp8266.nodemcuv2.bin` and `ntp-clock.esp8266.esp8266.nodemcuv2.elf`,
within `~/src/ntp-clock`.
The `.bin` file is the firmware that will be uploaded into the microcontroller.
The `.elf` file is a executable and linkable format (elf)
formated file used when object file linking is required.

### Step 7: Fix the `esptool` - DONE
I had to do a fix the [`esptool.py` tool][24] (ROM bootloader for ESP8266 & ESP32)
since I got the following error while doing an
upload to an ESP32 (ESP8266 appears to work fine):
"Error: fork/exec /home/jeff/.arduino15/packages/esp32/tools/esptool/2.3.1/esptool: no such file or directory".
The fix is the following:

```bash
# make ESP32 esptool executable
cd ~/.arduino15/packages/esp32/tools/esptool/2.3.1
cp esptool.py esptool
chmod a+x esptool
```

### Step 8: UpLoad the Sketch - DONE
We can now upload the sketch and see the board in operation.

```bash
# help information concerning upload
$ arduino-cli help upload
Upload Arduino sketches.

Usage:
  arduino-cli upload [flags]

Examples:
  arduino-cli upload /home/user/Arduino/MySketch

Flags:
  -b, --fqbn string    Fully Qualified Board Name, e.g.: arduino:avr:uno
  -h, --help           help for upload
  -i, --input string   Input file to be uploaded.
  -p, --port string    Upload port, e.g.: COM10 or /dev/ttyACM0
  -v, --verbose        Optional, turns on verbose mode.
  -t, --verify         Verify uploaded binary after the upload.

Global Flags:
      --config-file string   The custom config file (if not specified ./.cli-config.yml will be used).
      --debug                Enables debug output (super verbose, used to debug the CLI).
      --format string        The output format, can be [text|json]. (default "text")
```

As before, we need to provide the FQBN, but also specify the serial port used by the board:
We'll do this for the [ntp-clock.ino][20] sketch:

```bash
# upload the compile sketch
$ arduino-cli upload --fqbn esp8266:esp8266:nodemcuv2 --port /dev/ttyUSB0 ~/src/ntp-clock
No new serial port detected.
Uploading 267072 bytes from /home/jeff/src/ntp-clock/ntp-clock.esp8266.esp8266.nodemcuv2.bin to flash at 0x00000000
................................................................................ [ 30% ]
................................................................................ [ 61% ]
................................................................................ [ 91% ]
.....................                                                            [ 100% ]
```

----

## Using Makefile
You'll find Makefile template for the [Arduino board family][26],
the [Atmel (now Microchip) AVR family][25],
[Espressif Systems ESP family][17], and others.
The harsh reality is that there isn't single Makefile template for all
the boards & processors leveraging the Arduino IDE will not fit a simple template.
Some have tried, at least in part, and the results a completely unreadable and hard to manage Makefille.

I'm going another route.
I want to create a simple to understand Makefile for
the specific board/processor families I'm interested in.
There are several "best practices" I'm going to attempt to leverage.
You'll find them here:

* [Practical Makefiles, by example](http://nuclear.mutantstargoat.com/articles/make/)
* [Make: Building AVR And Arduino Firmware On The Command Line](https://flyingcarsandstuff.com/2017/11/make-building-avr-and-arduino-firmware-on-the-command-line/)
* [Makefiles, Best Practices](https://danyspin97.org/blog/makefiles-best-practices/)

### Step X:
You'll find that the simple Makefile below
will support our example sketch [ntp-clock.ino][20]:

```bash
FQBN = esp8266:esp8266:nodemcuv2       # fully qualified board name (FQBN)
PORT = /dev/ttyUSB0                    # serial port used by the board


# string within names give to .bin and .elf files
VAR = $(shell echo $(FQBN) | tr ':' '.')

# compile and firmware flashing commands
CC = arduino-cli compile --fqbn $(FQBN)
UPLOAD = arduino-cli upload --fqbn $(FQBN) --port $(PORT)


.PHONY: build upload clean

build:
	$(CC) $(CURDIR)

upload:
	$(UPLOAD) $(CURDIR)

clean:
	rm --force ntp-clock.$(VAR).bin ntp-clock.$(VAR).elf
```


[01]:http://playground.arduino.cc/Learning/Linux
[02]:http://www.mjoldfield.com/atelier/2009/02/arduino-cli.html
[03]:https://www.arduino.cc/en/main/software
[04]:http://playground.arduino.cc/Interfacing/LinuxTTY
[05]:http://www.rackaid.com/resources/linux-screen-tutorial-and-how-to/
[06]:http://www.gnu.org/software/screen/manual/screen.html#Overview
[07]:http://linux.die.net/man/1/cu
[08]:http://www.computerhope.com/unix/screen.htm
[09]:https://www.arduino.cc/en/Guide/Linux
[10]:https://www.intorobotics.com/alternatives-standard-arduino-ide-one-choose/
[11]:https://learn.sparkfun.com/tutorials/what-is-an-arduino
[12]:https://www.arduino.cc/en/Main/Software
[13]:https://blog.arduino.cc/2018/08/24/announcing-the-arduino-command-line-interface-cli/
[14]:https://github.com/arduino/arduino-cli
[15]:https://www.gnu.org/software/make/
[16]:https://playground.arduino.cc/Learning/CommandLine
[17]:https://github.com/thunderace/Esp8266-Arduino-Makefile
[18]:https://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use/arduino-libraries
[19]:https://www.arduino.cc/en/hacking/preferences
[20]:https://gist.github.com/jeffskinnerbox/d74f518157c28dc2dac3297095417447
[21]:https://www.youtube.com/watch?v=TTiQR_4edXw
[22]:https://platformio.org/
[23]:https://www.arduino.cc/en/Guide/Linux
[24]:https://pypi.org/project/esptool/
[25]:https://flyingcarsandstuff.com/2017/11/make-building-avr-and-arduino-firmware-on-the-command-line/
[26]:https://hackaday.com/2015/10/01/arduino-development-theres-a-makefile-for-that/
[36]:https://frightanic.com/iot/interview-ivan-grokhotkov-espressif/
[37]:https://github.com/esp8266/Arduino
[38]:https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/installing-the-esp8266-arduino-addon
[39]:https://arduino-esp8266.readthedocs.io/en/2.4.2/#
[40]:https://github.com/espressif/arduino-esp32
[41]:
[42]:
[43]:
[44]:
[45]:
[46]:
[47]:
[48]:
[49]:
[50]:
