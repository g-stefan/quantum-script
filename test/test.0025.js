// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

var x = 10;
var c = x;

++x;

if((x == 11) && (c == 10)) {
	Console.writeLn("-> test 0025 ok");
} else {
	Console.writeLn("-> test 0025 fail");
};

