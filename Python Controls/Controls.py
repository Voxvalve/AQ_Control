__author__ = 'Voxvalve'

import serial
import time
import os.path
import csv
from datetime import datetime
from __future__ import print_function

ser = serial.Serial('/dev/ttyACM0', 9600)
lights = 0
fname = "temp.csv"
time11 = now.replace(hour=11, minute=0, second=0, microsecond=0)
time22 = now.replace(hour=22, minute=0, second=0, microsecond=0)

while 1:
    now = datetime.now()

    #Lights On.
    if now > time11 and now < time22:
        if lights == 0:
            ser.write('2')
            lights = 1
    #Lights off.
    elif now > time22 and now < time11:
        if lights == 1:
            ser.write('3')
            lights = 0

    ser.write('1')
    time.sleep(1000)
    temp = ser.readline()
    # If file exists
    if os.path.isfile(fname):
        with open(fname, 'a', newline='') as fp:
            a = csv.writer(fp, delimiter=',')
            a.writerow([temp, datetime.now()])
    #Create CSV column header first time.
    else:
        with open(fname, 'w', newline='') as fp:
            a = csv.writer(fp, delimiter=',')
            a.writerows(["Temperature", "Date"], [temp, datetime.now()])

    #Wait for a while.
    time.sleep(9000)
