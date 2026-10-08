// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: resuming a coroutine as a method replaced this of the caller

Script.requireExtension("Console");

var testName = "0049";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var source = {
	value: 10,
	generator: function() {
		yield this.value;
		yield this.value + 1;
	}
};

var caller = {
	name: "caller",
	run: function() {
		var value = source.generator();
		return this.name + ":" + value;
	}
};

check("first call", caller.run(), "caller:10");
check("resume", caller.run(), "caller:11");

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
