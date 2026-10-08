// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: a coroutine called in tail position (return fn()) failed on resume

Script.requireExtension("Console");

var testName = "0048";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

function generator() {
	yield 1;
	yield 2;
	return 3;
};

function tailCall() {
	return generator();
};

var list = [];
list[0] = tailCall();
list[1] = tailCall();
list[2] = tailCall();
list[3] = tailCall();
check("tail call", list.join(","), "1,2,3,1");

var object = {
	value: 10,
	generator: function() {
		yield this.value;
		yield this.value + 1;
		return this.value + 2;
	}
};

function tailCallThis() {
	return object.generator();
};

list = [];
list[0] = tailCallThis();
list[1] = tailCallThis();
list[2] = tailCallThis();
check("tail call with this", list.join(","), "10,11,12");

function generatorCall() {
	yield 1;
	return 2;
};

function tailCallModeCall() {
	return generatorCall.call(null);
};

list = [];
list[0] = tailCallModeCall();
list[1] = tailCallModeCall();
list[2] = tailCallModeCall();
check("tail call mode call", list.join(","), "1,2,1");

// coroutine with a tail call on return

function one() {
	return 1;
};

function generatorTail() {
	yield 0;
	return one();
};

list = [];
list[0] = generatorTail();
list[1] = generatorTail();
list[2] = generatorTail();
check("coroutine with tail call on return", list.join(","), "0,1,0");

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
