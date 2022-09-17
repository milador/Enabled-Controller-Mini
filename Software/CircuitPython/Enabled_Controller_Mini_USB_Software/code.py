#***************************************************************************
#* File Name: Enabled_Controller_Mini_USB_Software/code.py
#* Title: Enabled Controller Mini USB CircuitPython Code
#* Developed by: Milador
#* Version Number: 1.0 (17/9/2022)
#* Github Link: https://github.com/milador/Enabled-Controller-Mini/tree/main/Software/CircuitPython/Enabled_Controller_Mini_USB_Software
#***************************************************************************/

#Import modules
import time
import board
from digitalio import DigitalInOut, Direction, Pull
import usb_hid
import neopixel
led = neopixel.NeoPixel(board.NEOPIXEL, 1)

#Import modules from the Adafruit hid library
from adafruit_hid.keyboard import Keyboard
from adafruit_hid.keycode import Keycode
from adafruit_hid.mouse import Mouse
from adafruit_hid.consumer_control_code import ConsumerControlCode

#Customize values
led.brightness = 0.3
switch_time_delay = 0.125
switch_debug_mode = False    #To serial print = True , To not serial print = False
switch_operation_mode = 3    #Keyboard = 1, Mouse = 2, Media = 3
switch_mac_mode = False      #To use keys for mac = True , To use keys for other os = False


#Mac mode or other keyboard mode actions
if switch_mac_mode:
    switch_a_keyboard_action = Keycode.F1
    switch_b_keyboard_action = Keycode.F2
    switch_c_keyboard_action = Keycode.F3
    switch_d_keyboard_action = Keycode.F4
else:
    switch_a_keyboard_action = Keycode.A
    switch_b_keyboard_action = Keycode.B
    switch_c_keyboard_action = Keycode.C
    switch_d_keyboard_action = Keycode.D

#Mouse actions
switch_a_mouse_action = Mouse.LEFT_BUTTON
switch_b_mouse_action = Mouse.RIGHT_BUTTON
switch_c_mouse_action = Mouse.MIDDLE_BUTTON
switch_d_mouse_action = "DRAG"

#Media actions
switch_a_media_action = ConsumerControlCode.PLAY_PAUSE
switch_b_media_action = ConsumerControlCode.SCAN_NEXT_TRACK  
switch_c_media_action = ConsumerControlCode.VOLUME_DECREMENT 
switch_d_media_action = ConsumerControlCode.VOLUME_INCREMENT

time.sleep(1)

# Define buttons
button_a = DigitalInOut(board.D6)
button_a.direction = Direction.INPUT
button_a.pull = Pull.UP

button_b = DigitalInOut(board.D7)
button_b.direction = Direction.INPUT
button_b.pull = Pull.UP

button_c = DigitalInOut(board.D3)
button_c.direction = Direction.INPUT
button_c.pull = Pull.UP

button_d = DigitalInOut(board.D2)
button_d.direction = Direction.INPUT
button_d.pull = Pull.UP

#Creates a keyboard instance
keyboard = Keyboard(usb_hid.devices)

# Define colors
led_color_teal = (0, 128, 128)
led_color_blue = (0, 0, 255)
led_color_yellow = (150, 255, 0)
led_color_green = (0, 255, 0)
led_color_red = (255, 0, 0)

#Initial led color
led[0] = led_color_teal

#Setup mouse
mouse = Mouse(usb_hid.devices)
mouse_state = 0

time.sleep(.5)

if switch_debug_mode:
    print("waiting for pin press")
#Main loop that listens to which pin activated and sends a key press
while True:
    if (not button_a.value):
        led[0] = led_color_blue
        if switch_debug_mode:
            print("button_a state:" , button_a.value)
        if (switch_operation_mode == 1):
            keyboard.press(switch_a_keyboard_action)
            keyboard.release_all()
        elif (switch_operation_mode == 2):
            mouse_state = 0
            mouse.click(switch_a_mouse_action)
        elif (switch_operation_mode == 3):
            consumer_control.press(switch_a_media_action)
            consumer_control.release()
    if (not button_b.value):
        led[0] = led_color_yellow
        if switch_debug_mode:
            print("button_b state:" , button_b.value)
        if (switch_operation_mode==1):
            keyboard.press(switch_b_keyboard_action)
            keyboard.release_all()
        elif (switch_operation_mode==2):
            mouse_state = 0
            mouse.click(switch_b_mouse_action)
        elif (switch_operation_mode == 3):
            consumer_control.press(switch_b_media_action)
            consumer_control.release()
    if (not button_c.value):
        led[0] = led_color_green
        if switch_debug_mode:
            print("button_c state:" , button_c.value)
        if (switch_operation_mode == 1):
            keyboard.press(switch_c_keyboard_action)
            keyboard.release_all()
        elif (switch_operation_mode == 2):
            mouse_state = 0
            mouse.click(switch_c_mouse_action)
        elif (switch_operation_mode == 3):
            consumer_control.press(switch_c_media_action)
            consumer_control.release()
    if (not button_d.value):
        led[0] = led_color_red
        if switch_debug_mode:
            print("button_d state:" , button_d.value)
        if (switch_operation_mode == 1):
            keyboard.press(switch_d_keyboard_action)
            keyboard.release_all()
        elif (switch_operation_mode == 2):
            if (mouse_state == 0):
                mouse.press(Mouse.LEFT_BUTTON)
                mouse_state = 1
                time.sleep(0.2)
            else:
                mouse.release_all()
                mouse_state = 0
                time.sleep(0.2)
        elif (switch_operation_mode == 3):
            consumer_control.press(switch_d_media_action)
            consumer_control.release()
    time.sleep(switch_time_delay)
    if (switch_operation_mode == 2 and mouse_state == 1):
        pass
    else:
        led[0] = led_color_teal