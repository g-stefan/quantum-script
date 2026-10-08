// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression:
// - x ^= y threw "referenceSet", the left side was not an l-value
// - "x".split("") never returned
// - Resource.prototype.close / drop were not registered
// - a compile error in code without a file name had an empty name

Script.requireExtension("Console");

var testName = "0055";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

// ^=
var x = 16;
x ^= 1;
check("x ^= 1", x, 17);
x ^= 1;
check("x ^= 1 twice", x, 16);
x = 5;
x ^= -1;
check("x ^= -1", x, -6);
var object = {value: 12};
object.value ^= 4;
check("property ^= 4", object.value, 8);
var list = [3, 6];
list[1] ^= 3;
check("element ^= 3", list[1], 5);
function xorLocal(a, b) {
	var c = a;
	c ^= b;
	return c;
};
check("local ^=", xorLocal(10, 3), 9);
var y = 1;
var z = (y ^= 3);
check("^= value", z, 2);
check("^= target", y, 2);

// split with an empty separator: characters, UTF-8 sequences kept whole
var parts = "abc".split("");
check("split empty length", parts.length, 3);
check("split empty join", parts.join("|"), "a|b|c");
check("split empty of empty", "".split("").length, 0);
parts = "aé€😀b".split("");
check("split empty utf-8 length", parts.length, 5);
check("split empty utf-8 2 bytes", parts[1], "é");
check("split empty utf-8 3 bytes", parts[2], "€");
check("split empty utf-8 4 bytes", parts[3], "😀");
check("split empty utf-8 last", parts[4], "b");
check("split separator still works", "a,b".split(",").join("|"), "a|b");

// Resource methods
check("Resource.prototype.close", Script.isFunction(Resource.prototype.close), true);
check("Resource.prototype.drop", Script.isFunction(Resource.prototype.drop), true);

// compile error message of code without a file name
var message = "";
try {
	Script.execute("var = ;");
} catch(e) {
	message = e.message;
};
check("compile error without file name", message, "Compile error on line 1");
message = "";
try {
	Script.execute("var = ;", [], "tagged.js");
} catch(e) {
	message = e.message;
};
check("compile error with tag", message, "Compile error in tagged.js line 1");

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
