// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: array index handling
// - reading out of range grew the array
// - non numeric index wrote element 0
// - converting a sparse array to string crashed

Script.requireExtension("Console");

var testName = "0042";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var list = [1, 2, 3];
var value = list[10];
check("read out of range is undefined", Script.isUndefined(value), true);
check("read out of range keeps length", list.length, 3);
check("read negative index is undefined", Script.isUndefined(list[-1]), true);
check("read negative index keeps length", list.length, 3);

var isError = false;
try {
	list["foo"] = 99;
} catch(e) {
	isError = true;
};
check("write non numeric index throws", isError, true);
check("write non numeric index keeps element 0", list[0], 1);

list["1"] = 20;
check("write numeric string index", list[1], 20);

check("delete non numeric index", delete list["x"], false);
check("delete non numeric index keeps array", list.join(","), "1,20,3");
check("delete out of range", delete list[10], false);
check("delete out of range keeps array", list.join(","), "1,20,3");

var sparse = [];
sparse[3] = 1;
check("sparse length", sparse.length, 4);
check("sparse to string", "" + sparse, "undefined,undefined,undefined,1");
check("sparse join", sparse.join("-"), "undefined-undefined-undefined-1");

var count = 0;
var item;
for(item of sparse) {
	if (Script.isUndefined(item)) {
		++count;
	};
};
check("sparse iterate values", count, 3);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
