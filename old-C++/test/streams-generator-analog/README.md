<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


----


# I2S Analog Output Test
Inter Integrated Circuit Sound Protocol, or Inter-IC Sound, or [I2S][30] (pronounced "eye-squared-ess"),
sometimes called "digital audio",is an oxymoron since audio is inherently analog signal.
This is a simple basic test for the ESP32 __analog output__ using I2S.

We just send a generated sine wave and expect to hear a clean signal.
Please note the log level should be set so that there is no disturbing output!

## Output Device: Piezo Electric Element
To test the output I am using a [piezo electric element](https://en.wikipedia.org/wiki/Piezoelectricity)
like a [20mm Piezo Elements Sounder Sensor Trigger Drum Disc](https://www.amazon.com/gp/product/B07FJXY4W8).

You can also use some simple earphones with an [audio jack](https://pschatzmann.github.io/Resources/img/earphones.jpg).
Specifically, I used a breakout board with a [TRRS 3.5mm Audio Jack](https://www.amazon.com/gp/product/B07L3P93ZD)
to it makes easy to us an audio-style connector that you see on [phone earbuds](https://www.amazon.com/Certified-Earphones-Microphone-Headphones-Compatible/dp/B0BZ77BT73/).
TRRS stands for “Tip, Ring, Ring, Sleeve” which reflects the fact that,
unlike a standard stereo connector, this actually has three conductors and a ground.
Some devices use the extra conductor for a microphone (like hands-free headsets)
or to carry a video signal (like in some MP3/MP4 players).

## Wiring & Pins
On the ESP32 the output is on the Pins GPIO26 and GPIO27

| PIEZO |      ESP32      |
|-------|-----------------|
|   +   | GPIO25 / GPIO26 |
|   -   | GND             |

| TRRS Audio Jack |      ESP32      |
|-----------------|-----------------|
|      Tip
|      Ring1
|      Ring2
|      Sleeve
|   +   | GPIO25 / GPIO26 |
|   -   | GND             |

## Testing
```bash
# use screen to view the output
screen /dev/ttyUSB0 115200,cs8cls
```

[30]:https://en.wikipedia.org/wiki/I%C2%B2S

