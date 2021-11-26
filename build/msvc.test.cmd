@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo - %BUILD_PROJECT% ^> test

goto cmdXDefined
:cmdX
%*
if errorlevel 1 goto cmdXError
goto :eof
:cmdXError
echo "Error: %ACTION%"
exit 1
:cmdXDefined

for /L %%i in (1,1,9) do call :cmdX output\bin\quantum-script --execution-time test/test.000%%i.js
for /L %%i in (10,1,36) do call :cmdX output\bin\quantum-script --execution-time test/test.00%%i.js
