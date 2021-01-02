//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

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
