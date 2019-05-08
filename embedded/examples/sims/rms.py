#!/usr/bin/python3.6

import time
import subprocess as sp
from canUtil import *



def sendTorqueModeActive(intf):
    mesg = mesg.send(intf)

# LSB first, may need to be switched. All values are 2 bytes
def buildMesgs():
    canMesgs = []
    ## Temperatures, PhaseA IGBT: 30C, Gate driver board: 31C
    canMesgs.append(CAN_Mesg(canId=0x0A0,
        rawData=[0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x36, 0x01]))

    ## Temp Gate Driver: 32C
    canMesgs.append(CAN_Mesg(canId=0x0A1,
        rawData=[0x40, 0x01, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0]))

    ## 0,1 Electrical Angle: 50 degrees; 2, 3 RPM of motor: 6000
    canMesgs.append(CAN_Mesg(canId=0x0A5,
        rawData=[0xF4, 0x01, 0x70, 0x17, 0x00, 0x00, 0x00, 0x00]))
    
    ## TorqueModeActive
    canMesgs.append(CAN_Mesg(canId=0x0AA, 
        rawData=[0x0, 0x04, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80]))
    
    ## 0,1 Phase A Current: 200; 6,7 DC Bus Current Calc: 10
    canMesgs.append(CAN_Mesg(canId=0xA6,
        rawData=[0xD0, 0x07, 0x0, 0x0, 0x0, 0x0, 0x0A, 0x0]))
    
    ## 0,1 DC Bus voltage: 280; 2,3 Output Voltage peak to neutral: 250; 
    ## 4,5 Vd voltage w/inverter en.: 220; 6,7 Vq voltage w/inverter en.: 190;
    canMesgs.append(CAN_Mesg(canId=0xA7,
        rawData=[0xF0, 0x0A, 0xC4, 0x09, 0x98, 0x08, 0x6C, 0x07]))

    ## 2,3 flux feedback: 3; 4,5: ld current feedback: 3;
    ## 6,7 lq current feedback: 4;
    canMesgs.append(CAN_Mesg(canId=0xA8,
        rawData=[0x0, 0x0, 0xBB, 0x08, 0x1E, 0x0, 0x28, 0x0]))

    ## 6,7 12V Sys Voltage: 13;
    canMesgs.append(CAN_Mesg(canId=0xA9,
        rawData=[0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x14, 0x05]))
    ## Fault Codes: (NOT SET HERE)
    #canMesgs.append(CAN_Mesg(canId=0xAB,

    ## 0,1 Command Torque: 8; 2,3 Torque Feedback: 6;
    canMesgs.append(CAN_Mesg(canId=0xAC,
        rawData=[0x50, 0x00, 0x3C, 0x0, 0x0, 0x0, 0x0, 0x0]))

    ## 4,5 D-Axis Current Commanded: 50; 6,7 Q-Axis Current cmded: 49;
    canMesgs.append(CAN_Mesg(canId=0xAD,
        rawData=[0x0, 0x0, 0x0, 0x0, 0xF4, 0x01, 0xEA, 0x01]))
   
    return canMesgs

def main():
    intf = findIntf()
    mesgs = buildMesgs()
    c = 0
    while c < 50:
        for mesg in mesgs:
            mesg.send(intf)
        c = c + 1
        time.sleep(1)

if __name__ == "__main__":
    main()
