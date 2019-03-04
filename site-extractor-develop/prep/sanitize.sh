#!/bin/sh

#Sanitize the file by removing duplicate lines, ordering them and putting cmd 

OUTPUT="$1"

sort -ur $OUTPUT > $OUTPUT.clean
