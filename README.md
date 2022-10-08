<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


----


# ESP32 Walkie-Talkie
I have been thinking about creating a child's walkie-talkie that will work around the house or between homes.
I did some web searching and found some existing projects that came close,
or in some ways, exactly what I was looking for.
Specifical the pojects that inspired me where:

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
the only component missing is a [Mumble Client][11] that will work on a ESP32,
which I found [here][???].
It might also be wish to provide a [small OLED display][12] to show status
since the design is getting more complex.



[01]:https://hackaday.com/2021/04/07/an-esp32-walkie-talkie-for-those-spy-radio-moments/
[02]:https://www.youtube.com/c/greatscottlab
[03]:https://www.youtube.com/watch?v=SZYwvvh6m-s
[04]:https://www.youtube.com/c/atomic14
[05]:https://www.youtube.com/c/AndreasSpiess
[06]:https://www.youtube.com/watch?v=YJ25eQRbhaQ&t=0s
[07]:https://www.youtube.com/watch?v=b9qIMWn8uyY&t=95s
[08]:https://www.youtube.com/playlist?list=PL5vDt5AALlRfGVUv2x7riDMIOX34udtKD
[09]:https://www.mumble.info/
[10]:https://www.espressif.com/en/products/software/esp-now/overview
[11]:https://wiki.mumble.info/wiki/3rd_Party_Applications#Clients
[12]:https://www.amazon.com/HiLetgo-Serial-128X64-Display-Color/dp/B06XRBTBTB?th=1

