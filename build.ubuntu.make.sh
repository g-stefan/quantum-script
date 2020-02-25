#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

ACTION=$1
if [ "$ACTION" = "" ]; then
	ACTION=make
fi

echo "-> $ACTION quantum-script"

cmdX(){
	if ! "$@" ; then
		echo "Error: $ACTION"
		exit 1
	fi
}

cmdX file-to-cs --touch=source/quantum-script-libstdarray.cpp --file-in=source/quantum-script-libstdarray.js --file-out=source/quantum-script-libstdarray.src --is-string --name=libStdArraySource
cmdX file-to-cs --touch=source/quantum-script-libstderror.cpp --file-in=source/quantum-script-libstderror.js --file-out=source/quantum-script-libstderror.src --is-string --name=libStdErrorSource
cmdX file-to-cs --touch=source/quantum-script-libstdfunction.cpp --file-in=source/quantum-script-libstdfunction.js --file-out=source/quantum-script-libstdfunction.src --is-string --name=libStdFunctionSource
cmdX file-to-cs --touch=source/quantum-script-libstdobject.cpp --file-in=source/quantum-script-libstdobject.js --file-out=source/quantum-script-libstdobject.src --is-string --name=libStdObjectSource
cmdX file-to-cs --touch=source/quantum-script-libstdscript.cpp --file-in=source/quantum-script-libstdscript.js --file-out=source/quantum-script-libstdscript.src --is-string --name=libStdScriptSource

cmdX xyo-cc --mode=$ACTION @util/quantum-script.static.compile.info
cmdX xyo-cc --mode=$ACTION @util/quantum-script.dynamic.compile.info

cmdX xyo-cc --mode=$ACTION @util/quantum-script-extension-console.static.compile.info
cmdX xyo-cc --mode=$ACTION @util/quantum-script-extension-console.dynamic.compile.info

cmdX xyo-cc --mode=$ACTION @util/quantum-script.compile.info
