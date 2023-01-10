// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

messageAction("make.prepare");

runInPath("source/XYO/QuantumScript",function(){
	exitIf(Shell.system("file-to-cs --touch=LibStdArray.cpp --file-in=LibStdArray.js --file-out=LibStdArray.Source.cpp --is-string --name=libStdArraySource"));
	exitIf(Shell.system("file-to-cs --touch=LibStdError.cpp --file-in=LibStdError.js --file-out=LibStdError.Source.cpp --is-string --name=libStdErrorSource"));
	exitIf(Shell.system("file-to-cs --touch=LibStdFunction.cpp --file-in=LibStdFunction.js --file-out=LibStdFunction.Source.cpp --is-string --name=libStdFunctionSource"));
	exitIf(Shell.system("file-to-cs --touch=LibStdObject.cpp --file-in=LibStdObject.js --file-out=LibStdObject.Source.cpp --is-string --name=libStdObjectSource"));
	exitIf(Shell.system("file-to-cs --touch=LibStdScript.cpp --file-in=LibStdScript.js --file-out=LibStdScript.Source.cpp --is-string --name=libStdScriptSource"));
});
