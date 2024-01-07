
<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="https://raw.githubusercontent.com/jeffskinnerbox/blog/main/content/images/banners-bkgrds/work-in-progress.jpg" title="These materials require additional work and are not ready for general use." align="center" width=420px height=219px>
</div>


-----


In CircuitPython, there are three different techniques to output audio:
* DAC using `audioio`
* PWM using `audiopwmio` - requires an external low-pass filter (a RC filter or just the limit frequency response of your seaker)
* I2S using `audiobusio` - requires external I2S decoder hardware

* PDM using ??? -
    * [ESP32-S3 No DAC?](https://atomic14.substack.com/p/esp32-s3-no-dac)
    * [We don't need a DAC - ESP32 PDM Audio](https://www.youtube.com/watch?v=oZ39VCUvKjw)
    * [atomic14/esp32-pdm-audio](https://github.com/atomic14/esp32-pdm-audio)

# What is WAV vs FLAC vs AIFF vs MP3 vs AAC vs WMA?
**Lossless Formats** - Audio is encoded with the objective of not making any changes to the fidelity of the sound
* [Waveform Audio File Format (WAV)][14] is a Microsoft & IBM creation to replace the simple (and unpleasant) beeping noises used in early computers to notify the user.
WAV files aren’t compressed when digitally encoded, meaning the original audio elements stay in the file.
Audio editors describe WAV files as “lossless” because you don’t lose any part of your audio.
Advantages: high quality sound, easy editing, create quality recordings with simple/inexpensive technology.
Diadvantages: large file size, incompatibility with some devices
* [Free Lossless Audio Codec (FLAC)][15] files are a good option if you're looking to store a large number of audio files. This file type is compressed but is still a lossless file type, as little quality is lost due to audio track compression. This file type gives you similar sound quality to a WAV file while taking up half the storage space.
* [Audio Interchange File Formats (AIFF)][16], developed by Apple for the same reason that Microsoft & IBM created WAV files. AIFF files use the exact same encoding method that WAV files use, and as a result, this file type is very similar in quality and file size when compared to WAV.

**Lossy Formats** - Audio is encoded with compression algorthms to reduce size
* [MPEG Audio Layer-3 (MP3)][17]
are compressed to make their file size more manageable. As a result, they lose some audio quality and are considered a "lossy" audio file type.
When encoded, some parts of the MP3 audio are stripped from the recording to compress the file (to make it smaller). This process of altering the audio signal, known as perceptual coding or psychoacoustic modeling, results in the loss of some sound considered beyond the hearing capabilities of most people.
Advantages: smaller file size, widely supported on devices,
Disadvantages: loss of some sound quality (minor), audio distortion (aka "compression artifacts")
* [Advanced Audio Coding (AAC)][18] file type was designed as an improvement of the MP3 file type. Like MP3s, AAC audio files are much smaller than WAV or other lossless, but AAC files provide better sound quality than MP3 files. AAC files are not as widely used as MP3s, and therefore, some of the best podcast software and hosting services do not support AACs.
* [Windows Media Audio (WMA)][19] differs little from WMA and MP3 files in terms of file size or quality. Like with AAC files, using WMA to record and distribute your podcast could cause hang-ups due to device and platform compatibility issues.

Sources:
* [WAV vs MP3: What's the Difference & Which Is Better for Podcasters](https://riverside.fm/blog/wav-vs-mp3)

# What is PWM vs PDM vs SDM?
Whats the difference between the PWM / PDM / SDM modulation schemes used for audio applications?

* A [Pulse-Width Modulation (PWM)][10] circuit is often used as a simple digital to analog converter
to produce analog waveforms that require only relatively low frequencies, typically less than 100KHz.
Many digital devices like microprocessors and FPGAs use pulse-width modulation to create
low-bandwidth analog signals because they require few resources (just a single output pin and a simple passive RC filter).
* A [Pulse Density Modulation (PDM)][11] circuit is a variation on the PWM that works in a similar fashion.
Like a PWM circuit, a PDM circuit drives a digital pulse train on a single digital pin,
and a low-pass filter integrates the digital signal to produce an analog signal.
But in a PDM circuit, the pulse widths are constant, but the period between the pulses change.
* A [Delta-Sigma Modulation (DSM)][12] circuit achieve higher transmission efficiency
by transmitting only the changes (delta) in value between consecutive samples,
rather than the actual samples themselves.
Oversampling reduces the effect of noise within the signal bandwidth of interest,
benefiting the delta-sigma ADC’s analog operation.
ADCs and DACs both can use delta-sigma modulation.
DSM typically has more quantization noise, and therefore, generally not used in audio applications.
* All [Class D Audio Amplifiers][13] decode information about the audio signal from a stream of pulses.
Generally, the pulse widths are linked to the amplitude of the audio signal,
and the spectrum of the pulses includes the desired audio signal plus undesired
(but unavoidable) high-frequency content.
Class D amplifiers output a switching waveform,
at a frequency far higher than the highest audio signal that needs to be reproduced.
The most common modulation technique used to drive a class D audio amplifier is PWM.

Sources:
* [Pulse Width Modulation][10]

# Converting Audio Files
* [Microcontroller Compatible Audio File Conversion](https://learn.adafruit.com/microcontroller-compatible-audio-file-conversion)



[10]:https://www.realdigital.org/doc/333049590c67cb553fc7f9880b2f79c3
[11]:https://www.realdigital.org/doc/333049590c67cb553fc7f9880b2f79c3#pulse-density-modulation
[12]:https://www.electronicdesign.com/technologies/analog/article/21798185/understanding-delta-sigma-modulators
[13]:https://www.eetimes.com/class-d-audio-amplifiers-what-why-and-how-part-5/
[14]:https://docs.fileformat.com/audio/wav/
[15]:https://docs.fileformat.com/audio/flac/
[16]:https://docs.fileformat.com/audio/aiff/
[17]:https://docs.fileformat.com/audio/mp3/
[18]:https://docs.fileformat.com/audio/aac/
[19]:https://docs.fileformat.com/audio/wma/
[20]:

