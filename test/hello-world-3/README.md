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

## Hardware

## Wiring

## Software Settings




# DESCRIPTION
This a ESP32 test routine uses the WiFiHandler and DeBug classes.
Debug trace messages will be sent to the ESP32's USB serial interface and
those same messages can be viewed remotely via `telnet` using
the ESP32 TelnetStream connectivity.

Telnet is an application protocol used over the Internet or local area network.
Telnet provides a bidirectional interactive text communications
(virtual teletype, employing a 7-bit ASCII character set)
using a virtual terminal connection.

# PHYSICAL DESIGN

## Hardware
ESP-32S (aka ESP-WROOM-32 ESP32) Development Board - https://www.amazon.com/gp/product/B0718T232Z/

## Wiring
Nothing required

# MONITOR
To monitor the trace messages of the ESP32 development board,
connection the USB port to a Linux computer and execute the following:

    screen /dev/ttyUSB0 9600,cs8cls

To terminate monitoring, enter `CNTR-a :quit`.
To stop the screen scrolling, enter `CNTR-a :`.

To monitor using telnet, execute the following:

    telnet ESP_24F9FB             # using esp32 hostname ('ESP_' + chip ID)
    telnet 192.168.1.109          # using esp32 ip address
    telnet hello-world-3.local

To terminate telnet monitoring, enter `Ctrl-] quit`.

To find the device on your LAN:

    sudo netdiscover -c 3 -s 10 -L -N -r 192.168.1.0/24
    sudo arp-scan 192.168.1.0/24 | grep Espressif

# TESTING
To test the telnet capabilities

    google-chrome http://hello-world-3.local/hello

