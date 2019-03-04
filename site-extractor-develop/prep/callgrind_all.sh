#!/bin/sh

for filename in ~/Documents/wireshark/GUILLAUME-downloaded-pcaps/sample_pcap/pcap/*
do
  valgrind --tool=callgrind --dump-instr=yes --collect-jumps=yes /dev/shm/none-valgrind/.libs/lt-tshark -nVr $filename 1>/dev/null
done;