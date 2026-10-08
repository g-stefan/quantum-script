// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: octal and unicode string escapes were decoded wrong

Script.requireExtension("Console");

var testName = "0046";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var s = "\101BC";
check("octal 3 digits", s, "ABC");
check("octal 3 digits length", s.length, 3);
check("octal 1 digit", "\7x".getElement(0), 7);
check("octal 1 digit keeps next", "\7x".getElement(1), 120);
check("octal 2 digits", "\12".getElement(0), 10);

var u = "A";
check("unicode ascii", u, "A");
check("unicode ascii length", u.length, 1);

u = "é";
check("unicode 2 bytes utf-8", u, "\303\251");

u = "€";
check("unicode 3 bytes utf-8", u, "\342\202\254");

u = "😀";
check("unicode surrogate pair utf-8", u, "\360\237\230\200");

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
