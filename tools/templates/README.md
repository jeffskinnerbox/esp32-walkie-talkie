<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.6
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----


# DESCRIPTION

# PHYSICAL DESIGN

## Hardware

### <Specific Device(s)>

## Wiring

# SOFTWARE SETTINGS

# MONITOR
If the files `DeBug.cpp` & `DeBug.h` were compiled into the code,
it will provide you trace information useful for debugging.
You can monitor the execution of the program via the USB serial port,
connection the USB port to a Linux computer and execute the following:

    screen /dev/ttyUSB0 9600,cs8cls

To terminate `screen`, you should enter: `CNTR-a :quit`.
To stop the screen scrolling, enter: `CNTR-a :`.

When you can't connect with the devices USB port,
you can get the same trace messages remotely via `telnet`:

    telnet <hostname>lock.local
    telnet 192.168.1.44

To terminate `telnet` you should use `Ctrl-]` followed by `quit`.

If you don't have the host name or you didn't use the [mDNS service][01] in the program,
you can get the IP address of this program by doing one of the following:

    sudo arp-scan 192.168.1.0/24 | grep Espressif
    sudo netdiscover -c 3 -s 10 -L -N -r 192.168.1.0/24 | grep Espressif

# CALIBRATION

# TESTING

# REFERENCE MATERIALS
These documents were helpful in the design of this project:

# SOURCES
Source code for this project was taken from:



[01]:https://espressif.github.io/esp-protocols/mdns/en/index.html
[02]:
[03]:
[04]:
[05]:
[06]:
[07]:
[08]:
[09]:
[10]:

