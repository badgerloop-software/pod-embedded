#!/usr/bin/python3.6

import time
import subprocess as sp
from canUtil import *


def buildMesgs():
    canMesgs = []

    canMesgs.append(CAN_Mesg(canId=0x6B0,
        rawData=[0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0]))

    return canMesgs


def main():
    mesgs = buildMesgs()
    
    for mesg in mesgs:
    


if __name__ == "__main__":
    main()
