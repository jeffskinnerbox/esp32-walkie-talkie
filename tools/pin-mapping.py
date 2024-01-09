
"""
    CircuitPython Pin Map Script.

    Description:
    This script will list the mapping of physical board pins to what those
    pins can be called in a CircuitPython script.

    Usage:
    ampy --port /dev/ttyACM0 run pin-mapping.py

    Tested:
    Adafruit KB2040, Unexpected Maker FeatherS3

    Source:
    https://github.com/adafruit/Adafruit_Learning_System_Guides/blob/main/CircuitPython_Essentials/Pin_Map_Script/code.py
"""


import microcontroller
import board
try:
    import cyw43       # raspberry pi
except ImportError:
    cyw43 = None

board_pins = []
for pin in dir(microcontroller.pin):
    if (isinstance(getattr(microcontroller.pin, pin), microcontroller.Pin) or
        (cyw43 and isinstance(getattr(microcontroller.pin, pin), cyw43.CywPin))):
        pins = []
        for alias in dir(board):
            if getattr(board, alias) is getattr(microcontroller.pin, pin):
                pins.append(f"board.{alias}")
        # Add the original GPIO name, in parentheses.
        if pins:
            # Only include pins that are in board.
            pins.append(f"({str(pin)})")
            board_pins.append(" ".join(pins))

for pins in sorted(board_pins):
    print(pins)

