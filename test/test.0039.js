// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: a closure assigning an argument of the parent function
// wrote to its own arguments (or crashed)

Script.requireExtension("Console");

var testName = "0039";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

function outerUseArgument(a) {
	var fn = function(b) {
		a = 5;
		return b;
	};
	fn(1);
	return a;
};
check("assign parent argument", outerUseArgument(1), 5);

function outerNoArgument(a) {
	var fn = function(b) {
		a = 7;
	};
	fn(1);
	return a;
};
check("assign parent argument, own argument not used", outerNoArgument(1), 7);

function outerTwoLevels(a) {
	var fn1 = function() {
		var fn2 = function(c) {
			a = a + c;
		};
		fn2(10);
	};
	fn1();
	return a;
};
check("assign argument two levels up", outerTwoLevels(1), 11);

function outerIncrement(a) {
	var fn = function(b) {
		++a;
		return b;
	};
	fn(0);
	fn(0);
	return a;
};
check("increment parent argument", outerIncrement(1), 3);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
