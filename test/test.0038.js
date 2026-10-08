// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: calling a native function with .call() crashed

Script.requireExtension("Console");

var testName = "0038";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

check("native call", Script.isNumber.call(null, 1), true);
check("native call with this", String.prototype.indexOf.call("hello", "l"), 2);

function tailNativeCall() {
	return String.prototype.indexOf.call("hello", "o");
};
check("native call in tail position", tailNativeCall(), 4);

function scriptFunction(a, b) {
	return this.value + a + b;
};
check("script call", scriptFunction.call({value: 1}, 2, 3), 6);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
