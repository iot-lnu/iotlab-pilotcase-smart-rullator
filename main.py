from m5stack import *
from m5stack_ui import *
from uiflow import *
import socket
import time
import unit
from math import radians, cos, sin, asin, sqrt

screen = M5Screen()
screen.clean_screen()
screen.set_screen_bg_color(0xFFFFFF)
gps_0 = unit.get(unit.GPS, unit.PORTC)

label0 = M5Label('label0', x=25, y=35, color=0x000, font=FONT_MONT_14, parent=None)
label1 = M5Label('label1', x=25, y=66, color=0x000, font=FONT_MONT_14, parent=None)
label2 = M5Label('label2', x=26, y=100, color=0x000, font=FONT_MONT_14, parent=None)
label3 = M5Label('label3', x=26, y=133, color=0x000, font=FONT_MONT_14, parent=None)
label4 = M5Label('Distance (m)', x=25, y=166, color=0x000, font=FONT_MONT_14, parent=None)
label5 = M5Label('label5', x=128, y=166, color=0x000, font=FONT_MONT_14, parent=None)

#udpsocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#udpsocket.connect(('192.168.50.64', 1337))

def distance(lat1, lat2, lon1, lon2):

    # The math module contains a function named
    # radians which converts from degrees to radians.
    lon1 = radians(lon1)
    lon2 = radians(lon2)
    lat1 = radians(lat1)
    lat2 = radians(lat2)

    # Haversine formula
    dlon = lon2 - lon1
    dlat = lat2 - lat1
    a = sin(dlat / 2)**2 + cos(lat1) * cos(lat2) * sin(dlon / 2)**2

    c = 2 * asin(sqrt(a))

    # Radius of earth in kilometers. Use 3956 for miles
    r = 6371

    # calculate the result
    return(c * r)

def average_long_lat():
  lats = []
  longs = []
  while not (gps_0.latitude and gps_0.longitude):
    pass

  for i in range(30):
    lat_dd = gps_0.latitude[0:2]
    lat_mm = gps_0.latitude[2:-1]
    lat_dir = gps_0.latitude[-1:]

    lon_dd = gps_0.longitude[0:3]
    lon_mm = gps_0.longitude[3:-1]
    lon_dir = gps_0.longitude[-1:]


    lat_dec = lat_dd + " " + lat_mm + " " +  lat_dir
    lon = lon_dd + " " + lon_mm + " " +  lon_dir

    lat_dec = (float(lat_dd) + float(lat_mm)/60)
    lon_dec = (float(lon_dd) + float(lon_mm)/60)
    lats.append(lat_dec)
    longs.append(lon_dec)
    wait(1)


  return sum(lats) / len(lats), sum(longs) / len(longs)


old_lat = -99
old_long = -99

total_dist = 0

while True:

  lats, longs = average_long_lat()

  label0.set_text(str(gps_0.gps_time))
  label2.set_text(str(lats))
  label1.set_text(str(longs))
  label3.set_text(str(gps_0.pos_quality))

  if old_long != -99:
    dist = distance(old_lat, lats, old_long, longs)

    dist *= 1000;
    if dist > 5:
      total_dist += dist
    label5.set_text(str(total_dist))

  old_lat = lats
  old_long = longs

    #obj = str(lat_dec) + " " + str(lon_dec)
    #udpsocket.send(obj)
