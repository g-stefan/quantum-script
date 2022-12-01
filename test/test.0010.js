// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function loop(x) {
	if (x >= 10) {
		return x;
	}
	return loop(x + 1);
};

if(loop(0) == 10) {
	Console.writeLn("-> test 0010 ok");
} else {
	Console.writeLn("-> test 0010 fail");
};

