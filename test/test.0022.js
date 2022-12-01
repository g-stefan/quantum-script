// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

var testObject = {
	a: 10,
	b: 20,
	c: 30
};

var result1 = [];
var result2 = [];

for(var k in testObject) {
	result1[result1.length] = [k, testObject[k]];
};

function testFunction(param) {
	for(var k in param) {
		result2[result2.length] = [k, param[k]];
	};
};

testFunction(testObject);

if(
	(result1[0][0] == "a")
	&& (result1[0][1] == 10)
	&& (result1[1][0] == "b")
	&& (result1[1][1] == 20)
	&& (result1[2][0] == "c")
	&& (result1[2][1] == 30)

	&& (result2[0][0] == "a")
	&& (result2[0][1] == 10)
	&& (result2[1][0] == "b")
	&& (result2[1][1] == 20)
	&& (result2[2][0] == "c")
	&& (result2[2][1] == 30)
) {
	Console.writeLn("-> test 0022 ok");
} else {
	Console.writeLn("-> test 0022 fail");
};
