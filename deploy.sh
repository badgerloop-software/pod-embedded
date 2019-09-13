#!/bin/bash

sudo modprobe vcan
./embedded/utils/setupCAN.sh
make examples VIRTUAL=1
retVal=$?
if [ $retVal -ne 0 ]; then
	exit 1
fi
make VIRTUAL=1
retVal=$?
if [ $retVal -ne 0 ]; then
	exit 1
fi
python3 ./embedded/examples/sims/rms.py &
./out/tests/can_test
exit 0
