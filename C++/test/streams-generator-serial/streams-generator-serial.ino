
/* -----------------------------------------------------------------------------

Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1

DESCRIPTION
    Test of basic library functionality, generated sine wave, convert to CSV format, send to the serial output

PHYSICAL DESIGN
    Hardware
        DOIT ESP32 DEVKIT DOIT V2, or KeeYees ESP32S Dev Board, or HiLetgo ESP-WROOM-32 - 38 pins

    Wiring
        Just power & serial connect via USB, no other devices required.

MONITOR
    No commands required. Just provide power with USB cable, and monitor activity via
    connection to the USB port (aka serial) with a Linux terminal by executing the following:

        screen /dev/ttyUSB0 115200,cs8cls

    To terminate monitoring, enter: CNTR-a :quit
    To stop the screen scrolling, enter: CNTR-a :

    To find the device on your LAN:

        sudo arp-scan 192.168.1.200/24 | grep Espressif

SOURCES
    GitHub: pschatzmann/arduino-audio-tools - https://github.com/pschatzmann/arduino-audio-tools/tree/main/examples/examples-stream/streams-generator-serial

DOCUMENTATION
    Audio Tools Wiki - https://github.com/pschatzmann/arduino-audio-tools/wiki/Introduction
    #419 ESP32 Audio Tutorial with lots of examples - https://www.youtube.com/watch?v=a936wNgtcRA

CREATED BY
    Oringinal Author: Phil Schatzmann
    Modification By: jeffskinnerbox@yahoo.com
    Copyright: GPLv3

----------------------------------------------------------------------------- */

#include "AudioTools.h"


AudioInfo audio_info(44100, 2, 16);
SineWaveGenerator<int16_t> sineWave(32000);                // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);             // Stream generated from sine wave
CsvOutput<int16_t> out(Serial);
StreamCopy copier(out, sound);                             // copies sound to out

// Arduino Setup
void setup(void) {
  // Open Serial
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  // Define CSV Output
  out.begin(audio_info);

  // Setup sine wave
  sineWave.begin(audio_info, N_B4);
  Serial.println("started...");
}

// Arduino loop - copy sound to out
void loop() {
  copier.copy();
}

