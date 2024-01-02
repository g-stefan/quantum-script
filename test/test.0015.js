// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function fibI(n) {
	var last = 0;
	var cur = 1;
	n = n - 1;
	while(n) {
		--n;
		var tmp = cur;
		cur = last + cur;
		last = tmp;
	}
	return cur;
}

if(fibI(43) == 433494437) {
	Console.writeLn("-> test 0015 ok");
} else {
	Console.writeLn("-> test 0015 fail");
};

