// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function outside(x) {
	function inside(y) {
		return x + y;
	};
	return inside;
};

var fn_inside = outside(3);
var result = fn_inside(5);
var result1 = outside(3)(5);

if(result == 8 && result1 == 8) {
	Console.writeLn("-> test 0011 ok");
} else {
	Console.writeLn("-> test 0011 fail");
};

