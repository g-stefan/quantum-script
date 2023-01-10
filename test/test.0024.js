// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function inc(n) {
	++n;
};

var x = 10;

inc(x);

if(x == 10) {
	Console.writeLn("-> test 0024 ok");
} else {
	Console.writeLn("-> test 0024 fail");
};

