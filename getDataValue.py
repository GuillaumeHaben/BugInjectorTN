import json
import os
 
#create a gdb shell files to analyze UC variables
def createDataValue(varinfo):
    for var in varinfo:
        function= var[0]
        name= var[1]
        lines= var[2]
        dtvFile = open(function + "_" + name + "_dataValue.sh", "w")
        dtvFile.write("#!/bin/sh \n")
        for line in lines:
            intLine = int(line) + 1
            dtvFile.write("break " + str(intLine) + "\n")
        dtvFile.write("start \n continue")
        for line in range(len(lines)):
            dtvFile.write("\n print " + name + "\n continue" )
        dtvFile.close()
    return 0


#get the UC variable and where the lines where it's used in a json format
def get_vars_info_json():
    info=[]
    F = open("output_trace.json","r").read()
    data = json.loads(F)
    vars = data["data"]
    for v in vars:
        var=v["variable"]
        dico=var[0]
        funname=v["function"]
        varname=dico["name"]
        varlines=dico["lines"]
        info.append([funname,varname,varlines])
    return info

def main():    
    listvar=get_vars_info_json()
    createDataValue(listvar)
    return 0

main()
