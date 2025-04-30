// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Tail call optimization test - relate to test 0036
Script.requireExtension("Console");

function tailCall(n) {
	if(n > 4096 * 100) {
		return 0;
	};
	return tailCall(n + 1);
};

tailCall(0);

Console.writeLn("-> test 0035 ok");
