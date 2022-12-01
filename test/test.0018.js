// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

var x = [100, 101, 102, 103];

var kx = [];
var vx = [];

for(var m in x) {
	kx[m] = m;
	vx[m] = x[m];
};

if( (kx[0] == 0 && vx[0] == 100) &&
	(kx[1] == 1 && vx[1] == 101) &&
	(kx[2] == 2 && vx[2] == 102) &&
	(kx[3] == 3 && vx[3] == 103) ) {

	Console.writeLn("-> test 0018 ok");

} else {

	Console.writeLn("-> test 0018 fail");

};

