#!/usr/bin/python3.6

import subprocess as sp
import os

class CAN_Mesg:
    def __init__(self, canId, rawData):
        self.canId = format(canId, 'x')
        while len(self.canId) < 3:
            self.canId = "0{}".format(self.canId)

        if len(self.canId) > 3:
            print("Warning, likely an invalid message, ID needs 3 numbers")
        data = []
        for i, val in enumerate(rawData):
            data.append(format(val, 'x'))
            if len(data[i]) == 1:
                data[i] = "0{}".format(data[i])
            elif len(data[i]) > 2:
                print("Each data byte should be just a byte!")

        self.data  = ".".join(data)

    def getCanId(self):
        return self.canId

    def setCanId(self, canId):
        self.canId = canId

    def getData(self):
        return self.data

    def setData(self, data):
        self.data = data

    def __str__(self):
        return "{}#{}".format(self.canId, self.data)
    
    def __repr__(self):
        return self.__str__()

    def send(self, intf):
        sp.check_output("cansend {} {}".format(intf, self), shell=True)

def findIntf():
    intf = "NONE"
    with open("/proc/net/dev", "r") as nets:
        for line in nets:
            for word in line.split():
                # We prefer CAN interface, but if all we have is
                # VCAN we will make due
                if "vcan" in word:
                    intf = word
                elif "can" in word:
                    intf = word.replace(':', '')
                    print("Using {}".format(intf))          
                    return intf

    intf = intf.replace(':', '')
    print("Using {}".format(intf))
    return intf

def main():
    canIntf = findIntf()    

if __name__ == "__main__":
    main()
