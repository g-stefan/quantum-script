// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: booleans were compared by identity, true == (1 < 2) was false

Script.requireExtension("Console");

var testName = "0050";
var failed = 0;

var a = true;
var b = (1 < 2);
var c = (1 > 2);

if (!(a == b)) {
	Console.writeLn("-> test " + testName + " fail: true == (1 < 2)");
	++failed;
};
if (!(a === b)) {
	Console.writeLn("-> test " + testName + " fail: true === (1 < 2)");
	++failed;
};
if (a != b) {
	Console.writeLn("-> test " + testName + " fail: true != (1 < 2)");
	++failed;
};
if (!(false == c)) {
	Console.writeLn("-> test " + testName + " fail: false == (1 > 2)");
	++failed;
};
if (a == c) {
	Console.writeLn("-> test " + testName + " fail: true == false");
	++failed;
};
if (a === c) {
	Console.writeLn("-> test " + testName + " fail: true === false");
	++failed;
};

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
