@echo off
rem Public domain
rem http://unlicense.org/
rem Created by Grigore Stefan <g_stefan@yahoo.com>

echo -^> version quantum-script

xyo-cc --mode=version --source-path=source/quantum-script.exe quantum-script
xyo-cc --mode=version --source-path=source/quantum-script.lib quantum-script
xyo-cc --mode=version --source-path=source/quantum-script-extension-console quantum-script-extension-console
