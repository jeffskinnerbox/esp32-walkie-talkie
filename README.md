


For any given ESP board, there is only one firmware package to download
but there are many types to choose from.
To find your firmware, go to CircuitPython's [Downloads page][21],
and select from the many boards listed there.

If you are putting MicroPython on your board
then you should first erase the entire flash and then load the firmwatre with `esptool.py`


```bash
# check type of board, flash size, etc.
esptool.py --port /dev/ttyUSB0 flash_id
esptool.py --port /dev/ttyACM0 flash_id

# erease the entire flash on the esp32
esptool.py --chip esp32-c3 --port /dev/ttyACM0 erase_flash
esptool.py --chip esp32 --port /dev/ttyACM0 erase_flash  <-- I used "OPEN INSTALLER" at https://circuitpython.org/board/unexpectedmaker_tinys2/

# flash the firmware starting at address 0x0 (different for micropython)
esptool.py --chip esp32-c3 --port /dev/ttyACM0 --baud 460800 write_flash -z 0x0 ~/Downloads/circuitpython/adafruit-circuitpython-seeed_xiao_esp32c3-en_US-8.2.9.bin
esptool.py --chip esp32 --port /dev/ttyACM0 --baud 460800 write_flash -z 0x0 ~/Downloads/circuitpython/adafruit-circuitpython-unexpectedmaker_tinys2-en_US-8.2.9.bin  <-- I used "OPEN INSTALLER" at https://circuitpython.org/board/unexpectedmaker_tinys2/

# contents of file system (assume you rebooted the ESP32)
ampy --port /dev/ttyACM0 ls -l

# upload and execute file pin-mapping.py
ampy --port /dev/ttyACM0 run ~/Downloads/circuitpython/pin-mapping.py

# serial terminal into the esp32
screen /dev/ttyACM0 115200,cs8cls
```

To get the CircuitPython to board pin mapping, we will need to execute the script below, I call `pin-mapping.py`:

```python
# SPDX-FileCopyrightText: 2020 anecdata for Adafruit Industries
# SPDX-FileCopyrightText: 2021 Neradoc for Adafruit Industries
# SPDX-FileCopyrightText: 2021-2023 Kattni Rembor for Adafruit Industries
# SPDX-FileCopyrightText: 2023 Dan Halbert for Adafruit Industries
#
# SPDX-License-Identifier: MIT

"""CircuitPython Essentials Pin Map Script"""
import microcontroller
import board
try:
    import cyw43  # raspberrypi
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
```

So how do you find out what modules are available for your board?

```bash
# execuse a module listing
echo "help('modules')" > junk ; ampy --port /dev/ttyACM0 run junk

# to find out more about a specific module
echo "help('modules.wifi')" > junk ; ampy --port /dev/ttyACM0 run junk
```




[21]:https://circuitpython.org/downloads

