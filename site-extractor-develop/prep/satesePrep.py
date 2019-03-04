#!/usr/bin/python

import sys
import glob
import os

inputData = '/home/guillaume/Desktop/callgrind/occurencesFunctionsWireshark.txt'
outputData = '/home/guillaume/Desktop/callgrind/interestingFunctionsForSatese.txt'

f = open(inputData,"r")
lines = f.readlines()
f.close()


f = open(outputData,"w")

for line in lines:
	line = line.rstrip()
	path = line.split(',')[0]
	function = line.split(',')[1]
	fileName = path.split('dissectors/')[1]
	output = fileName + ',' + function + '\n'
	f.write(output)
f.close()
