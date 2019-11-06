#!/bin/bash

if [ $1 == 'build']
then

elif [ $1 == 'clean']
then

else
	echo "Invalid argument: please put it in the form "./deploy.sh build" or "./deploy.sh clean""
fi

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
