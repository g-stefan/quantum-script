// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

Script.getFunctionFromFile = function(fileName) {
	var retV;
	Script.setFunctionFromFile("Script.__fn", fileName);
	retV = Script.__fn;
	Script.__fn = undefined;
	return retV;
};

Script.getFunctionFromString = function(sourceCode) {
	var retV;
	Script.setFunctionFromString("Script.__fn", sourceCode);
	retV = Script.__fn;
	Script.__fn = undefined;
	return retV;
};

Script.with = function(this_, proc_) {
	proc_.call(this_);
};

Script.forEach = function(what_, proc_, this_) {
	for(var key in what_) {
		proc_.call(this_, key, what_[key]);
	};
};
