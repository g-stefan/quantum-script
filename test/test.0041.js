// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: String.matchASCII ignored its argument

Script.requireExtension("Console");

var testName = "0041";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

check("match *.txt", "hello.txt".matchASCII("*.txt"), true);
check("no match *.txt", "hello.js".matchASCII("*.txt"), false);
check("undefined does not match", "undefined".matchASCII("zzz"), false);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
