// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function testYield() {
	yield 1;
	yield 2;
	yield 3;
	return 4;
};

var x = [];
x[0] = testYield();
x[1] = testYield();
x[2] = testYield();
x[3] = testYield();

if(x[0] == 1 && x[1] == 2 && x[2] == 3 && x[3] == 4) {
	Console.writeLn("-> test 0031 ok");
} else {
	Console.writeLn("-> test 0031 fail");
};

