// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function testYield() {
	try {
		yield;
		yield;
		throw("-> test 0032 ok");
	} catch(e) {
		Console.writeLn(e.message);
	};
};

testYield();
testYield();
testYield();



