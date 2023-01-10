// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Tail call optimization test - relate to test 0035
Script.requireExtension("Console");

function tailCall(n) {
	var x;
	for(x = 0; x < 4096 * 100; ++x) {
	};
	return 0;
};

tailCall(0);

Console.writeLn("-> test 0036 ok");
