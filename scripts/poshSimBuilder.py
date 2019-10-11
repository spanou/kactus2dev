#!/usr/bin/python3
import sys
import re


def getArgumentList():
    if len(sys.argv) < 2:
        return None

    args = ""

    for x in sys.argv[1:]:
        args = args +  " " + x

    regExp = r"-{1,2}\w*\s*=*\s*[\w|\d|\s|\/|.]*"
    x = re.findall(regExp, args)
    return x

def printArgumentList(argList):
    if(None == argList):
        return

    argCount = 1;
    for arg in argList:
        print( "Argument[" + str(argCount) + "] :" + arg)
        argCount = argCount + 1

def printMessage():

    maxOutputLen = 80
    print("")
    print("-" * maxOutputLen)
    banner = "Running the POSH Simulation Generator"
    print( banner, end="")

    if len(banner) > maxOutputLen:
        filler = maxOutputLen
    else:
        filler = maxOutputLen - len(banner)

    print ("." * filler)
    print ("Script: " + sys.argv[0])
    printArgumentList(getArgumentList())
    print("-" * maxOutputLen)

def main():
    printMessage();

if __name__ == "__main__":
    main()
