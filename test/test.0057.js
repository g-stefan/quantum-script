// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// try / catch / finally: the finally always runs, return in the finally overrides,
// break / continue run the finally then leave the loop

Script.requireExtension("Console");

var testName = "0057";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var log = "";

// --- return in finally

function returnInFinally() {
	try {
		return 1;
	} finally {
		return 2;
	};
};
check("return in finally overrides return", returnInFinally(), 2);

function returnInFinallyNormal() {
	var s = "try;";
	try {
		s += "body;";
	} finally {
		return s + "finally";
	};
};
check("return in finally, normal end", returnInFinallyNormal(), "try;body;finally");

function returnInFinallyThrow() {
	try {
		throw "x";
	} finally {
		return "finally";
	};
};
check("return in finally overrides throw", returnInFinallyThrow(), "finally");

function returnInFinallyCatch() {
	try {
		throw "x";
	} catch (e) {
		return "catch";
	} finally {
		return "finally";
	};
};
check("return in finally overrides catch", returnInFinallyCatch(), "finally");

function returnInFinallyLoop() {
	var k;
	var sum = 0;
	for (k = 0; k < 1000; ++k) {
		sum += returnInFinally();
	};
	return sum;
};
check("return in finally, operand stack", returnInFinallyLoop(), 2000);

function returnNested() {
	log = "";
	try {
		try {
			return "inner";
		} finally {
			log += "inner;";
		};
	} finally {
		log += "outer;";
	};
};
check("nested finally, return", returnNested(), "inner");
check("nested finally, order", log, "inner;outer;");

function returnNestedOverride() {
	try {
		try {
			return "inner";
		} finally {
			return "inner finally";
		};
	} finally {
		log = "outer";
	};
};
log = "";
check("nested finally, return in inner finally", returnNestedOverride(), "inner finally");
check("nested finally, outer finally runs", log, "outer");

// --- catch is covered by the finally

function returnInCatch() {
	log = "";
	try {
		throw "x";
	} catch (e) {
		return "catch";
	} finally {
		log += "finally";
	};
};
check("return in catch", returnInCatch(), "catch");
check("return in catch, finally", log, "finally");

function throwInCatch() {
	log = "";
	try {
		try {
			throw "x";
		} catch (e) {
			throw "catch " + e.message;
		} finally {
			log += "finally;";
		};
	} catch (e) {
		log += e.message;
	};
	return log;
};
check("throw in catch", throwInCatch(), "finally;catch x");

function breakInCatch() {
	log = "";
	do {
		try {
			throw "x";
		} catch (e) {
			break;
		} finally {
			log += "finally;";
		};
		log += "not here;";
	} while (false);
	return log + "after";
};
check("break in catch", breakInCatch(), "finally;after");

// --- break / continue

function breakDo() {
	log = "";
	do {
		try {
			break;
		} finally {
			log += "f";
		};
	} while (false);
	return "after";
};
check("break in try, do", breakDo(), "after");
check("break in try, do, finally", log, "f");

function continueDoCondition() {
	var k = 0;
	do {
		++k;
		if (k < 5) {
			continue;
		};
	} while (k < 3);
	return k;
};
check("continue in do, condition", continueDoCondition(), 3);

function continueDo() {
	var k = 0;
	log = "";
	do {
		++k;
		try {
			continue;
		} finally {
			log += k;
		};
		log += "not here";
	} while (k < 3);
	return log;
};
check("continue in try, do", continueDo(), "123");

function breakFor() {
	var k;
	log = "";
	for (k = 0; k < 10; ++k) {
		try {
			if (k == 2) {
				break;
			};
		} finally {
			log += k;
		};
	};
	return log + ";" + k;
};
check("break in try, for", breakFor(), "012;2");

function continueFor() {
	var k;
	log = "";
	for (k = 0; k < 4; ++k) {
		try {
			if (k % 2) {
				continue;
			};
			log += "a";
		} finally {
			log += k;
		};
	};
	return log;
};
check("continue in try, for", continueFor(), "a01a23");

function breakWhile() {
	var k = 0;
	log = "";
	while (true) {
		try {
			++k;
			if (k == 3) {
				break;
			};
			continue;
		} finally {
			log += k;
		};
	};
	return log;
};
check("break / continue in try, while", breakWhile(), "123");

function breakNested() {
	var k;
	log = "";
	for (k = 0; k < 10; ++k) {
		try {
			try {
				break;
			} finally {
				log += "inner;";
			};
		} finally {
			log += "outer;";
		};
	};
	return log + k;
};
check("break in nested try, for", breakNested(), "inner;outer;0");

function breakForIn() {
	var x;
	var o = {a: 1, b: 2, c: 3};
	log = "";
	for (x in o) {
		try {
			if (x == "b") {
				break;
			};
		} finally {
			log += x;
		};
	};
	return log;
};
check("break in try, for in", breakForIn(), "ab");

function breakInFinally() {
	var k;
	log = "";
	for (k = 0; k < 10; ++k) {
		try {
			throw "x";
		} finally {
			log += k;
			break;
		};
	};
	return log + ";" + k;
};
check("break in finally drops the exception", breakInFinally(), "0;0");

function continueInFinally() {
	var k;
	log = "";
	for (k = 0; k < 3; ++k) {
		try {
			return "not here";
		} finally {
			log += k;
			continue;
		};
	};
	return log;
};
check("continue in finally drops the return", continueInFinally(), "012");

function breakInSwitch() {
	var k;
	log = "";
	for (k = 0; k < 2; ++k) {
		switch (k) {
		case 0:
			try {
				break;
			} finally {
				log += "f" + k;
			};
		default:
			log += "d" + k;
		};
	};
	return log;
};
check("break in try, switch", breakInSwitch(), "f0d1");

// --- throw

function throwInFinally() {
	try {
		try {
			throw "try";
		} finally {
			throw "finally";
		};
	} catch (e) {
		return e.message;
	};
};
check("throw in finally overrides", throwInFinally(), "finally");

function throwThroughFinally() {
	log = "";
	try {
		try {
			throw "try";
		} finally {
			log += "finally;";
		};
	} catch (e) {
		log += e.message;
	};
	return log;
};
check("throw through finally", throwThroughFinally(), "finally;try");

// --- tail call in try: the catch / finally runs after the call

function tailValue() {
	return "value";
};

function tailThrow() {
	throw "tail";
};

function tailFinally() {
	var local = "local";
	log = "";
	try {
		return tailValue();
	} finally {
		log = local;
	};
};
check("tail call in try", tailFinally(), "value");
check("tail call in try, finally", log, "local");

function tailCatch() {
	var local = "local";
	try {
		return tailThrow();
	} catch (e) {
		return local + " " + e.message;
	};
};
check("tail call in try, catch", tailCatch(), "local tail");

// --- yield in finally

function generatorFinally() {
	try {
		yield 1;
	} finally {
		yield 2;
	};
	return 3;
};
check("yield in finally", [generatorFinally(), generatorFinally(), generatorFinally(), generatorFinally()].join(","), "1,2,3,1");

// ---

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
