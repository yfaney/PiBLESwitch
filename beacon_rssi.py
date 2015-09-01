#!/usr/bin/python

import sys

AVG_FILE = "/var/ramdrive/beacon_avg_data.data"
MAX_QUEUE_SIZE = 10
MAX_RSSI = 218.0
MIN_RSSI = 162.0
THRESHOLD_PERCENT_LOW = 25.0
THRESHOLD_PERCENT_HIGH = 35.0

avg_data = []

try:
    file = open(AVG_FILE, "r")
    for line in file:
        avg_data.append(float(line))
    file.close()
except:
    avg_data = []

if(len(sys.argv)<2):
    print ("Argument needed")
    sys.exit(2)

current_rssi = float(int(sys.argv[1],16))
avg_data.append(current_rssi)
if(len(avg_data) >= MAX_QUEUE_SIZE):
    avg_data.pop(0)

avg_rssi = sum(avg_data)/len(avg_data)
rssi_perc = (avg_rssi - MIN_RSSI)/(MAX_RSSI-MIN_RSSI) * 100.0
#print (rssi_perc)
try:
    file = open(AVG_FILE, "w")
    for item in avg_data:
        file.write(str(item) + "\n")
    file.close()
except:
    print "ERROR!"

if (rssi_perc < THRESHOLD_PERCENT_LOW):
    sys.exit(-1)
elif (rssi_perc > THRESHOLD_PERCENT_HIGH):
    sys.exit(1)
else:
    sys.exit(0)
