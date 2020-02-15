#!/bin/bash

fileName=${0##*/}
if [ "$#" -ne 1 ]; then
	echo "one argument expected after './$fileName'"
	exit 1
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
elif [ "$1" == "cross-setup" ]; then
	# This part of the script is not checked for errors
	cd /tmp/
	
	wget -c https://releases.linaro.org/components/toolchain/binaries/6.5-2018.12/arm-linux-gnueabihf/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf.tar.xz
	
	sudo tar xf gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf.tar.xz -C /opt/
	
	rm gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf.tar.xz

	echo "cross build setup finished"
elif [ "$1" == "cross" ]; then
	mkdir build && cd build

	cmake -DCMAKE_TOOLCHAIN_FILE=../toolchains/beaglebone.cmake ../
	retVal=$?
	if [ $retVal -ne 0 ]; then
		echo "Failure detected during cmake (cross compiling). You might want to run './$fileName cross-setup' first."
		exit 1
	fi

	make
	retVal=$?
	if [ $retVal -ne 0 ]; then
		echo "failure detected during make"
		exit 1
	fi
	echo "cross build finished"
elif [ "$1" == "clean" ]; then
	rm -rf build
	rm -rf out
	echo "clean finished"
else
	echo "Invalid argument: please put it in the form './$fileName build' or './$fileName cross-setup' or './$fileName cross' or './$fileName clean'"
	exit 1
fi
exit 0
