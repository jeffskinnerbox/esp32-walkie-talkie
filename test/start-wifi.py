
"""
    CircuitPython WiFi Startup Script.

    Description:
    Scan for WiFi networks, prompt the user for SSID & password, initiate a
    WiFi connection to the device, and then do a ping test to google.com for
    validation of connection.

    Usage:
    cp start-wifi.py <path-to-device>/CIRCUITPY/code.py

    Tested:
    Adafruit KB2040, Unexpected Maker FeatherS3, Unexpected Maker TinyS2

    Source:
    https://gist.github.com/todbot/7534740779cd64a2ce636d14ecb6e5af
    https://learn.adafruit.com/iot-led-sign/circuitpython-internet-test
"""


import wifi
import ipaddress

# scanning for wifi networks
print('\n\nScanning for available WiFi networks...')
print('{:^24s}{:^5s}{:^10s}{:^55s}'.format('SSID', 'RSSI', 'Channels', 'Autentication Mode'))
for ap in wifi.radio.start_scanning_networks():
    print('{:<24s}{:<5d}{:^10d}{:^55s}'.format(ap.ssid, ap.rssi, ap.channel, str(ap.authmode)))
wifi.radio.stop_scanning_networks()

# request wifi network you wish to join
ssid = input('\nWhat is your WiFi SSID: ')
password = input('What is your WiFi password: ')

# join your requested network
print("\nAttempting to join requested network...")
wifi.radio.connect(ssid, password)
print("IP address: ", wifi.radio.ipv4_address)

# check if you are connected by doing a ping
print('\nPerforming ping test...')
ping_ip = ipaddress.IPv4Address("8.8.8.8")
ping = wifi.radio.ping(ip = ping_ip)

# retry ping once more if it timed out
if ping is None:
    ping = wifi.radio.ping(ip = ping_ip)

if ping is None:
    print("Couldn't ping 'google.com' successfully")
else:
    # convert seconds to milliseconds
    print(f"Pinging 'google.com' took: {ping * 1000} ms")

