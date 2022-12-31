<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.1.0
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----



Additional `CardHandler` services ideas can be found here
[Interfacing Micro SD Card Module with Arduino](https://lastminuteengineers.com/arduino-micro-sd-card-module-tutorial/)


# DESCRIPTION
The `i2s-music-player` program uses the ESP32 I2
reads a WAV file for  from a
Micro SD TF Card Adapter
and plays the music via a head phone

Sources:
* [Interfacing Micro SD Card Module with Arduino][11]
* [List files stored in SD Card connected to Arduino](https://www.tutorialspoint.com/list-files-stored-in-sd-card-connected-to-arduino)
* [Building a Digital Music Player with I2S?! What is I2S! EB#45](https://www.youtube.com/watch?v=qNLvoSQCx60)
* [ESP32 Sound - Working with I2S](https://www.youtube.com/watch?v=m-MPBjScNRk)
* [I2S Sound Tutorial for ESP32](https://diyi0t.com/i2s-sound-tutorial-for-esp32/)
* [Guide to SD Card Module with Arduino](https://randomnerdtutorials.com/guide-to-sd-card-module-with-arduino/)
* [ESP32: Guide for MicroSD Card Module using Arduino IDE][13]
* [How to use SD card with esp32 – 2](https://www.mischianti.org/2021/03/28/how-to-use-sd-card-with-esp32-2/)

## SD Card Module
Embeded devices can use a [Micro SD (aka TF or TransFlash) card][08],
typically used in phones, for mass storage.
The SD card module can be very useful for projects that require data logging
or when data needs to be retrived for use with your program.
The [Arduino IDE has a SD library][09] that helps you
create directories and file so you can read/write on the SD card.
There are different SD models from different suppliers,
but they all work in a similar way,
using the [SPI communication protocol][10].
SD card module are very inexpensive and I purchased a [package of 5 from Amazon][06].

## I2S Decoder/Amplifier
Adafruit [MAX98357][07] I2S decoder/amplifier

# PHYSICAL DESIGN
I used a ESP32 [NodeMCU-32s][03], suppling it power via the serial USB port,
including the SD card module, Adafruit [MAX98357][07] I2S decoder/amplifier, perpherial devices.

## Hardware
I used a [HiLetgo nodemcu-32s][01] and supplied power via the serial USB port.
I used a bench power supply to apply voltage to the ADC input pin
and a Linux terminal, attached to the USB serial port,
to see the predicited value of that voltage.

### Micro SD Card Adater Reader Module
[HiLetgo Micro SD TF Card Adater Reader Module][06]
6 Pin SPI Interface Driver Module

## Wiring
The ESP32 features two SPI interfaces: [HSPI and VSPI][14].
(See "[ESP32: Guide for MicroSD Card Module using Arduino IDE][13]" for details.)
Using the [VSPI pinout for HiLetgo nodemcu-32s][02] and interface the [SD Card Module][11],
the following wiring plan should be used:

| SD Card Module  |   NodeMCU-32s    |
|:---------------:|:----------------:|
| CS              | GPIO-05 / PIN-29 |
| SCK             | GPIO-18 / PIN-30 |
| MOSI            | GPIO-23 / PIN-37 |
| MOSO            | GPIO-19 / PIN-31 |
| VCC (5V)        | VCC (5V)         |
| GRD             | GRD              |

### Preparing the SD Card
Sources:
* [How to Format a SD Card to FAT32 in Windows/macOS/Linux](https://www.fastoe.com/blog/format-sd-card-to-fat32)
* [How to Write, Format, and Manage an SD Card in Linux](https://www.makeuseof.com/tag/write-format-manage-sd-card-linux/)

Sometimes you find SD Cards in less than ideal state,
so you might want to wipe the disk clean.
The process below will wipe all data from the disk
(**NOTE:** This takes some time!  over 30 minutes for a 8GB disk):

```bash
# instert the usb sd card reader (card installed)
# into the computer and find its device file
$ lsblk | grep media
├─sdg1   8:97   1    56M  0 part  /media/jeff/boot
└─sdg2   8:98   1   7.4G  0 part  /media/jeff/13d368bf-6dbf-4751-8ba1-88bed06bef77

# copying zeroes onto the drive
$ sudo dd if=/dev/zero of=/dev/sdg
dd: writing to '/dev/sdg': No space left on device
15564801+0 records in
15564800+0 records out
7969177600 bytes (8.0 GB, 7.4 GiB) copied, 2835.97 s, 2.8 MB/s

# check the disk status
$ lsblk | grep media
└─sdg1   8:97   1   7.4G  0 part  /media/jeff/3400-001A

# eject the usb drive
sudo eject /dev/sdg
```







```bash
# install the graphical partioning tool
sudo apt-get install gparted

# instert the usb sd card reader (card installed)
# into the computer and find its device file
$ lsblk | grep media
└─sdg1   8:97   1   7.4G  0 part  /media/jeff/5E60-38BB
```

To use [GParted (GNOME Partition Editor)][16],
enter the following command, `sudo gparted`, in the terminal.
Now do the following in the GParted GUI:

* Find your USB drive in the drop-down box at the top right of the GParted window.
Make sure your in the right drive (`/dev/sdg` for this example).
* Select the `/dev/sdg1` drive
* Click on the button **Partition** > **Unmount**
* Select the `/dev/sdg1` drive
* Click on the button **Partition** > **Format** > **fat32**
* Click the **Green Checkmark** and then the **Apply** button to apply the changes.
* Exit GParted

Now check the disk status:

```bash
# check the disk status
$ lsblk | grep media
└─sdg1   8:97   1   7.4G  0 part  /media/jeff/8A39-8F59
```







One of my [sources][11] for this project,
makes a strong recommended that you use the [official SD card formatter utility][15]
developed by the SD association.

You first must prepare the SD Card by formatting it as FAT16 or FAT32.
To formatting the SD card,
place it in a [USB SD Card reader][12] and use the following Linux commands:

```bash
# install disk partition tool
sudo apt install parted

# instert the usb sd card reader (card installed)
# into the computer and find its device file
$ lsblk | grep media
├─sdg1   8:97   1    56M  0 part  /media/jeff/boot
└─sdg2   8:98   1   7.4G  0 part  /media/jeff/13d368bf-6dbf-4751-8ba1-88bed06bef77
```

In the example above, the name of the SD device is `/dev/sdg`.

Next we will create a partition and do the formatting.
We will format the USB drive with the SD card to FAT32
and make a single partition.

```bash
# unmount the drive
sudo umount /dev/sdg1 /dev/sdg2

# create the partition table
sudo parted /dev/sdg --script -- mklabel msdos

# create a fat32 partition that takes up the whole sd card
sudo parted /dev/sdg --script -- mkpart primary fat32 1MiB 100%

# format the boot partition to fat32
sudo mkfs.vfat -F32 /dev/sdg1
```

Once done, use the command below to print the partition table
and verify that everything is set up correctly:

```bash
# print the partition table
$ sudo parted /dev/sdg --script print
Model: Generic STORAGE DEVICE (scsi)
Disk /dev/sdg: 7969MB
Sector size (logical/physical): 512B/512B
Partition Table: msdos
Disk Flags:

Number  Start   End     Size    Type     File system  Flags
 1      1049kB  7969MB  7968MB  primary  fat32        lba
```

Within the DeBug trace messages, you could get the following errors:

* If you there is no SD Card install, you'll get the error message: "SD card mount failure."
* If you have an unfomated SD Card installed, you'll get the error message: "No SD Card attached"
* In both cases, you'll also get the message: "SD Card initialization failure"

## Software Settings
None

# MONITOR
To monitor the scrolling display's activities on the NodeMCU serial ports while operating,
connection the USB port to a Linux computer and execute the following:

    screen /dev/ttyUSB0 9600,cs8cls

To terminate monitoring, enter: `CNTR-a :quit`.
To stop the screen scrolling, enter: `CNTR-a :`.

# CALIBRATION
Due to variation in internal reference voltage of different ESP32 chips,
the ESP32 ADCs needs to be calibrated before using.
One way of doing calibration is using the true ADC reference voltage stored in [eFuse][04].
This value is measured and written to the ESP32 when it was made at factory.

>**NOTE:** If you choose do the calibration yourself,
>check out this [ADC calibration tool][05].

# TESTING
Apply a voltage (2.4V or less) to GPIO Pin35 and compare that to what the program
prints on the USB serial port.

# REFERENCE MATERIALS
Key documentation for understand this design can be found here:

* [ESP32 I2S Audio](https://github.com/atomic14/esp32_audio)
* [ESP32 Audio Input - MAX4466, MAX9814, SPH0645LM4H, INMP441](https://atomic14.com/2020/09/12/esp32-audio-input.html)
* [Measuring analog inputs with ESP32 ADC (ESP-IDF)](https://embeddedexplorer.com/esp32-adc-esp-idf-tutorial/)
* [Analog to Digital Converter](https://docs.espressif.com/projects/esp-idf/en/v4.1.1/api-reference/peripherals/adc.html)

# SOURCES
Source code was taken from:

* [ESP32 I2S Audio](https://github.com/atomic14/esp32_audio)



[01]:https://www.amazon.com/dp/B010N1SPRK
[02]:https://forum.fritzing.org/t/esp32s-hiletgo-dev-boad-with-pinout-template/5357
[03]:https://www.teachmemicro.com/using-esp32-for-the-first-time/
[04]:https://en.wikipedia.org/wiki/EFuse
[05]:https://github.com/tommag/ESP32_ADC_Calibration_tool
[06]:https://www.amazon.com/dp/B07BJ2P6X6
[07]:https://learn.adafruit.com/adafruit-max98357-i2s-class-d-mono-amp
[08]:https://appuals.com/what-is-tf-transflash-card-and-how-is-it-different-from-micro-sd/
[09]:https://www.arduino.cc/reference/en/libraries/sd/
[10]:https://www.circuitbasics.com/basics-of-the-spi-communication-protocol/
[11]:https://lastminuteengineers.com/arduino-micro-sd-card-module-tutorial/
[12]:https://www.amazon.com/dp/B015QVIJFA
[13]:https://randomnerdtutorials.com/esp32-microsd-card-arduino/
[14]:https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/spi_master.html#:~:text=SPI2%20and%20SPI3%20are%20general,same%20number%20of%20SPI%20slaves.
[15]:https://www.sdcard.org/downloads/formatter/
[16]:https://gparted.org/
[17]:
[18]:
[19]:
[20]:


