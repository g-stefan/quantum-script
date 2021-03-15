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

cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstdarray.cpp --file-in=source/quantum-script.lib/quantum-script-libstdarray.js --file-out=source/quantum-script.lib/quantum-script-libstdarray.src --is-string --name=libStdArraySource
cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstderror.cpp --file-in=source/quantum-script.lib/quantum-script-libstderror.js --file-out=source/quantum-script.lib/quantum-script-libstderror.src --is-string --name=libStdErrorSource
cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstdfunction.cpp --file-in=source/quantum-script.lib/quantum-script-libstdfunction.js --file-out=source/quantum-script.lib/quantum-script-libstdfunction.src --is-string --name=libStdFunctionSource
cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstdobject.cpp --file-in=source/quantum-script.lib/quantum-script-libstdobject.js --file-out=source/quantum-script.lib/quantum-script-libstdobject.src --is-string --name=libStdObjectSource
cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstdscript.cpp --file-in=source/quantum-script.lib/quantum-script-libstdscript.js --file-out=source/quantum-script.lib/quantum-script-libstdscript.src --is-string --name=libStdScriptSource

cmdX xyo-cc --mode=$ACTION @build/source/quantum-script.static.compile
cmdX xyo-cc --mode=$ACTION @build/source/quantum-script.dynamic.compile

cmdX xyo-cc --mode=$ACTION @build/source/quantum-script-extension-console.static.compile
cmdX xyo-cc --mode=$ACTION @build/source/quantum-script-extension-console.dynamic.compile

cmdX xyo-cc --mode=$ACTION @build/source/quantum-script.compile
