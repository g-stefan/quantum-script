@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

set ACTION=%1
if "%1" == "" set ACTION=make

echo -^> %ACTION% quantum-script

goto StepX
:cmdX
%*
if errorlevel 1 goto cmdXError
goto :eof
:cmdXError
echo "Error: %ACTION%"
exit 1
:StepX

call :cmdX file-to-cs --touch=source/quantum-script-libstdarray.cpp --file-in=source/quantum-script-libstdarray.js --file-out=source/quantum-script-libstdarray.src --is-string --name=libStdArraySource
call :cmdX file-to-cs --touch=source/quantum-script-libstderror.cpp --file-in=source/quantum-script-libstderror.js --file-out=source/quantum-script-libstderror.src --is-string --name=libStdErrorSource
call :cmdX file-to-cs --touch=source/quantum-script-libstdfunction.cpp --file-in=source/quantum-script-libstdfunction.js --file-out=source/quantum-script-libstdfunction.src --is-string --name=libStdFunctionSource
call :cmdX file-to-cs --touch=source/quantum-script-libstdobject.cpp --file-in=source/quantum-script-libstdobject.js --file-out=source/quantum-script-libstdobject.src --is-string --name=libStdObjectSource
call :cmdX file-to-cs --touch=source/quantum-script-libstdscript.cpp --file-in=source/quantum-script-libstdscript.js --file-out=source/quantum-script-libstdscript.src --is-string --name=libStdScriptSource

call :cmdX xyo-cc --mode=%ACTION% @util/quantum-script.static.compile.info
call :cmdX xyo-cc --mode=%ACTION% @util/quantum-script.dynamic.compile.info

call :cmdX xyo-cc --mode=%ACTION% @util/quantum-script-extension-console.static.compile.info
call :cmdX xyo-cc --mode=%ACTION% @util/quantum-script-extension-console.dynamic.compile.info

call :cmdX xyo-cc --mode=%ACTION% @util/quantum-script.compile.info
