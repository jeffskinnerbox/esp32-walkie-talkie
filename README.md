<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


----


# More Walkie Talkie examples
* [ESP32 | Walkie-Talkie, Half-duplex communication based on WebSocket](https://www.youtube.com/watch?v=kw30vLdrGE8&list=PLnq7JUnBumAyTTTpzvRdsLKR8ls3sAtHW&index=11)
* [ESP32 | Walkie-Talkie based on Node.js Server for multi-clients (ft. PCM speaker)](https://www.youtube.com/watch?v=vq7mPgecGKA&list=PLnq7JUnBumAyTTTpzvRdsLKR8ls3sAtHW&index=13)


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



-----



# ESP32
Since the release of the original ESP32, a number of variants have been introduced and announced. They form the ESP32 family of microcontrollers. These chips have different CPUs and capabilities, but all share the same SDK and are largely code-compatible. Additionally, the original ESP32 was revised.

* [ESP32](https://en.wikipedia.org/wiki/ESP32)
* [ESP32 for IoT: A Complete Guide](https://www.nabto.com/guide-to-iot-esp-32/)
* [The Internet of Things with ESP32](http://esp32.net/)

## ESP32-S2
The orginal ESP32 is equipped with WiFi, Bluetooth, two fast & low power cores,
lots of exposed pins, etc
It did lack native USB support (like the SAMD processors), so this support came for aadditional chips.

Things changed with the ESP32-S2.
ESP32-S2 only has a single 32-bit Xtensa core, reduced RAM and ROM,
it natively suports USB, and supports WiFi only (no Bluetooth).
It also support always connected WiFi during light-sleep (could be a power saver from some applications).
These tradeoffs mean the ESP32-S2 draws significantly less power consumption, especially while active.

* [#339 The new ESP32-S2: How relevant is it for the Makers? First tests](https://www.youtube.com/watch?v=L6IoSVdKwNM)
* [EYE on NPI - Espressif ESP32-S2 SoC](https://www.youtube.com/watch?v=4F25y-P8krM)
* [ESP32-S2 Processor Datasheet Released, Development Boards Unveiled](https://www.cnx-software.com/2019/09/03/esp32-s2-processor-datasheet-released-development-boards-unveiled/)
* [FeatherS2 Board Brings ESP32-S2 to Adafruit Feather Form Factor](https://www.cnx-software.com/2020/10/05/feathers2-board-brings-esp32-s2-to-adafruit-feather-form-factor/)
* [ESP32-S2 board targets battery-powered applications with 30uA deep sleep power consumption](https://www.cnx-software.com/2020/10/28/esp32-s2-board-targets-battery-powered-applications-with-30ua-deep-sleep-power-consumption/)
* [Test of Battery operated ESP32 Boards (Olimex, TinyPICO, EzSBC, TTGO)](https://www.youtube.com/watch?v=ajt7vtgKNNM)
* [ESP32-S2 New Arduino USB Functionality: Cool Combination with WiFi](https://www.youtube.com/watch?v=XnqDEV21BSA)

### Node-MCU-32S
The AI-Thinker development boards contain the ESP-WROOM-32 module

Contains the ESP32 chip
**ESP32-D0WDQ6** is the	initial production release chip of the ESP32 series,
containing 2 cores, operating at 240 MHz, 320 KiB RAM, 448 KiB ROM, WiFi 802.11 b/g/n,
Bluetooth v4.2 BR/EDR and BLE (shares the radio with WiFi)

# ESP32-S3
The dual-core ESP32-S3 with a clock of 240 MHz is similar to the original and, unlike the S2, has Bluetooth 5 (LE) support. In addition, it has instructions specific to neural networks and signal processing which is helpful in edge machine learning applications.
Other key differences between ESP32-S3 and ESP32 include support for larger capacity and higher-speed SPI flash and PSRAM chips, a few more I/O pints, more security features, a lack of Ethernet MAC, and support for Bluetooth LE 5.0 connectivity with long-range and 2Mbps transfer support.

* [ESP32-S3](https://esp32s3.com/)
* [Unexpected Maker](https://unexpectedmaker.com/esp32s2)
* [Announcing ESP32-S3 for AIoT Applications](https://www.espressif.com/en/news/ESP32_S3)
* [Espressif's New ESP32-S3 Adds AI Features for IoT Devices](https://www.hackster.io/news/espressif-s-new-esp32-s3-adds-ai-features-for-iot-devices-b42b902abdf5)
* [New Part Day: Espressif ESP32-S3](https://hackaday.com/2021/01/09/new-part-day-espressif-esp32-s3/)

# ESP32-C3
When Esspresif introduced the ESP32-C3, it was their first SOM based entirely on the RISC-V architecture. It is a low-cost, single-core SOM with WiFi and Bluetooth support.

The new “C3” variant has a single 160 MHz RISC-V core that out-performs the ESP8266, and at the same time includes most of the peripheral set of an ESP32. While RAM often ends up scarce on an ESP8266 with around 40 kB or so, the ESP32-C3 sports 400 kB of RAM, and manages to keep it all running while burning less power. Like the ESP32, it has Bluetooth LE 5.0 in addition to WiFi.

ESP32-C3 is a single-core WiFi and Bluetooth 5 (LE) microcontroller SoC, based on the open-source RISC-V architecture for secure IoT applications. It strikes the right balance of power, I/O capabilities and security, thus offering the optimal cost-effective solution for connected devices.

* [he SMALLEST ESP32 Board 🔥🔥 | Getting Started with XIAO ESP32 C3 | ESP32 Projects](https://www.youtube.com/watch?v=PZZYTYN8hjc)
* [Seeed Studio Launches Compact RISC-V XIAO ESP32-C3 for Battery-Powered Wearable and IoT Projects](https://www.hackster.io/news/seeed-studio-launches-compact-risc-v-xiao-esp32-c3-for-battery-powered-wearable-and-iot-projects-932ba7de4fe9)
* [RISC-V based XIAO ESP32C3 is enabled with Wi-Fi and BLE](https://linuxgizmos.com/risc-v-based-xiao-esp32c3-is-enabled-with-wi-fi-and-ble/)
* [HANDS-ON: THE RISC-V ESP32-C3 WILL BE YOUR NEW ESP8266](https://hackaday.com/2021/02/08/hands-on-the-risc-v-esp32-c3-will-be-your-new-esp8266/)
* [RISC-V: How much is open source? Featuring the new ESP32-C3](https://www.youtube.com/watch?v=VdPsJW6AHqc)
* [XIAO ESP32C3](https://www.cnx-software.com/2022/07/08/seeed-studio-5-xiao-esp32c3-board-wifi-and-ble-battery/)



# Interrogate ESP Device

```bash
/home/jeff/.arduino15/packages/esp32/tools/esptool_py/4.2.1/esptool.py chip_id

/home/jeff/.arduino15/packages/esp32/tools/esptool_py/4.2.1/esptool.py flash_id | grep -e flash -e Chip -e Features
```



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

