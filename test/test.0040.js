// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: Script.compare truncated the difference to int,
// breaking sort and associative array keys

Script.requireExtension("Console");

var testName = "0040";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

check("compare(0.3, 0.1)", Script.compare(0.3, 0.1), 1);
check("compare(0.1, 0.3)", Script.compare(0.1, 0.3), -1);
check("compare(0.1, 0.1)", Script.compare(0.1, 0.1), 0);
check("compare(10000000000, 0)", Script.compare(10000000000, 0), 1);
check("compare(0, 10000000000)", Script.compare(0, 10000000000), -1);
check("compare('a', 'b')", Script.compare("a", "b"), -1);
check("reverseCompare(0.3, 0.1)", Script.reverseCompare(0.3, 0.1), -1);
check("compare(NaN, 1) is antisymmetric", Script.compare(NaN, 1), -Script.compare(1, NaN));
check("compare('a', 1) is antisymmetric", Script.compare("a", 1), -Script.compare(1, "a"));

var list = [0.3, 0.1, 0.2, 0.25];
check("default sort", list.sort().join(","), "0.1,0.2,0.25,0.3");

list = [10000000000, 0, 20000000000, 5];
check("sort large numbers", list.sort().join(","), "0,5,10000000000,20000000000");

var map = new AssociativeArray();
map[0.5] = "half";
map[0.7] = "seven";
check("associative array fractional keys length", map.length, 2);
check("associative array key 0.5", map[0.5], "half");
check("associative array key 0.7", map[0.7], "seven");

map = new AssociativeArray();
map["a"] = 1;
map[1] = 2;
map["b"] = 3;
map[2] = 4;
check("associative array mixed keys length", map.length, 4);
check("associative array mixed keys", "" + map["a"] + map[1] + map["b"] + map[2], "1234");

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
