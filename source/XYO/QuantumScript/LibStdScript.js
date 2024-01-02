// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
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

Script.requireExtensionExternalOrInternal = function(extension) {
	try{
		return Script.requireExternalExtension(extension);
	} catch(e){};

	return Script.requireInternalExtension(extension);
};

Script.requireExtensionInternalOrExternal = function(extension) {
	try{
		return Script.requireInternalExtension(extension);
	} catch(e){};

	return Script.requireExternalExtension(extension);	
};

Script.requireExtension=Script.requireExtensionExternalOrInternal;
