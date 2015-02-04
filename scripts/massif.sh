#!/bin/bash
OUT="$1"
shift
valgrind --tool=massif --massif-out-file="$OUT" --ignore-fn=mallocate --stacks=yes \
  --ignore-fn='std::basic_filebuf<char, std::char_traits<char> >::open(char const*, std::_Ios_Openmode)' \
  "$@"
