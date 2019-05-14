#!/bin/bash

sudo modprobe vcan
./embedded/examples/setupCAN.sh
make examples VIRTUAL=1
make VIRTUAL=1
python3 ./embedded/examples/sims/rms.py &
./out/tests/can_test

