// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

var x = {a: 100, b: 101, c: 102, d: 103};

var kx = [];
var vx = [];
var n = 0;

for(var m in x) {
	kx[n] = m;
	vx[n] = x[m];
	++n;
};

if( (kx[0] == "a" && vx[0] == 100) &&
	(kx[1] == "b" && vx[1] == 101) &&
	(kx[2] == "c" && vx[2] == 102) &&
	(kx[3] == "d" && vx[3] == 103) ) {

	Console.writeLn("-> test 0019 ok");

} else {

	Console.writeLn("-> test 0019 fail");

};

