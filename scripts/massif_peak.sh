#!/bin/bash
ms_print "$1" | grep '^ \+[0-9]\+ ' | awk '{print $3}' | sed 's/,//g' | \
  sort -g | tail -1
