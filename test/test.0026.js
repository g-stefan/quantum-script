// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function inc(n) {
	var q;
	q = n;
	++q;
	return q;
};

var x = 10;
var z;

z = inc(x);

if(z == 11 && x == 10) {
	Console.writeLn("-> test 0026 ok");
} else {
	Console.writeLn("-> test 0026 fail");
};

