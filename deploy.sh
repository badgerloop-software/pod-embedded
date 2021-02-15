#!/bin/bash

fileName=${0##*/}
if [ "$#" -ne 1 ]; then
	echo "one argument expected after './$fileName'"
	echo "Options are: build, cross-setup, cross, clean, gtest-setup, setup"
	exit 1
elif [ "$1" == "build" ]; then
    ./deploy.sh clean
    ./deploy.sh gtest-setup
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
	cd /tmp/
	wget -c https://releases.linaro.org/components/toolchain/binaries/6.5-2018.12/arm-linux-gnueabihf/gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf.tar.xz
	if [ $? -ne 0 ]; then
		echo "Failure to get arm-linux-gnueabihf"
	fi
	sudo tar xf gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf.tar.xz -C /opt/

	rm gcc-linaro-6.5.0-2018.12-x86_64_arm-linux-gnueabihf.tar.xz

	echo "cross build setup finished"
elif [ "$1" == "cross" ]; then
    ./deploy.sh clean
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

elif [ "$1" == "gtest-setup" ]; then
	echo "Building GTest"
	cd /usr/src/gtest
	sudo cmake CMakeLists.txt
	sudo make
	sudo cp *.a /usr/lib
	
	echo "GTest Install Complete"

elif [ "$1" == "setup" ]; then
	echo "Installing Dependencies (Assumes Debian based System) - Sudo access required"
	sudo apt-get update
	sudo apt-get install -y build-essential
	sudo apt-get install -y cmake libgtest-dev
    sudo apt-get install -y python3 python3-pip python-is-python3 clang-format
	
	if [ $? -ne 0 ]; then
		echo "failure detected during dependency installation"
		exit 1
	fi
	echo "setup finished"

elif [ "$1" == "format" ]; then
	find . -regex '.*\.\(cpp\|hpp\|cu\|c\|h\)' -exec clang-format -style=file -i {} \;
	echo "Format successful"
elif [ "$1" == "clean" ]; then
	rm -rf build
	rm -rf out
	echo "clean finished"
else
	echo "Invalid argument: please put it in the form './$fileName build' or './$fileName cross-setup' or './$fileName cross' or './$fileName clean'"
	exit 1
fi
exit 0
