// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: deleting properties while iterating an object used a freed node

Script.requireExtension("Console");

var testName = "0047";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var object = {a: 1, b: 2, c: 3, d: 4};
var keys = [];
var key;
for(key in object) {
	keys[keys.length] = key;
	delete object.b;
	delete object.c;
};
check("keys after delete", keys.join(","), "a,d");

object = {a: 1, b: 2, c: 3};
keys = [];
for(key in object) {
	keys[keys.length] = key;
	delete object[key];
};
check("delete current key", keys.join(","), "a,b,c");

object = {a: 1, b: 2, c: 3};
var values = [];
var value;
for(value of object) {
	values[values.length] = value;
	delete object.b;
};
check("values after delete", values.join(","), "1,3");

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
