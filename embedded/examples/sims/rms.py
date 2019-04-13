#!/usr/bin/python3.6

import time
import subprocess as sp
from canUtil import *



def sendTorqueModeActive(intf):
    mesg = mesg.send(intf)

# Assumes MSB first, may need to be switched. All values are 2 bytes
def buildMesgs():
    canMesgs = []
    ## Temperatures, PhaseA IGBT: 30C, Gate driver board: 31C
    canMesgs.append(CAN_Mesg(canId=0x0A0,
        rawData=[0x01, 0x2C, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2D]))

    ## Temp Gate Driver: 32C
    canMesgs.append(CAN_Mesg(canId=0x0A1,
        rawData=[0x01, 0x2F, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0]))

    ## 0,1 Electrical Angle: 50 degrees; 2, 3 RPM of motor: 6000
    canMesgs.append(CAN_Mesg(canId=0x0A5,
        rawData=[0x01, 0xF4, 0x17, 0x70, 0x00, 0x00, 0x00, 0x00]))
    
    ## TorqueModeActive
    canMesgs.append(CAN_Mesg(canId=0x0AA, 
                    rawData=[0x04, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x0]))
    
    return canMesgs

def main():
    intf = findIntf()
    mesgs = buildMesgs()
    c = 0
    while c < 3:
        for mesg in mesgs:
            mesg.send(intf)
        c = c + 1
        time.sleep(1)

if __name__ == "__main__":
    main()
