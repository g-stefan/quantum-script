// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Control flow, closures, coroutines and calls, also run as compiled C++
// (--compile-cpp): the native runtime resolves them without the interpreter

Script.requireExtension("Console");

var testName = "0056";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var log = [];

// try / catch / finally
function tryReturn() {
	try {
		return "try";
	} finally {
		log[log.length] = "finally";
	};
};
check("return in try", tryReturn(), "try");
check("finally after return", log.join(","), "finally");

function tryCatchFinally() {
	var s = "";
	try {
		throw "boom";
	} catch (e) {
		s += "catch " + e.message + ";";
	} finally {
		s += "finally;";
	};
	return s;
};
check("try catch finally", tryCatchFinally(), "catch boom;finally;");

function tryNested() {
	var s = "";
	try {
		try {
			throw "inner";
		} finally {
			s += "inner finally;";
		};
	} catch (e) {
		s += "outer catch " + e.message;
	};
	return s;
};
check("nested try", tryNested(), "inner finally;outer catch inner");

function tryNative() {
	var o = null;
	try {
		o.x = 1;
	} catch (e) {
		return "caught";
	};
	return "not caught";
};
check("error of the runtime", tryNative(), "caught");

function tryObject() {
	try {
		throw {message: "custom", code: 3};
	} catch (e) {
		return e.message + e.code;
	};
};
check("thrown object", tryObject(), "custom3");

// do-while with break / continue
function doWhile() {
	var k = 0;
	var s = "";
	do {
		++k;
		if (k == 2) {
			continue;
		};
		if (k == 5) {
			break;
		};
		s += k;
	} while (k < 10);
	return s;
};
check("do while", doWhile(), "134");

// switch
function switchCase(x) {
	var r = "";
	switch (x) {
	case 1:
		r += "one";
	case 2:
		r += "two";
		break;
	case "a":
		r = "letter";
		break;
	default:
		r = "default";
	};
	return r;
};
check("switch", switchCase(1) + "," + switchCase(2) + "," + switchCase("a") + "," + switchCase(9), "onetwo,two,letter,default");

// closures
function counter() {
	var n = 0;
	return function() {
		++n;
		return n;
	};
};
var counter1 = counter();
var counter2 = counter();
counter1();
counter1();
check("closure", counter1() + "," + counter2(), "3,1");

function outer(a) {
	var b = 10;
	function middle(c) {
		function inner(d) {
			return a + b + c + d;
		};
		return inner(4);
	};
	return middle(3);
};
check("closure levels", outer(1), 18);

// coroutines
function generator() {
	var k;
	for (k = 0; k < 3; ++k) {
		yield k;
	};
	return "done";
};
check("yield in a loop", [generator(), generator(), generator(), generator(), generator()].join(","), "0,1,2,done,0");

function generatorForIn() {
	var o = {a: 1, b: 2, c: 3};
	for (var key in o) {
		yield key;
	};
	return "end";
};
check("yield in for-in", [generatorForIn(), generatorForIn(), generatorForIn(), generatorForIn()].join(","), "a,b,c,end");

var generatorLog = "";
function generatorTry() {
	try {
		yield 1;
		yield 2;
	} finally {
		generatorLog += "finally";
	};
	return 3;
};
check("yield in try", [generatorTry(), generatorTry(), generatorTry()].join(","), "1,2,3");
check("yield in try finally", generatorLog, "finally");

// for-in / for-of
function forIn() {
	var o = {x: 1, y: 2};
	var s = "";
	for (var k in o) {
		s += k + "=" + o[k] + ";";
	};
	for (var v of [5, 6, 7]) {
		s += v;
	};
	for (var k in [5, 6]) {
		s += "[" + k + "]";
	};
	return s;
};
check("for-in", forIn(), "x=1;y=2;567[0][1]");

// new, prototype
function Point(x, y) {
	this.x = x;
	this.y = y;
};
Point.prototype.sum = function() {
	return this.x + this.y;
};
var point = new Point(2, 3);
check("new", point.sum(), 5);
check("instanceof", point instanceof Point, true);
function Factory() {
	return {made: true};
};
check("constructor result", (new Factory()).made, true);

// calls
function argumentsList() {
	return arguments.length + ":" + arguments.join("-");
};
check("arguments", argumentsList(1, 2, 3), "3:1-2-3");
check("call", argumentsList.call(null, "a", "b"), "2:a-b");
check("apply", argumentsList.apply(null, ["x", "y", "z"]), "3:x-y-z");
var factorial = function(n) {
	if (n <= 1) {
		return 1;
	};
	return n * self(n - 1);
};
check("self", factorial(10), 3628800);
check("callback", [3, 1, 2].sort(function(a, b) {
	return a - b;
}).join(","), "1,2,3");
function calledFromScript(x) {
	return "called " + x;
};
check("Script.execute", Script.execute("return calledFromScript(7);"), "called 7");

// recursion, compiled code uses the C++ stack of the thread (1 MB on Windows)
function depth(n) {
	if (n == 0) {
		return 0;
	};
	return 1 + depth(n - 1);
};
check("recursion", depth(500), 500);
function tailCall(n, total) {
	if (n == 0) {
		return total;
	};
	return tailCall(n - 1, total + 1);
};
check("tail call", tailCall(100000, 0), 100000);

// assign the value of a condition
var globalValue = 0;
function conditionValue(c) {
	var local = 0;
	var o = {};
	var a = [];
	local = c ? 1 : 2;
	globalValue = c ? "g" : "h";
	o.v = c ? "o" : "p";
	a[0] = c ? "a" : "b";
	local += c ? 10 : 20;
	return local + globalValue + o.v + a[0];
};
check("condition value", conditionValue(true) + "," + conditionValue(false), "11goa,22hpb");

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
