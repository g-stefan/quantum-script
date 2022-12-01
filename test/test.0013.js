// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function outside() {
	var x = 10;
	function inside(x) {
		return x;
	};
	return inside;
};
result = outside()(20);


if(result == 20) {
	Console.writeLn("-> test 0013 ok");
} else {
	Console.writeLn("-> test 0013 fail");
};

