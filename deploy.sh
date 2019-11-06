#!/bin/bash


mkdir build && cd build
cmake ..
retVal=$?
if [ $retVal -ne 0 ]; then
	echo "Failure detected during cmake."
	exit 1
fi
make
retVal=$?
if [ $retVal -ne 0 ]; then
	echo "Failure detected during make."
	exit 1
fi
exit 0
