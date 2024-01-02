// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

function mandelbrot(x0, y0) {
	var x = 0;
	var y = 0;
	var xTemp;
	var iteration = 0;
	var maxIteration = 1000;
	while ((x * x + y * y < 2 * 2) && (iteration < maxIteration)) {
		xTemp = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = xTemp;
		++iteration;
	};
	return iteration;
};

var output = "";
var map = [];
var x, y, z;

map[map.length] = " ";
for(z = 0; z < 180; ++z) {
	map[map.length] = ".";
};
map[map.length] = "#";
for(y = -20; y < 20; ++y) {
	for(x = -80; x < 80; ++x) {
		output += map[(mandelbrot(x / 50, y / 25) * (map.length - 1)) / 1000];
	};
	output += "\n";
};

Console.writeLn(output);
Console.writeLn("-> test 0017 ok");
