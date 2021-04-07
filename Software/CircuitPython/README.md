# Enabled-Controller-Mini CircuitPython software setup

  1. Download latest version of [CircuitPython for QT Py UF2](https://circuitpython.org/board/qtpy_m0/)
  2. Plug your QT Py into your computer using a known-good USB cable
  3. Double-click the small RST (reset) button, and you will see the NeoPixel RGB LED turn green
  4. You will see a new disk drive appear called QTPY_BOOT under Device and Drivers 
  5. Drag the adafruit_circuitpython_etc.uf2 file to QTPY_BOOT
  6. The red LED will flash. Then, the QTPY_BOOT drive will disappear and a new disk drive called CIRCUITPY will appear
  7. Replace the code.py under CIRCUITPY drive with Enabled-Controller-Mini USB [code.py](https://raw.githubusercontent.com/milador/Enabled-Controller-Mini/main/Software/CircuitPython/Enabled_Controller_Mini_USB_Software/code.py)
  8. Drag and drop required libraries under CIRCUITPY drive in lib folder. [Required libraries](https://github.com/milador/Enabled-Controller-Mini/tree/main/Software/CircuitPython/Enabled_Controller_Mini_USB_Software/lib)
  9. The led on Enabled-Controller-Mini should turn to teal color
