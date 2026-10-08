// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: a long file name in Script.include overflowed a stack buffer

Script.requireExtension("Console");

var testName = "0051";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var fileName = "x";
var k;
for(k = 0; k < 13; ++k) {
	fileName = fileName + fileName;
};
check("file name length", fileName.length, 8192);

var isError = false;
try {
	Script.include(fileName);
} catch(e) {
	isError = true;
};
check("include throws", isError, true);

isError = false;
try {
	Script.includeOnce(fileName);
} catch(e) {
	isError = true;
};
check("include once throws", isError, true);

isError = false;
try {
	Script.requireExtension(fileName);
} catch(e) {
	isError = true;
};
check("require extension throws", isError, true);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
