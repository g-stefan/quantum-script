// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function fibR(n) {
	if (n < 2) {
		return n;
	}
	return (fibR(n - 2) + fibR(n - 1));
}

if(fibR(8) == 21) {
	Console.writeLn("-> test 0020 ok");
} else {
	Console.writeLn("-> test 0020 fail");
};


