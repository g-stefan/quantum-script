// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

var mx = "";

function testYield(msg) {
	mx += msg;
	yield 1;
	mx += msg;
	yield 2;
	mx += msg;
	yield 3;
	mx += msg;
	return 4;
};

var x = [];
x[0] = testYield("A");
x[1] = testYield("B");
x[2] = testYield("C");
x[3] = testYield("D");

if(x[0] == 1 && x[1] == 2 && x[2] == 3 && x[3] == 4 && mx == "ABCD") {
	Console.writeLn("-> test 0034 ok");
} else {
	Console.writeLn("-> test 0034 fail");
};

