#!/bin/sh

set -e

io=2

cd /sys/class/gpio
echo $io > export
echo out > gpio$io/direction

trap "echo $io > unexport; exit" 2

while true
do
	echo 0 > gpio$io/value
	echo 1 > gpio$io/value
done
