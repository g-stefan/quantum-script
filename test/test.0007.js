// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function Test() {
	var pass = "-> test 0007 ok";
	function displayPass() {
		Console.writeLn(pass);
	};
	return displayPass;
};

var doTest = new Test();
doTest();

