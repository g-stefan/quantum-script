@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> test quantum-script

goto StepX
:cmdX
%*
if errorlevel 1 goto cmdXError
goto :eof
:cmdXError
echo "Error: test"
exit 1
:StepX

for /L %%i in (1,1,9) do call :cmdX bin\quantum-script --execution-time test/test.000%%i.js
for /L %%i in (10,1,36) do call :cmdX bin\quantum-script --execution-time test/test.00%%i.js
