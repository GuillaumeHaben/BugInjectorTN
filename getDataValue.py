import json
import os
 
#create a gdb shell files to analyze UC variables
def createDataValue(varinfo):
    dtvFile = open("dataValue.sh", "w")
    dtvFile.write("#!/bin/sh \n")
    for var in varinfo:
        name= var[0]
        lines= var[1]
        for line in lines:
            dtvFile.write("break " + line + "\n")
    dtvFile.write("start \n continue")
    for var in varinfo:
        name= var[0]
        lines= var[1]
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
        varname=dico["name"]
        varlines=dico["lines"]
        info.append([varname,varlines])
    return info

def main():    
    listvar=get_vars_info_json()
    print(listvar)
    createDataValue(listvar)
    return 0

main()
