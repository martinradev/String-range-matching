#!/bin/bash
echo $1
PEAKSNAP=$(ms_print $1 | grep -o '[0-9]\+ (peak)' | awk '{print $1}')
PEAKMEM=$(ms_print $1 | grep "^ \+$PEAKSNAP " | awk '{print $3}')
echo $PEAKMEM
