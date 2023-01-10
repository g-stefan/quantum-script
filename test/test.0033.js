// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function testYield() {
	var k = 0;
	while(true) {
		++k;
		yield k;
	};
};

if(testYield() == 1 && testYield() == 2 && testYield() == 3 && testYield() == 4) {
	Console.writeLn("-> test 0033 ok");
} else {
	Console.writeLn("-> test 0033 fail");
};

