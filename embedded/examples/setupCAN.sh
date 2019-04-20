#!/bin/bash

CAN_INTF=vcan
INTF_NAME=${CAN_INTF}0

# Set this if we have can0, otherwise leave empty
BITRATE= #bitrate 250000

# Run with sudo access!
# Setup CAN
sudo ip link add ${INTF_NAME} type ${CAN_INTF} ${BITRATE} 2>/dev/null
sudo ifconfig ${INTF_NAME} up


$(ifconfig ${INTF_NAME} | grep -q "RUNNING")
if [ "$?" = 0 ]; then
    echo "Interface appears to be correct"
fi

cangen -n 5 ${INTF_NAME} &
candump -T 1000 ${INTF_NAME}

echo "If you saw any CAN messages printed, your interface is setup!"
