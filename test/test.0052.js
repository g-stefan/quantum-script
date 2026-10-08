// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Regression: skipping 0 lines skipped the whole source

Script.requireExtension("Console");

var testName = "0052";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

var includeLine1 = 0;
var includeLine2 = 0;
Script.includeSkipLines("test.0052.include.js", [], 0);
check("include skip 0 lines, line 1", includeLine1, 1);
check("include skip 0 lines, line 2", includeLine2, 2);

includeLine1 = 0;
includeLine2 = 0;
Script.includeSkipLines("test.0052.include.js", [], 1);
check("include skip 1 line, line 1", includeLine1, 0);
check("include skip 1 line, line 2", includeLine2, 2);

var executeLine1 = 0;
var executeLine2 = 0;
Script.executeSkipLines("executeLine1 = 1;\nexecuteLine2 = 2;\n", [], 0);
check("execute skip 0 lines, line 1", executeLine1, 1);
check("execute skip 0 lines, line 2", executeLine2, 2);

executeLine1 = 0;
executeLine2 = 0;
Script.executeSkipLines("executeLine1 = 1;\nexecuteLine2 = 2;\n", [], 1);
check("execute skip 1 line, line 1", executeLine1, 0);
check("execute skip 1 line, line 2", executeLine2, 2);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
