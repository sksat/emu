#!/bin/bash
emu=$1
bin=$2
log=$3

$emu $bin >& $log
if [ $? != 0 ]; then
	echo -e "[ \e[31mfailed\e[m ] $bin"
else
	echo -e "[ \e[32m  ok  \e[m ] $bin"
fi
