#!/bin/bash

fileName=${0##*/}
if [ "$#" -ne 1 ]; then
	echo "one argument expected after './$fileName'"
elif [ "$1" == "build" ]; then
	mkdir build && cd build
	
	cmake ..
	retVal=$?
	if [ $retVal -ne 0 ]; then
		echo "failure detected during cmake"
		exit 1
	fi
	
	make
	retVal=$?
	if [ $retVal -ne 0 ]; then
		echo "failure detected during make"
		exit 1
	fi
	echo "build finished"
elif [ "$1" == "clean" ]; then
	rm -rf build
	rm -rf out
	echo "clean finished"
else
	echo "Invalid argument: please put it in the form './$fileName build' or './$fileName clean'"
fi
exit 0
