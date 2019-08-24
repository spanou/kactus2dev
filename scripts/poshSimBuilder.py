import sys

def printMessage():
    print("")
    print("-----------------------------------------")
    print("Running the POSH Simulation Generator....")
    print("Arguments Count: ", len(sys.argv))
    i=0;
    for x in sys.argv:
        print("Argument[", i ,"]: ", x)
        i += 1
    print("-----------------------------------------")

def main():
    printMessage();

if __name__ == "__main__":
    main()
