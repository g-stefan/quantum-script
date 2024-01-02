// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function testX(a, b, c) {
	return a + b + c;
};

if(testX(1, 2, 3) == 6) {
	Console.writeLn("-> test 0005 ok");
} else {
	Console.writeLn("-> test 0005 fail");
};

