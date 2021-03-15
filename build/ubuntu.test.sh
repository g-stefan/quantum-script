#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

echo "-> test quantum-script"

cmdX(){
	if ! "$@" ; then
		echo "Error: test"
		exit 1
	fi
}

for i in $(seq 1 9)
do
	cmdX output/bin/quantum-script --execution-time test/test.000$i.js
done
for i in $(seq 10 36)
do
	cmdX output/bin/quantum-script --execution-time test/test.00$i.js
done
