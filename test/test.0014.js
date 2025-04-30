// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

var o = {
	a: 2,
	m:
	function(b) {
		return this.a + 1;
	}
};


if(o.m() == 3) {
	Console.writeLn("-> test 0014 ok");
} else {
	Console.writeLn("-> test 0014 fail");
};

