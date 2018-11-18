#!/bin/bash

for b in `ls test/*.bin`; do
	echo -ne "$b\t"
	make run BIN=$b >& /dev/null
	if [ $? != 0 ]; then
		echo "failed"
	else
		echo "ok"
	fi
done
