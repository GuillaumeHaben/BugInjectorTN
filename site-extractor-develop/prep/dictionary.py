#!/usr/bin/python

import sys
import glob
import os

path = '/home/guillaume/Desktop/callgrind/'
dictionary = {}

for filename in glob.glob(os.path.join(path, '*.formated.clean')):
	f = open(filename,"r")
	lines = f.readlines()
	f.close()

	pcap = lines[0].rstrip()

	for i in range(0, len(lines)):
		#Don't take first line, it's the pcap file
		if i != 0:
			line = lines[i].rstrip()
			#Add to dict
			if line in dictionary:
				dictionary[line][0] += 1
			else:
				dictionary[line] = [1, pcap]

#Sort the dictionary 
s = [(k, dictionary[k]) for k in sorted(dictionary, key=dictionary.get, reverse=True)]

#Print results
for key, value in s:
	result = key + "," + str(value)
	print(result)