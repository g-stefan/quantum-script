@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

set ACTION=%1
if "%1" == "" set ACTION=make

echo - %BUILD_PROJECT% ^> %ACTION%

goto cmdXDefined
:cmdX
%*
if errorlevel 1 goto cmdXError
goto :eof
:cmdXError
echo "Error: %ACTION%"
exit 1
:cmdXDefined

call :cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstdarray.cpp --file-in=source/quantum-script.lib/quantum-script-libstdarray.js --file-out=source/quantum-script.lib/quantum-script-libstdarray.src --is-string --name=libStdArraySource
call :cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstderror.cpp --file-in=source/quantum-script.lib/quantum-script-libstderror.js --file-out=source/quantum-script.lib/quantum-script-libstderror.src --is-string --name=libStdErrorSource
call :cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstdfunction.cpp --file-in=source/quantum-script.lib/quantum-script-libstdfunction.js --file-out=source/quantum-script.lib/quantum-script-libstdfunction.src --is-string --name=libStdFunctionSource
call :cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstdobject.cpp --file-in=source/quantum-script.lib/quantum-script-libstdobject.js --file-out=source/quantum-script.lib/quantum-script-libstdobject.src --is-string --name=libStdObjectSource
call :cmdX file-to-cs --touch=source/quantum-script.lib/quantum-script-libstdscript.cpp --file-in=source/quantum-script.lib/quantum-script-libstdscript.js --file-out=source/quantum-script.lib/quantum-script-libstdscript.src --is-string --name=libStdScriptSource

call :cmdX xyo-cc --mode=%ACTION% @build/source/quantum-script.static.compile
call :cmdX xyo-cc --mode=%ACTION% @build/source/quantum-script.dynamic.compile

call :cmdX xyo-cc --mode=%ACTION% @build/source/quantum-script-extension-console.static.compile
call :cmdX xyo-cc --mode=%ACTION% @build/source/quantum-script-extension-console.dynamic.compile

call :cmdX xyo-cc --mode=%ACTION% @build/source/quantum-script.compile
call :cmdX xyo-cc --mode=%ACTION% @build/source/quantum-scriptw.compile
