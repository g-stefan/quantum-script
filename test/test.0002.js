// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function factor(n) {
	if(n == 0) {
		return 1;
	};
	return n * factor(n - 1);
};

for(l = 0; l < 8; ++l) {
	for(m = 0; m < 8; ++m) {
		factor(10);
	};
};

Console.writeLn("-> test 0002 ok");


