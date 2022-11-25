<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----


This set of test programs demonstrates how to use the ESP32 built-in
Analog to Digital Converter (ADC) and Inter-IC Sound (I2S - pronounced "eye-squared-ess")
for capturing audio data and for audio output.
There are four test programs in this directory:

* `loop_sampling` - This program shows how to use the Arduino `analogRead`
function and the Espressif `adc1_get_raw` function.
It also demonstrates how to get a calibrated value back from the ADC
to give you the actual voltage at the input.
* `i2s_sampling` - This program handles both analogue devices
(such as the MAX4466 and the MAX9814) and I2S devices (such as the SPH0645 and INMP441).
It demonstrates how to use the I2S peripheral for high-speed sampling
using DMA to transfer samples directly to RAM.
We can read these samples from the internal ADC or from the I2S peripheral directly.
Samples are read from the DMA buffers and sent to a server running on your desktop machine.
* `i2s_output` - This program shows how to drive an I2S output device.
Its tested against the [MAX98357][02] breakout board from Adafruit.
You can play a WAV file (from a ESP32 SPIFFS file system)
or you can play a simple sin wave through the output.
* `server` - This is a node server that writes the samples received from the ESP32 to a file.

#### Step 1: Clone the ESP32 I2S Audio Repository
I'm going to clone atomic14's [ESP32 I2S Audio][01] repository
and use it as part of my testing.

```bash
# clone the repository
cd ~/src/embedded-devices/esp32-walkie-talkie/test
git clone https://github.com/atomic14/esp32_audio.git ./i2s-audio

# remove referance to the clones github and build methodology
cd i2s-audio
rm -r -f .git .github platformio*
```



[01]:https://github.com/atomic14/esp32_audio
[02]:https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp
[03]:
[04]:
[05]:
[06]:
[07]:
[08]:
[09]:
[10]:

