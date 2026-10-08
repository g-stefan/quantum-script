// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: x %= 0 was an integer division by zero (crash)

Script.requireExtension("Console");

var testName = "0037";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var x = 7;
x %= 0;
check("7 %= 0 is NaN", Script.isNaN(x), true);

x = 7;
x %= 0.5;
check("7 %= 0.5", x, 0);

x = 7;
x %= 3;
check("7 %= 3", x, 1);

x = 7.5;
x %= 2;
check("7.5 %= 2", x, 1.5);

x = -7;
x %= 3;
check("-7 %= 3", x, -1);

check("7 % 3", 7 % 3, 1);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
