
"""
    CircuitPython Pulse-Width Modulation (PWM) File Decoded to Analog Audio Output.

    Description:
    While button is pressed, you will hear a repeat one second of a tone,
    and one second of silence.

    A tone_volume of 1.0 can be heard on a speaker without amplification.  Larger values will exceed 2 byte limit
    The tone generated isn't pure but the reactance of the speaker helps filter out harmonics

    Wiring:
    Adafruit KB2040: momentary button on A1, audio speaker positive lead on A0 pin

    Usage:
    cp pwm-audio-out.py <path-to-device>/CIRCUITPY/code.py

    Tested:
    Adafruit KB2040

    Source:
    https://learn.adafruit.com/circuitpython-essentials/circuitpython-audio-out
    https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp/circuitpython-wiring-test
"""


import time
import array
import math
import board
import digitalio
from audiocore import RawSample

try:
    from audioio import AudioOut
except ImportError:
    try:
        from audiopwmio import PWMAudioOut as AudioOut
    except ImportError:
        pass  # not always supported by every board!

# button used to start/stop playing tone
button = digitalio.DigitalInOut(board.A1)             # board.BUTTON = board.D11 = GPIO11
button.switch_to_input(pull = digitalio.Pull.UP)

# specify the parameteres that define the tone to be played
tone_volume = 1.0                                                              # value of 0 to 1.0 to increase the volume of the tone
tone_frequency = 440                                                           # set this to the Hz of the tone you want to generate
sampling_rate = 8000                                                           # sampling rate, in samples-per-second, you will use on the tone, 32 bit value that dictates how quickly samples are played in Hertz (cycles per second)
qlevels = 2**15                                                                # 15 bit quantization levels between the lowest possible sample value and the highest
length = sampling_rate // tone_frequency                                       # // means divide to get integer result, discard remainder

# generate one period of a sine wave (very crud waveform)
sine_wave = array.array("H", [0] * length)                                     # array of unsigned short integers, initialize with zeros
for i in range(length):
    sine_value = math.sin(math.pi * 2 * i / length)
    sine_wave[i] = int((1 + sine_value) * tone_volume * (qlevels - 1))         # dc offset of 1 + sine wave of frequency 440, multiplied by tone_volume * max quantization

audio = AudioOut(board.A0)                                                     # create a AudioOut object, allowing you to play audio signals
sine_wave_sample = RawSample(sine_wave)                                        # create a raw audio sample buffer in memory using the wave_form

# while button is pressed, repeat one second of a tone, and one second of silence
while True:
    if not button.value:
        print("playing tone ...")
        audio.play(sine_wave_sample, loop = True)                              # loop over the one period sine wave to create continuous tone
        time.sleep(1)
        audio.stop()
        time.sleep(1)

