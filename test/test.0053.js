// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// Native array methods: sort, reverseSort, push, pop, shift

Script.requireExtension("Console");

var testName = "0053";
var failed = 0;

function check(name, value, expected) {
	if (value === expected) {
		return;
	};
	Console.writeLn("-> test " + testName + " fail: " + name + " = " + value + " (expected " + expected + ")");
	++failed;
};

// sort returns a new array, source is not changed
var list = [3, 1, 2];
var sorted = list.sort();
check("sort", sorted.join(","), "1,2,3");
check("sort keeps source", list.join(","), "3,1,2");
check("sort empty", [].length, 0);
var empty = [];
check("sort empty array", empty.sort().length, 0);

// custom compare function
list = [5, 3, 9, 1, 7];
check("sort compare", list.sort(function(a, b) {
	return a - b;
}).join(","), "1,3,5,7,9");
check("reverse sort", list.reverseSort().join(","), "9,7,5,3,1");
check("reverse sort compare", list.reverseSort(function(a, b) {
	return a - b;
}).join(","), "9,7,5,3,1");

// stable sort, equal keys keep their order
var items = [];
items.push({key: 2, name: "a"});
items.push({key: 1, name: "b"});
items.push({key: 2, name: "c"});
items.push({key: 1, name: "d"});
var result = items.sort(function(x, y) {
	return x.key - y.key;
});
var names = "";
var item;
for(item of result) {
	names += item.name;
};
check("stable sort", names, "bdac");

// large input, sorted and reverse sorted
var large = [];
var k;
for(k = 0; k < 5000; ++k) {
	large.push(k);
};
var largeSorted = large.reverseSort(function(a, b) {
	return a - b;
});
check("large reverse sort first", largeSorted[0], 4999);
check("large reverse sort last", largeSorted[4999], 0);
check("large reverse sort length", largeSorted.length, 5000);

// compare function throws, the error must propagate and later calls must work
var isError = false;
try {
	[3, 2, 1].sort(function(a, b) {
		throw new Error("compare");
	});
} catch(e) {
	isError = true;
};
check("compare throws", isError, true);
check("sort after throw", [3, 2, 1].sort(function(a, b) {
	return a - b;
}).join(","), "1,2,3");

// nested sort inside compare function
var nested = [[3, 1], [2, 0]].sort(function(a, b) {
	return a.sort()[0] - b.sort()[0];
});
check("nested sort", nested[0].join(","), "2,0");

// push, pop, shift
list = [];
list.push(1, 2, 3);
check("push", list.join(","), "1,2,3");
check("push undefined", Script.isUndefined(list.push(4)), true);
check("pop", list.pop(), 4);
check("pop length", list.length, 3);
check("shift", list.shift(), 1);
check("shift result", list.join(","), "2,3");
empty = [];
check("pop empty", Script.isUndefined(empty.pop()), true);
check("shift empty", Script.isUndefined(empty.shift()), true);

if (failed) {
	throw "test " + testName + " failed";
};
Console.writeLn("-> test " + testName + " ok");
