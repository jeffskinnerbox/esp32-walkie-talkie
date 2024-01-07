
"""
    CircuitPython MP3 File Decoded to Analog Audio Output.

    Description:
    This script plays the first mp3, wait for a button to be pressed,
    and then play another mp3, continues to the last and then repeats from the beginning.

    `mp3` files are compressed audio, smaller than the alternative, uncompressed `wav` files.
    Not all boards support the `MP32Decoder`, and in those case you must handle
    the bigger `wav` files. CircuitPython supports any MP3 file you like, mono & stereo
    files from 32kbit/s to 128kbit/s work, with sample rates from 16kHz to 44.1kHz.

    Wiring:
    KB2040: audio speaker positive lead on A0 pin, momentary switch on positive lead on A1 pin

    Usage:
    cp mp3-audio-out.py <path-to-device>/CIRCUITPY/code.py

    Tested:
    Adafruit KB2040

    Source:
    https://learn.adafruit.com/circuitpython-essentials/circuitpython-mp3-audio
"""

import board
import digitalio
from audiomp3 import MP3Decoder
try:
    from audioio import AudioOut
except ImportError:
    try:
        from audiopwmio import PWMAudioOut as AudioOut
    except ImportError:
        pass  # not always supported by every board!

button = digitalio.DigitalInOut(board.A1)           # for for Adafruit KB2040, board.A1 == GPIO27
button.switch_to_input(pull = digitalio.Pull.UP)    # make button.value = true

# The listed mp3 files will be played in order
mp3files = ["slow.mp3", "happy.mp3"]

mp3 = open(mp3files[0], "rb")                        # you have to specify some mp3 file when creating the decoder
decoder = MP3Decoder(mp3)
audio = AudioOut(board.A0)                          # for Adafruit KB2040, board.A0 == GPIO26

while True:
    for filename in mp3files:
        decoder.file = open(filename, "rb")         # updating the .file property of the existing decoder helps avoid running out of memory (MemoryError exception)
        audio.play(decoder)
        print("playing", filename)

        # This allows you to do other things while the audio plays!
        while audio.playing:
            pass

        print("Waiting for button press to continue!")
        while button.value:
            pass

