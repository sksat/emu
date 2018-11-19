#!/bin/bash

for b in `ls test/*.bin`; do
	make run BIN=$b >& /dev/null
	if [ $? != 0 ]; then
		echo -ne "[ \e[31mfailed\e[m ]  "
	else
		echo -ne "[ \e[32mok\e[m ]      "
	fi
	echo -e "$b\t"
done
