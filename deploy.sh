#!/bin/bash

mkdir build && cd build
cmake ..
retVal=$?
if [ $retVal -ne 0 ]; then
	exit 1
fi
make
retVal=$?
if [ $retVal -ne 0 ]; then
	exit 1
fi
exit 0
