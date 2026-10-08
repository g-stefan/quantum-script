// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: Script.stackTraceWithLevel crashed on a non stack trace argument

Script.requireExtension("Console");

var testName = "0043";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var isError = false;
try {
	Script.stackTraceWithLevel(1, 2);
} catch(e) {
	isError = true;
};
check("invalid argument throws", isError, true);

var trace = "";
try {
	throw new Error("test");
} catch(e) {
	trace = Script.stackTraceWithLevel(e.stackTrace, 1);
};
check("stack trace with level", Script.isString(trace), true);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
