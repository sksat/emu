#!/usr/bin/bash

# originaly from https://gist.github.com/hikalium/6b78979187a32a88215716278a011eb0

echo $1
LANG=C file $1 | grep -v -q "UTF-8" && \
	cp $1 $1.org && \
	iconv -f shift-jis -t utf-8 $1.org | \
	sed -e 's/‾/~/g' | \
	sed -e 's/¥/\\/g' > tmp
tr -d '\r' < tmp > $1
rm tmp
