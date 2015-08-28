#!/bin/bash
# iBeacon Scan by Radius Networks
if [[ $1 == "parse" ]]; then
  packet=""
  capturing=""
  count=0
  while read line
  do
    count=$[count + 1]
    if [ "$capturing" ]; then
      if [[ $line =~ ^[0-9a-fA-F]{2}\ [0-9a-fA-F] ]]; then
        packet="$packet $line"
      else
        if [[ $packet =~ ^04\ 3E\ 2B\ 02\ 01\ .{35}\ 00\ 11\ 22\ 33\ 44\ 55\ 66 ]]; then
          UUID=`echo $packet | sed 's/^.\{84\}\(.\{53\}\).*$/\1/'`
          if [[ $2 == "-b" ]]; then
            echo "$UUID"
          else
            echo "UUID: $UUID"
          fi
        fi
        capturing=""
        packet=""
      fi
    fi
    if [ ! "$capturing" ]; then
      if [[ $line =~ ^\> ]]; then
        packet=`echo $line | sed 's/^>.\(.*$\)/\1/'`
        capturing=1
      fi
    fi
  done else
  sudo hcitool lescan --duplicates 1>/dev/null &
  if [ "$(pidof hcitool)" ]; then
    sudo hcidump --raw | ./$0 parse $1
  fi
fi
