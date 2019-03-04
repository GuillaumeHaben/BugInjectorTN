#!/usr/bin/python

# This script takes a callgrind.out.**** file as input and format it as follows:
# FileName,FunctionName
# Output is callgrind.out.****.formated

import sys
import glob
import os

path = '/home/guillaume/Desktop/callgrind/'

for filename in glob.glob(os.path.join(path, 'callgrind.out.*')):
	print filename
	f = open(filename,"r")
	lines = f.readlines()
	f.close()

	formatedFile =  filename + ".formated"

	f = open(formatedFile, "a+")
	for i in range(0, len(lines)):
		line = lines[i]
		if line.startswith("cmd"):
			print >>f,line.rstrip().split("-nVr ", 1)[-1]

		if line.startswith("fl=(") and "/dissectors/packet-" in line and "fn=(" in lines[i+1]:
			# rstrip removes newLine
			# split removes beginning of string
			fileName = line.rstrip().split(" ", 1)[-1]
			functionName = lines[i + 1].rstrip().split(" ", 1)[-1]
			result = fileName + "," + functionName
			print >>f,result
	f.close()