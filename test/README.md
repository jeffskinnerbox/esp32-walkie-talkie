<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----


To find the type of package, architecture, and board you should be using,
run the following command and search for you boards name or manufacture

```bash
arduino-cli board listall
```


[ESP32 for IoT: A Complete Guide](https://www.nabto.com/guide-to-iot-esp-32/)
[The Internet of Things with ESP32](http://esp32.net/)
[Wikipedia: ESP32](https://en.wikipedia.org/wiki/ESP32)



## ESP8266

## Original ESP8266

## HiLetgo ESP8266 ESP-12E NodeMCU
HiLetgo ESP8266 NodeMCU CP2102 ESP-12E Development Board
It has 15+15 pins.
[HiLetgo ESP8266 ESP-12E NodeMCU][18]
[pinout for ESP8266 12-E NodeMCU][20]

```bash
# fully qualified board name (FQBN)
FQBN = esp8266:esp8266:nodemcuv2
```



---------



## ESP32

## DOIT ESP32 DEVKIT DOIT
There are two versions of this board, with 30 and with 38 pins.
Both work in a similar way.
I prefer the version with 38 pin since its breadboard friendly, and a result cost more.

### DOIT ESP32 DEVKIT DOIT V1
### MELIFE ESP-32S
**NOTE: Not Breadboard Friendly!**
It has 15+15 pins.
I purchased MELIFE ESP-32S (aka ESP-WROOM-32, nodemcu-32s) Development Board
**NOTE:** It has 'ESP32 DEVKITV1' printed on the back of the board
[MELIFE nodemcu-32s][13]
[pinout for MELIFE nodemcu-32s][13]

```bash
# fully qualified board name (FQBN)
FQBN = esp32:esp32:esp32doit-devkit-v1
```

### DOIT ESP32 DEVKIT DOIT V2
### KeeYees ESP32S Dev Board
### HiLetgo ESP-WROOM-32
This is the 19+19 pin version.
[HiLetgo ESP-WROOM-32][15]
[pinout for HiLetgo ESP-WROOM-32][15]

```bash
# fully qualified board name (FQBN)
FQBN = esp32:esp32:nodemcu-32s
```

## ESP32 WeMos Lolin32
## ESP32 WROOM Lolin32
It has 20+16 pins.
Has Lithium battery interface
[pinout for ESP32 WeMos Lolin32][14]

```bash
# fully qualified board name (FQBN)
FQBN = esp32:esp32:lolin32
```

## ESP32 WROOM Lolin32 Lite
Has Lithium battery interface
It has 13+13 pins.
[pinout for ESP32 WeMos Lolin32 Lite][12]

The Wemos LOLIN32 Lite board is discontinued, but several clones are still being offered
(e.g. AITRIP).

https://www.amazon.com/AITRIP-Bluetooth-Development-Connector-Compatible/dp/B0BCJY8HDY

```bash
# fully qualified board name (FQBN)
FQBN = esp32:esp32:lolin32-lite
```

## LilyGO TTGO T-Display ESP32
## HiLetgo ESP32 LCD WiFi Kit
[HiLetgo ESP32 1.14 Inch LCD Display][16]
Appears to be the same as the [LilyGO TTGO T-Display ESP32][21]

[pinout for HiLetgo ESP32 1.14 Inch LCD Display][22]
It has 12+12 pins.

Chipset: ESPRESSIF-ESP32 240MHz Xtensa single-/dual-core 32-bit LX6 Microprocessor
Display: 1.14 Inch LCD
Radio: WiFi 802.11bgn, Bluetooth
FLASH: QSPI flash 4MB
SRAM: 520KB SRAM
USB: Type-C
USB to TTL: CH9102

```bash
# fully qualified board name (FQBN)
FQBN = esp32:esp32:ttgo-lora32
```

**NOTE:** [How to Install and Setup TTGO T-Display ( Getting Started tutorial)](https://www.youtube.com/watch?v=b8254--ibmM)
greatly assisted in identifing the required FQBN and libraries required to work with the TTGO T-Display ESP32.

## Heltec ESP32 0.96 Inch OLED Display
## MakerFocus ESP32 0.96 Inch OLED Display
Appears to be the same as the Heltec WiFi Kit 32
MakerFocus ESP32 Development Board Upgraded Version 8MB Flash, ESP32 WiFi Bluetooth, ESP32 OLED 0.96 Inch OLED Display
It has 18+18 pins.
[MakerFocus ESP32 0.96 Inch OLED Display][17]
[pinout for MakerFocus ESP32 0.96 Inch OLED Display][23]

```bash
# fully qualified board name (FQBN)
FQBN = esp32:esp32:heltec_wifi_kit_32
```

## DFRobot FireBeetle Board ESP32-E
[DFRobot FireBeetle Board ESP32-E][25] (aka FireBeetle 2 ESP32-E IoT Microcontroller)
**Not** the same as FireBeetle ESP32 IoT Microcontroller
[pinout for DFRobot FireBeetle Board ESP32-E][26]
It has 18+14 pins.

It is equipped with a DFRobot GDI(General Display Interface)
to connect screens like [DFR0664][27]/[DFR0649][28]/[DFR0665][29]/[DFR0669][30]

```bash
# fully qualified board name (FQBN)
FQBN = esp32:esp32:dfrobot_firebeetle2_esp32s3
```

## TinyS2 (ESP32-S2 Board)
To find out what revision TinyS2 you have, look under the USB connector on the back of the board and you will find the P revision.
Mine is P5
Use the Pinout diagram for "revision P3 and newer".
It has 11+12 pins.
[TinyS2 (ESP32-S2 Board)][19]
[pinout for TinyS2 (ESP32-S2 Board)][24]

```bash
# fully qualified board name (FQBN)
FQBN = esp32:esp32:tinys2
```

* The red power LED doesn't light up when running off battery to reduce power consumption.
* The orange charge LED blinks when no battery is connected.
The charge IC can’t tell the difference between a full battery and no battery,
so it cycles between trying to charge and not charge.
(**NOTE:** You can disable the orange status LED by cutting the solder jumper on the back of the TinyS2.)
* The TinyS2 can read an approximate voltage from a connected battery via IO3.
* TinyS2 has the ability to check if a 5V power source is connected to the board
by reading if IO21 is high.
(**NOTE:** This allows you to alter your code when running off battery when you have a 5V source present or not.)


[What is the difference between /dev/ttyUSB and /dev/ttyACM?](https://rfc1149.net/blog/2013/03/05/what-is-the-difference-between-devttyusbx-and-devttyacmx/)
/dev/ttyACM0 is a USB communication device (CDC) of sub-type "abstract control model" (ACM).
/dev/ttyS0 is a hardware serial port - the (typically) 9-pin D connector on the back of your computer.




[10]:
[11]:
[12]:https://templates.blakadder.com/wemos_LOLIN32_Lite_v1.html
[13]:https://www.amazon.com/gp/product/B07Q576VWZ
[14]:https://www.mischianti.org/2021/02/21/esp32-wemos-lolin32-high-resolution-pinout-and-specs/
[15]:https://www.amazon.com/gp/product/B0718T232Z
[16]:https://www.amazon.com/dp/B07X1W16QS
[17]:https://www.amazon.com/gp/product/B076KJZ5QM
[18]:https://www.amazon.com/dp/B010N1SPRK
[19]:https://www.amazon.com/dp/B093CJ4V3Y
[20]:https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
[21]:https://www.amazon.com/LILYGO-T-Display-Arduino-Development-CH9102F/dp/B099MPFJ9M
[22]:hhttp://www.lilygo.cn/prod_view.aspx?TypeId=50062&Id=1400&FId=t3:50062:3
[23]:https://resource.heltec.cn/download/WiFi_Kit_32/WIFI_Kit_32_pinoutDiagram_V2.1.pdf
[24]:https://unexpectedmaker.com/tinys2
[25]:https://www.amazon.com/dp/B08VNY821Y
[26]:https://www.dfrobot.com/product-2195.html

[27]:https://www.dfrobot.com/product-2071.html
[28]:https://www.dfrobot.com/product-2072.html
[29]:https://www.dfrobot.com/product-2106.html
[30]:https://www.dfrobot.com/product-2107.html

