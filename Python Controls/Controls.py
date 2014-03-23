__author__ = 'Voxvalve'

import serial
import time
from datetime import datetime
from __future__ import print_function

ser = serial.Serial('/dev/ttyACM0', 9600)

while 1:
    now = datetime.now().strftime('%H:%M')

    if now == "22:00":
        ser.write('2')
    elif now == "10:00":
        ser.write('3')
    else:
        ser.write('1')
        time.sleep(1000)
        temp = ser.readline()