# REFERENCE MATERIALS
* [RemoteDebug Library](https://github.com/JoaoLopesF/RemoteDebug)

# SOURCES



------



# mDNS
[Multicast Domain Name System (mDNS)][01] is one of the features of Bonjour,
included in Mac OS X 10.2 or later, and the linux equivalent called [Avahi][17].
mDNS resolves host names to IP addresses within small networks that do not include a local name server.
It is a [zero-configuration service (aka Zeroconf)][16],
using essentially the same programming interfaces,
packet formats, and operating semantics as the [unicast Domain Name System (DNS)][02].
When an mDNS client needs to resolve a host name or network service,
it sends an IP multicast query message that asks the host having that name to identify itself.
That target machine then multicasts a message that includes its IP address.
All machines in that subnet can then use that information to update their mDNS caches.

On the Internet at large, a DNS name such as `www.apple.com` must be resolved
to a numbered IP address before your computer can connect to the server.
A domain name system (DNS) server normally provides this name resolution service for your computer.
Your Internet service provider or network administrator normally provides a local DNS server for your use.

When an mDNS client needs to resolve a host name,
it sends an IP multicast query message that asks the host having that name to identify itself.
That target machine then multicasts a message that includes its IP address.
All machines in that subnet can then use that information to update their mDNS caches.

Networking device hostnames ending with [`.local`][18] are often employed in private networks,
where they are resolved either via the multicast domain name service (mDNS)
or local Domain Name System (DNS) servers.
Anywhere that a you could normally use a DNS name such as `www.apple.com`,
you could instead enter the Bonjour name of the computer on the local network.
To indicate that the name should be looked up using local multicast instead of a standard DNS query,
all Bonjour host names end with the extension `.local`.
This helps avoid confusion between names that are local Bonjour computer names
(`mycomputer.local`) and globally unique DNS names (`www.apple.com`).

Sources
* [Find ESP32/ESP8266 IP Address on a WiFi Network using mDNS](https://www.megunolink.com/articles/wireless/find-esp32-esp8266-ip-address-with-mdns/)
* [ESP32: mDNS address resolution](https://techtutorialsx.com/2020/04/17/esp32-mdns-address-resolution/)
* [How to discover ESP32 service over mDNS](https://iotespresso.com/how-to-discover-esp32-service-over-mdns/)

# mDNS Address Resolution
To assure that you have mDNS services on your local network,
check if `avahi-deamon` is running and its status was OK.
I did this with the following:

```bash
# install avahi & mdns tools
sudo apt-get install avahi-daemon avahi-discover avahi-utils libnss-mdns mdns-scan

# is the avahi-daemon running
$ ps -e | grep avahi
   1440 ?        00:00:03 avahi-daemon
   1491 ?        00:00:00 avahi-daemon

# status of the avahi-daemon
$ sudo service avahi-daemon status
● avahi-daemon.service - Avahi mDNS/DNS-SD Stack
     Loaded: loaded (/lib/systemd/system/avahi-daemon.service; enabled; vendor preset: enabled)
     Active: active (running) since Sat 2022-11-19 10:26:45 EST; 9h ago
TriggeredBy: ● avahi-daemon.socket
   Main PID: 1440 (avahi-daemon)
     Status: "avahi-daemon 0.8 starting up."
      Tasks: 2 (limit: 18915)
     Memory: 1.5M
        CPU: 3.531s
     CGroup: /system.slice/avahi-daemon.service
             ├─1440 "avahi-daemon: running [desktop.local]"
             └─1491 "avahi-daemon: chroot helper"

# scan for devices registered with mdns
$ mdns-scan
+ hubv2-24fd5b0000036206._ssh._tcp.local
+ hubv2-24fd5b0000036206._sftp-ssh._tcp.local
+ Philips Hue - 4A1515._hue._tcp.local
+ Chromecast-f92240088fe3919c2199f9d74f378538._googlecast._tcp.local
+ f9224008-8fe3-919c-2199-f9d74f378538._googlezone._tcp.local
+ iPhone (7)._rdlink._tcp.local
+ ntp-clock._arduino._tcp.local
+ nas._smb._tcp.local
+ nas._device-info._tcp.local
+ nas._webdavs._tcp.local
+ 93EB5643-7261-5F1D-8B2E-6C83A90212E4._homekit._tcp.local
```




# Setting Hostname
By default, each ESP8266 uses the last six hexdecimal digits of its MAC address
prefixed by `ESP_` to make its hostname unique (e.g. `ESP_B19F40`).
You can change this from within your sketch by calling `WiFi.hostname()`
and passing the function a string containing the new hostname.
This call **MUST** occur before you start the connection with `WiFi.begin()`.

# Using .local Hostnames
A hostname is a name assigned to a device on a network to more easily distinguish
between devices instead of using a IP addresses.
Hostnames can be resolved on a LAN without using a custom DNS
or specifying static IP addresses through the router.
To do so, devices connected to the network must be running a Zero Configuration Networking (Zeroconf) service
which then allows any device to be referenced by adding `.local` to its hostname.

On Linux, the Zeroconf service is called Avahi.
Avahi supports service discovery via the mDNS/DNS-SD protocol suite.
This enables you to plug your computer into a network
and instantly be able to view hostnames and services on your LAN,
much like you would with DNS on the Internet.

The big advantage of mDNS is that we don’t need to know the IP address assigned to the ESP8266 to access the HTTP webserver running on it. On top of that, we don’t need to deploy a dedicated server just to do the resolution of names into IPs.

So, we can just define that the ESP8266 will be listening on something like `myesp.local` and we can just access the server in a web browser by typing `http://myesp.local/path` instead of having to know the IP address. Besides this enhancement, the HTTP web server will work the same as it did before.

In order for this example to work, the machine that is accessing the ESP8266 web server also needs to support mDNS. Otherwise, it won’t be able to send the query needed to receive the resolved IP.

```bash
# install avahi suite
sudo apt-get install avahi-daemon avahi-discover avahi-utils libnss-mdns mdns-scan

# check if avahi daemon is running
sudo systemctl status avahi-daemon

# scan for mDNS/DNS-SD services published on the local network
$ mdns-scan
+ googlerpc._googlerpc._tcp.local
+ Chromecast-1aa07ba95aaa5b96c2c6db5ba7b23e35._googlecast._tcp.local
+ 1aa07ba9-5aaa-5b96-c2c6-db5ba7b23e35._googlezone._tcp.local
+ HP LaserJet P2035 @ desktop._printer._tcp.local
+ HP LaserJet P2035 @ desktop._ipps._tcp.local
+ HP LaserJet P2035 @ desktop._ipp._tcp.local
+ Philips Hue - 4A1515._hue._tcp.local
+ Philips hue - 4A1515._hap._tcp.local
Browsing ...

# browse for mDNS/DNS-SD services using the Avahi daemon
avahi-browse --all
```

# Asynchronous Web Server
Legacy web applications are synchronous in nature.
This means that the presentation delivered to the user represents a snapshot in time of what is a dynamic system.
In the Asynchronous Web it is possible to deliver spontaneous presentation
changes to the user as the state of a dynamic system changes,
without the need for the user to interact with the interface.
The advantages are obvious as we can now maintain an accurate view onto the system for the user.

* [What is the Asynchronous Web, and How is it Revolutionary?](https://www.theserverside.com/news/1363576/What-is-the-Asynchronous-Web-and-How-is-it-Revolutionary)
* [ESP32 Async Web Server – Control Outputs with Arduino IDE (ESPAsyncWebServer library)](https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/)

#### Step 1: Installing the Asynchronous Web Server library
The [ESPAsyncWebServer library][03] is not available to install in the Arduino IDE Library Manager.
So, you need to install it manually.
In addition, the ESPAsyncWebServer library requires the [ESPAsyncTCP library][04] to work.

```bash
# download the ESPAsyncWebServer library
cd ~/Downloads
wget https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip

# uncompress the zip file
unzip master.zip

# rename the folder master to ESPAsyncWebServer
mv ESPAsyncWebServer-master ESPAsyncWebServer

# move the ESPAsyncWebServer folder to your arduino ide installation libraries folder
mv ESPAsyncWebServer ~/src/arduino/sketchbooks/libraries/
```

Now do the same for the ESPAsyncTCP library needed for ESP8266:

```bash
# download the ESPAsyncTCP library
cd ~/Downloads
wget https://github.com/me-no-dev/ESPAsyncTCP/archive/master.zip

# uncompress the zip file
unzip master.zip

# rename the folder master to ESPAsyncTCP
mv ESPAsyncTCP-master ESPAsyncTCP

# move the ESPAsyncTCP folder to your arduino ide installation libraries folder
mv ESPAsyncTCP ~/src/arduino/sketchbooks/libraries/
```

Now do the AsyncTCP library needed for ESP32:

```bash
# clean up previous work
cd ~/Downloads
rm master.zip

# download the AsyncTCP library
wget https://github.com/me-no-dev/AsyncTCP/archive/master.zip

# uncompress the zip file
unzip master.zip

# rename the folder master to AsyncTCP
mv AsyncTCP-master AsyncTCP

# move the AsyncTCP folder to your arduino ide installation libraries folder
mv AsyncTCP ~/src/arduino/sketchbooks/libraries/
```

Now do the same for the ESPAsyncTCP library needed for ESP32:


* [Installing the Asynchronous Web Server library](https://reacoda.gitbook.io/molemi-iot/introducing-the-nodemcu/display-the-dht11-sensor-reading-on-a-web-server-using-nodemcu./installing-dht-library-on-the-esp8266/installing-the-asynchronous-web-server-library)
* [Installing Libraries Manually](https://learn.adafruit.com/adafruit-all-about-arduino-libraries-install-use/how-to-install-a-library)



[01]:http://www.multicastdns.org/
[02]:https://stackoverflow.com/questions/40080244/what-is-unicast-dns-and-how-does-it-differ-from-other-kinds-of-dns
[03]:https://github.com/me-no-dev/ESPAsyncWebServer
[04]:https://github.com/me-no-dev/ESPAsyncTCP
[16]:http://www.practicallynetworked.com/sharing/configure_and_use_avahi_and_linux.htm
[28]:http://compnetworking.about.com/od/dns_domainnamesystem/f/dns_servers.htm

