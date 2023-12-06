
#-------------------------------------------------------------------------------
#
# Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
# Version:      0.9.6
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
#
# 	python /home/jeff/.arduino15/packages/esp32/hardware/esp32/2.0.5/tools/espota.py -d -i 192.168.1.109 -p 3232 -a 123 -f /tmp/hello-world-4/build/hello-world-4.ino.bin
#
# PINOUT
#   nodemcuv2
#   esp32doit-devkit-v1
#   nodemcu-32s
#   tinys2
#   lolin32
#   ttgo-lolin32
#   heltec_wifi_kit_32
#   dfrobot_firebeetle2_esp32s3
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
SHELL = /bin/bash

# name of program being created (aka hostname)
HOSTNAME = esp32-walkie-talkie

# type of package, architecture, and board in use
#BOARD =   nodemcuv2              # ESP8266 ESP-12E NodeMCU, 30 pins
#PACKAGE = esp8266
#ARCH =    esp8266
#BOARD =   esp32doit-devkit-v1    # DOIT ESP32 DEVKIT DOIT V1, MELIFE ESP-32S, 30 pins
#PACKAGE = esp32
#ARCH =    esp32
BOARD =   nodemcu-32s            # DOIT ESP32 DEVKIT DOIT V2, KeeYees ESP32S Dev Board, HiLetgo ESP-WROOM-32, 38 pins
PACKAGE = esp32
ARCH =    esp32
#BOARD =   tinys2                 # TinyS2 ESP32-S2 Board - https://unexpectedmaker.com/tinys2
#PACKAGE = esp32
#ARCH =    esp32
#BOARD =   lolin32                # ESP32 WeMos Lolin32, ESP32 WROOM Lolin32, pins 26
#PACKAGE = esp32
#ARCH =    esp32
#BOARD =   ttgo-lolin32           # LilyGO TTGO T Display ESP32 - https://www.amazon.com/dp/B07X1W16QS
#PACKAGE = esp32
#ARCH =    esp32
#BOARD =   heltec_wifi_kit_32     # Heltec ESP32 0.96" OLED Display, MakerFocus ESP32 0.96" OLED Display, 36 pins
#PACKAGE = esp32
#ARCH =    esp32
#BOARD =   dfrobot_firebeetle2_esp32s3  # DFRobot FireBeetle Board ESP32-E, 32 pins
#PACKAGE = esp32
#ARCH =    esp32

# serial port used by the board
PORT = /dev/ttyUSB0
#PORT = /dev/ttyACM0

#------------------------- names, tools, flags, paths --------------------------

# optional verbose compile/upload flag
#VERBOSE = -v

# fully qualified board name (FQBN)
FQBN = $(PACKAGE):$(ARCH):$(BOARD)

# location of the esptool used for usb flashing
ESPTOOL = /home/jeff/.arduino15/packages/esp32/tools/esptool_py/4.2.1/esptool.py

# string within names given to .bin and .elf files
#VAR := $(shell echo $(FQBN) | tr ':' '.')

# path for temp-storage of binary, object, and core.a files
BUILD = /tmp/$(HOSTNAME)
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

# location of the espota.py tool used for ota flashing
#ESPOTATOOL = /home/jeff/.arduino15/packages/esp8266/hardware/esp8266/3.0.2/tools/espota.py
ESPOTATOOL = /home/jeff/.arduino15/packages/esp32/hardware/esp32/2.0.5/tools/espota.py

# set ota password, ip address, and port for device
#OTAPASS := $(shell grep OTAPASSWORD secrets.h | uniq | cut -d" " -f3 | awk '{print substr($$0, 2, length($$0) - 2)}')
#OTAPORT := $(shell grep -i $(PACKAGE) secrets.h | grep OTAPORT | cut -d" " -f3)
#HOSTPORT = 3232
#OTAIP := $(shell ping -c1 $(HOSTNAME).local | grep "bytes from" | cut -d " " -f5 | awk '{ print substr($$0, 2) }' | awk '{ print substr( $$0, 1, length($$0)-2 ) }')

# ota firmware flasher and its flags
UPLOAD_OTA = $(ESPOTATOOL)
UPLOAD_OTA_FLAGS = -d -r -i $(OTAIP) -p $(OTAPORT) -P $(HOSTPORT) -a $(OTAPASS) -f /tmp/$(HOSTNAME)/build/$(HOSTNAME).ino.bin

#-------------------------------------------------------------------------------


# explicitly declare these target to be phony
.PHONY: help build upload upload-ota clean erase size

all: build upload                               # build and then upload via usb

all-ota: build upload-ota                       # build and then upload via ota

help:
	@echo ' '
	@echo 'Makefile for' $(HOSTNAME)
	@echo ' '
	@echo 'Usage:'
	@echo '   make              create all files and flash device via usb'
	@echo '   make all          create all files and flash device via usb'
	@echo '   make all-ota      create all files and flash device via ota'
	@echo '   make build        compile the code and create the *.elf file'
	@echo '   make upload       create the *.bin file and flash device via usb'
	@echo '   make upload-ota   create the *.bin file and flash device via ota'
	@echo '   make chip-id      get the chip ID from the device and exit'
	@echo '   make erase        erease the entire flash from the device and exit'
	@echo '   make size         print the flash size within the device and exit'
	@echo '   make clean        delete *.bin, *.elf, *.hex files'
	@echo '   make help         print this help message and exit'
	@echo ' '
	@echo "When you see 'Sending invitation to...', hit the reset button to begin OTA upload"
	@echo ' '
	@echo 'Pass the option "--debug" to enable trace messaging (e.g. make --debug build)'
	@echo ' '

build:                                          # build the binary executable
	$(CC) $(CC_FLAGS) $(CURDIR)

upload:                                         # flash the binary executable via usb (aka via serial port)
	$(UPLOAD_USB) $(UPLOAD_USB_FLAGS) $(CURDIR)

upload-ota:                                     # flash the binary executable via ota (aka via wifi)
	@echo "When you see 'Sending invitation to...', hit the reset button to begin OTA upload"
	$(UPLOAD_OTA) $(UPLOAD_OTA_FLAGS)

chip-id:                                        # get the chip id
	$(ESPTOOL) chip_id

erase:                                          # erase the entire flash
	$(ESPTOOL) erase_flash

size:                                           # determine the flash size
	$(ESPTOOL) flash_id

clean:                                          # delete all binaries and object files
	rm -r --force $(BUILD) *.bin *.elf *.hex

