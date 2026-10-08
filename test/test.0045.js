// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: bitwise operators returned NaN for negative values,
// shift count >= 64 was undefined behavior

Script.requireExtension("Console");

var testName = "0045";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

check("5 | -1", 5 | -1, -1);
check("5 & -1", 5 & -1, 5);
check("5 ^ -1", 5 ^ -1, -6);
check("~5", ~5, -6);
check("1 << 70", 1 << 70, 0);
check("-8 >> 1", -8 >> 1, -4);
check("-8 >> 70", -8 >> 70, -1);
check("8 >> 70", 8 >> 70, 0);
check("1 >>> 70", 1 >>> 70, 0);
check("16 >>> 2", 16 >>> 2, 4);
check("1 << -1 is NaN", Script.isNaN(1 << -1), true);
check("NaN | 1 is NaN", Script.isNaN(NaN | 1), true);

var x = 5;
x |= -1;
check("x |= -1", x, -1);
x = 5;
x &= -1;
check("x &= -1", x, 5);
x = 1;
x <<= 70;
check("x <<= 70", x, 0);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
