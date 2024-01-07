
<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="https://raw.githubusercontent.com/jeffskinnerbox/blog/main/content/images/banners-bkgrds/work-in-progress.jpg" title="These materials require additional work and are not ready for general use." align="center" width=420px height=219px>
</div>


-----


# More Walkie Talkie Examples
* [ESP32 compact board integrates Walkie-Talkie module](https://linuxgizmos.com/esp32-compact-board-integrates-walkie-talkie-module/)
* [ESP32 | Walkie-Talkie, Half-duplex communication based on WebSocket](https://www.youtube.com/watch?v=kw30vLdrGE8&list=PLnq7JUnBumAyTTTpzvRdsLKR8ls3sAtHW&index=11)
* [ESP32 | Walkie-Talkie based on Node.js Server for multi-clients (ft. PCM speaker)](https://www.youtube.com/watch?v=vq7mPgecGKA&list=PLnq7JUnBumAyTTTpzvRdsLKR8ls3sAtHW&index=13)


# ESP32 Walkie-Talkie
I have been thinking about creating a child's walkie-talkie that will not only
work around the house but also between homes.
I'm considering using the ESP32-S2 or ESP32-S3 as my microcontroller for the walkie-talkie device.
Here are some specifications for the ESP32 S2 & S3:

* [ESP32-S2][17] only has a single 32Bit Xtensa core with a clock speed of 240MHz,
reduced RAM and ROM, and WiFi only (no Bluetooth) compared to the traditional ESP32.
These tradeoffs mean the S2 draws significantly less power consumption,
especially while active.
* The 32Bit Dual Core 240MHz [ESP32-S3][18] is similar to the original and,
but unlike the S2, has the extra core and supports the latest Bluetooth 5 (Classic, BLE, and Mesh).
In addition, it has instructions specific to neural networks and signal processing
which could prove useful in an audio application.
Other key differences within the ESP32-S3 is its larger capacity,
higher-speed SPI flash and PSRAM chips,
a few more I/O pins, more security features,
and support for Bluetooth LE 5.0 connectivity with long-range and 2Mbps transfer support.
But it lacks an Ethernet MAC.

As to a board supplier for the ESP32-S2/S3, I like [Unexpected Maker][19].
The boards are small but exposes many pins,
supports [STEMMA QT][20] and [Inter-IC Sound Bus (I2S)][21] which will be very handly for digital audio,
and battery charging & monitoring.


I did some web searching and found some existing projects that came close,
or in some ways, exactly what I was looking for.
Specifically, the projects that inspired me where:

* To create a very simple walkie-talkie,
[GreatScott][02] using an [Arduino for control/audio sampling and NRF24L0+ for transceiver][03].
This walkie-talkie had poor sound quality but fixes were proposed by [Andreas Spiess][05] in his [video][06].
* It appears [Andreas Spiess][05] was inspired to create his own walkie-talkie but using a
[Raspberry Pi Zero W and the Mumble VIP platform][07].
* The [ESP32 based walkie-talkie][01], created by [atomic14][04],
can broadcast UDP packetized voice over a WiFi network, and  for free standing networking,
it uses the ESP-NOW protocol to broadcast to nearby walkie-talkies.
As a bonus, atomic14 has a series of video on [ESP32 audio][08] that could be very helpful.

Armed with these projects,
I decide to pick the best design approaches from each of them to create my walkie-talkie.
I really like the idea of using [Mumble][09] since it enables broadcasting across the Internet.
I also want to use the ESP32 hardware so I could use [ESP-NOW][10] when we don't have a WiFi,
and I could use broadcast UDP when I have WiFi.
Observing the earlier projects referenced above,
the only component missing is a [Mumble Client][11] that will work on a ESP32.
It might also be wish to provide a [small OLED display][12] to show status
since the design is getting more complex.

Give the projects referenced above,
it appear that my design objective is feasible, if not already accomplished.
To give myself some additional challages & learning opertunities,
I'm choosing to use [CircuitPython][13] as my programming language
(all the examples above are in C++).
I have done some programming in [Python][14] but I want to learn about the langage and its tool base.
In addition, I plan to try out some of the LLM AI assistant's to help with
my experimenting and coding of this project.
I'll start with [ForeFront AI assistant][15],
which has a generious free pricing plan is user-friendly,
and plenty of good features for all kinds of tasks like writing, coding,
and a [broad set of assistants][16].



[01]:https://hackaday.com/2021/04/07/an-esp32-walkie-talkie-for-those-spy-radio-moments/
[02]:https://www.youtube.com/c/greatscottlab
[03]:https://www.youtube.com/watch?v=SZYwvvh6m-s
[04]:https://www.youtube.com/c/atomic14
[05]:https://www.youtube.com/c/AndreasSpiess
[06]:https://www.youtube.com/watch?v=YJ25eQRbhaQ&t=0s
[07]:https://www.youtube.com/watch?v=b9qIMWn8uyY&t=95s
[08]:https://www.youtube.com/playlist?list=PL5vDt515
[09]:https://www.mumble.info/
[10]:https://www.espressif.com/en/products/software/esp-now/overview
[11]:https://wiki.mumble.info/wiki/3rd_Party_Applications#Clients
[12]:https://www.amazon.com/HiLetgo-Serial-128X64-Display-Color/dp/B06XRBTBTB?th=1
[13]:https://circuitpython.org/
[14]:https://www.python.org/
[15]:https://www.forefront.ai/
[16]:https://www.forefront.ai/assistants
[17]:https://www.youtube.com/watch?v=L6IoSVdKwNM
[18]:https://www.youtube.com/watch?v=kQnkE04I2mQ
[19]:https://esp32s3.com/
[20]:https://learn.adafruit.com/introducing-adafruit-stemma-qt/what-is-stemma-qt
[21]:https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2s.html
[22]:
[23]:
[24]:
[25]:
[26]:
[27]:
[28]:
[29]:
[30]:

