#!/bin/sh
hciconfig hci0 up
hciconfig hci0 leadv 3
hciconfig hci0 noscan
