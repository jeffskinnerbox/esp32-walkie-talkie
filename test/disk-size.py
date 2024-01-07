
"""
    Print the disk size and free space on the CIRCUITPY drive.

    Usage:
    ampy --port /dev/ttyACM0 run disk-size.py

    Tested:
    Adafruit KB2040, Unexpected Maker FeatherS3

    Source:
    https://github.com/todbot/circuitpython-tricks/tree/main?tab=readme-ov-file#get-circuitpy-disk-size-and-free-space
"""


import os

fs_stat = os.statvfs('/')
print("Disk size in MB: ", fs_stat[0] * fs_stat[2] / 1024 / 1024)
print("Free space in MB: ", fs_stat[0] * fs_stat[3] / 1024 / 1024)

