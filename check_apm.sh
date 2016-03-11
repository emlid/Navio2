#!/bin/bash 

if  ps -AT | grep -c "sched-timer" > /dev/null
then
  echo "APM is running. Can't launch the example"
  exit 1
else
  exit 0
fi
