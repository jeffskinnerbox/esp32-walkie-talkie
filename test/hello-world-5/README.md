<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.0.1
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


---

# DESCRIPTION
Test of `DeBug.cpp` with an active WiFi connection,
thereby enabling `telnet`.
This should work anyway but will given you a warning if you attempt to use `telnet` features.
This configuration is fine for any application that doesn't plan to use `telnet`.

If you want to use `telnet`, you must start-up WiFi.
For WiFi, the class `WiFiHandler.cpp` is preferred but not required.

# PHYSICAL DESIGN
GPIO2: This pin is connected to the blue LED on the board - https://esphome.io/devices/nodemcu_esp32.html

## Hardware

## Wiring

## Software Settings


------


# OTA Upload

```bash
# check all the rules added by the UFW firewall
sudo ufw status verbose

# open ports 3232
sudo ufw allow 3232/tcp

# to remove these rules, use the following:
sudo ufw status numbered
sudo ufw delete 4
```

```bash
# upload firmware via over-the-air (ota)
/home/jeff/.arduino15/packages/esp32/hardware/esp32/2.0.5/tools/espota.py \
    --debug --progress --ip=192.168.1.109 --port=3232 --host_port=3232 --auth=123 \
    --file=/tmp/hello-world-4/build/hello-world-4.ino.bin

# upload firmware via over-the-air (ota) - abbreviated arguments
/home/jeff/.arduino15/packages/esp32/hardware/esp32/2.0.5/tools/espota.py \
    -d -r -i 192.168.1.109 -p 3232 -P 3232 -a 123 -f /tmp/hello-world-4/build/hello-world-4.ino.bin
```

