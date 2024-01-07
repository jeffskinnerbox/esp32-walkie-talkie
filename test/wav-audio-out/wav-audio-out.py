
"""
    CircuitPython WAV File Decoded to Analog Audio Output.

    Description:
    This script plays the first mp3, wait for a button to be pressed,
    and then play another mp3, continues to the last and then repeats from the beginning.

    Use 'ampy --port /dev/ttyACM0 run pin-mapping.py' to find the I2S pins required.

    Wiring:
    KB2040: audio speaker positive lead on A0 pin, momentary switch on positive lead on D12 pin

    Usage:
    cp mp3-audio-out.py <path-to-device>/CIRCUITPY/code.py

    Tested:
    Adafruit KB2040

    Source:
    https://docs.circuitpython.org/en/latest/shared-bindings/audiobusio/index.html
"""

import board
import audiocore
import audiobusio
import digitalio

button = digitalio.DigitalInOut(board.A1)                                      # for for Adafruit KB2040, board.A1 == GPIO27
button.switch_to_input(pull = digitalio.Pull.UP)                               # make button.value = true

# The listed wav files will be played in order
wavfiles = ["pyle.wav", "scumbag.wav", "numnuts.wav"]

# creating the decoder
audio = audiobusio.I2SOut(board.SCK, board.SCL, board.SDA)                     # for Adafruit KB2040, continuous serial clock line = board.CLK board.SCK (GPIO18)  /  word select line (aka WS) = board.D13 board.SCL (GPIO13)  /  serial data line (aka SD) = board.D12 board.SDA (GPIO12)

while True:
    for filename in wavfiles:
        data = open(filename, "rb")                                            # updating the .file property of the existing decoder helps avoid running out of memory (MemoryError exception)
        wav = audiocore.WaveFile(data)
        audio.play(wav)
        print("Playing wav file", filename)

        # This allows you to do other things while the audio plays!
        while audio.playing:
            pass

        print("Waiting for button press to continue!")
        while button.value:
            pass

