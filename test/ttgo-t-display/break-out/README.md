<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


----



# Alien Attack Game
An Break Out game on the 135x240 TFT display of the [LilyGO TTGO T-Display ESP32][04].

Source:
* [How to Install and Setup TTGO T-Display (Getting Started tutorial)][01]
* [TTGO T-Display Board (ESP32) BRAKEOUT Game][03]
* [Code for Break Out Game][02]

## LilyGO TTGO T-Display ESP32
## HiLetgo ESP32 LCD WiFi Kit
[HiLetgo ESP32 1.14 Inch LCD Display][02]
Appears to be the same as the [LilyGO TTGO T-Display ESP32][04]

[pinout for HiLetgo ESP32 1.14 Inch LCD Display][05]
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

>**NOTE:** This [tutorial][01] greatly assisted in identifing the required FQBN
>and libraries required to work with the TTGO T-Display ESP32.

#### Step 1: Clone the Alien Attack Code
Very first step to creat this test program is to clone the [code for Alien Attack Game][02].

```bash
# go to the directory where the code will be stored
cd .../break-out

# download the break out game cod2
wget https://drive.google.com/u/0/uc?id=1_lYi8ClOMExkd-qtNqBLwg-l2Y3UiV7F&export=download
```

#### Step 2: Install Required Libary
```bash
# search to see if library is already installed
$ arduino-cli lib search TFT_eSPI | grep -i name: | grep -i tft_espi
Name: "Digital Rain Animation for TFT_eSPI"
Name: "TFT_eSPI"
Name: "TFT_eSPI_ES32Lab"

# install required library
arduino-cli lib install TFT_eSPI
```

#### Step 3: Edit `TFT_eSPI` Files
```bash
# make a backup copy of the target file
cp ~/Arduino/libraries/TFT_eSPI/User_Setup_Select.h ~/tmp/User_Setup_Select.h

# make the required edits to the header file
sed -i 's/\#include <User_Setup.h>/\/\/#include <User_Setup.h>/' ~/Arduino/libraries/TFT_eSPI/User_Setup_Select.h
sed -i 's/\/\/#include <User_Setups\/Setup25_TTGO_T_Display.h>/#include <User_Setups\/Setup25_TTGO_T_Display.h>/' ~/Arduino/libraries/TFT_eSPI/User_Setup_Select.h

# do a visual inspection of the file
vi ~/Arduino/libraries/TFT_eSPI/User_Setup_Select.h
```

#### Step 4: Compile and Load the Code
```bash
# compile the alien attack code
make build

# upload the code to the ttgo board
make upload
```



[01]:https://www.youtube.com/watch?v=b8254--ibmM
[02]:https://www.amazon.com/dp/B07X1W16QS
[03]:https://www.youtube.com/watch?v=N6V7ZJkhSbc&t=0s
[04]:https://www.amazon.com/LILYGO-T-Display-Arduino-Development-CH9102F/dp/B099MPFJ9M
[05]:hhttp://www.lilygo.cn/prod_view.aspx?TypeId=50062&Id=1400&FId=t3:50062:3

