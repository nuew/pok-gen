#!/usr/bin/env bash
# Formats hexdump input from pokégen.c for use in gdb.

# Take the arguemnt as the position to set, or default to the second slot
pos=$1
[ "$pos" == "" ] && pos="0x030043c4"

printf "set { char [100] } %s = { " $pos
cut -d " " -f 2-17 |
paste -d " " -s |
sed "s/\(..\) /0x\1, /g;s/, 0x  , 0x  , 0x  ,   $/ }/"
