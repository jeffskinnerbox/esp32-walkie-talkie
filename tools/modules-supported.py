"""
    CircuitPython What Modules are Supported by Board.

    Description:
    This script will list the mapping of physical board pins to what those
    pins can be called in a CircuitPython script.

    Usage:
    ampy --port /dev/ttyACM0 run modules-supported.py
       or
    echo "help('modules')" > junk ; ampy --port /dev/ttyACM0 run junk

    Tested:
    Adafruit KB2040, Unexpected Maker FeatherS3

    Source:
    https://github.com/adafruit/Adafruit_Learning_System_Guides/blob/main/CircuitPython_Essentials/Pin_Map_Script/code.py
"""


help('modules')

