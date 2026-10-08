// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: numbers were converted to string with only 6 significant digits

Script.requireExtension("Console");

var testName = "0044";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

check("pi", "" + 3.14159265358979, "3.14159265358979");
check("large integer", "" + 123456789012345, "123456789012345");
check("fraction", "" + 1234.5678, "1234.5678");
check("0.1 + 0.2", "" + (0.1 + 0.2), "0.30000000000000004");
check("0.5", "" + 0.5, "0.5");
check("integer", "" + 42, "42");
check("negative", "" + (-2.5), "-2.5");
check("zero", "" + 0, "0");

var total = 0;
var i;
for(i = 0; i < 3000; ++i) {
	total = total + i * 2000000;
};
check("sum", "" + total, "8997000000000");

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
