// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Dynamic property keys: read, in and delete of a key never used before

Script.requireExtension("Console");

var testName = "0054";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var object = {a: 1};
var key = "never" + "Used" + "Key" + 54;

check("read unknown key", Script.isUndefined(object[key]), true);
check("in unknown key", key in object, false);
check("delete unknown key", delete object[key], true);
check("object unchanged", object.a, 1);

object[key] = 5;
check("set key after lookup", object[key], 5);
check("in key after set", key in object, true);
check("delete key", delete object[key], true);
check("in key after delete", key in object, false);

check("read unknown index", Script.isUndefined(object[12345678]), true);
object[12345678] = 7;
check("set index", object[12345678], 7);

// prototype chain with dynamic key
function Base() {
};
Base.prototype.inherited = 3;
var derived = new Base();
check("dynamic key from prototype", derived["inh" + "erited"], 3);
check("in from prototype", ("inh" + "erited") in derived, true);
check("unknown key with prototype", Script.isUndefined(derived["nothing" + "Here"]), true);

// function properties
function fn() {
};
check("function unknown key", Script.isUndefined(fn["function" + "Key"]), true);
fn["function" + "Key"] = 9;
check("function set key", fn.functionKey, 9);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
