
#-------------------------------------------------------------------------------
#
# Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
# Version:      0.0.0
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
#   The Makefile was created with the help of this website and others:
#   https://learn.sparkfun.com/tutorials/efficient-arduino-programming-with-arduino-cli-and-visual-studio-code/all
#
# DOCUMENTATION
#   https://create.arduino.cc/projecthub/B45i/getting-started-with-arduino-cli-7652a5
#
#-------------------------------------------------------------------------------

# Use Bash shell instead of the default /bin/sh
SHELL := /bin/bash

# name of program being created
#PROG = esp32-walkie-talkie
PROG = main

# type of package, architecture, and board in use
#PACKAGE = esp8266
#ARCH =    esp8266
#BOARD =   nodemcuv2
PACKAGE = esp32
ARCH =    esp32
BOARD =   esp32

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
UPLOAD_USB_FLAGS = $(VERBOSE) --fqbn $(FQBN) --port $(PORT)

#--------------------------------- ota upload ----------------------------------

# location of the espota.py used for ota flashing
#ESPOTATOOL = /home/jeff/.arduino15/packages/esp8266/hardware/esp8266/2.5.2/tools/espota.py
ESPOTATOOL = /home/jeff/.arduino15/packages/esp32/hardware/esp32/1.0.2/tools/espota.py

# set ota password, ip address, and port for device
#OTAHOSTNAME := $(shell grep OTAHOSTNAME secrets.h | cut -d" " -f3 | awk '{print substr($$0, 2, length($$0) - 2)}')
#OTAPASS := $(shell grep OTAPASSWORD secrets.h | cut -d" " -f3 | awk '{print substr($$0, 2, length($$0) - 2)}')
#OTAPORT := $(shell grep OTAPORT secrets.h | cut -d" " -f3)
#OTAIP := $(shell ping -c1 $(OTAHOSTNAME).local | grep "bytes from" | cut -d " " -f5 | awk '{ print substr($$0, 2) }' | awk '{ print substr( $$0, 1, length($$0)-2 ) }')

# ota firmware flasher and flags
#UPLOAD_OTA = python2 $(ESPOTATOOL)
#UPLOAD_OTA_FLAGS = -d -i $(OTAIP) -p $(OTAPORT) -a $(OTAPASS) -f $(PROG).$(VAR).bin

#-------------------------------------------------------------------------------


# explicitly declare these target to be phony
.PHONY: help build upload upload-ota clean erase size

all: build upload                               # build and then upload via usb

all-ota: build upload-ota                       # build and then upload via ota

help:
	@echo ' '
	@echo 'Makefile for scroll-display'
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
	$(UPLOAD_USB) $(UPLOAD_USB_FLAGS) $(CURDIR)

#upload-ota:                                     # flash the binary executable via ota
#	@echo VAR = $(VAR)
#	@echo OTAIP = $(OTAIP)
#	@echo OTAPASS = $(OTAPASS)
#	@echo OTAPORT = $(OTAPORT)
#	@echo OTAHOSTNAME = $(OTAHOSTNAME)
#	@echo UPLOAD_OTA_FLAGS = $(UPLOAD_OTA_FLAGS)
#	$(CURDIR)/answerbot $(OTAIP) 23             # using telnet, reboot the device to do OTA
#	sleep 10                                    # wait until device is ready for OTA start
#	$(UPLOAD_OTA) $(UPLOAD_OTA_FLAGS)

erase:                                          # erase the entire flash
	$(ESPTOOL) erase_flash --port $(PORT)

size:                                           # determine the flash size
	$(ESPTOOL) flash_id --port $(PORT)

clean:                                          # delete all binaries and object files
	rm -r --force $(BUILD)
	rm --force *.bin *.elf *.hex

