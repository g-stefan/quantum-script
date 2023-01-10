// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

var undef;
var undef2;

var isOk = true;
if(undef) {
	Console.writeLn("-> test 0027 fail #1");
	isOk = false;
};
if(!undef) {
} else {
	Console.writeLn("-> test 0027 fail #2");
	isOk = false;
};
if(undef == null) {
} else {
	Console.writeLn("-> test 0027 fail #3");
	isOk = false;
};
if(null == undef) {
} else {
	Console.writeLn("-> test 0027 fail #4");
	isOk = false;
};
if(undef2 == undef) {
} else {
	Console.writeLn("-> test 0027 fail #5");
	isOk = false;
};
if(1 == undef) {
	Console.writeLn("-> test 0027 fail #6");
	isOk = false;
};
if(undef == 1) {
	Console.writeLn("-> test 0027 fail #7");
	isOk = false;
};
if(1.2 == undef) {
	Console.writeLn("-> test 0027 fail #8");
	isOk = false;
};
if(undef == 1.2) {
	Console.writeLn("-> test 0027 fail #9");
	isOk = false;
};
if(1 == null) {
	Console.writeLn("-> test 0027 fail #10");
	isOk = false;
};
if(null == 1) {
	Console.writeLn("-> test 0027 fail #11");
	isOk = false;
};
if(1.2 == null) {
	Console.writeLn("-> test 0027 fail #12");
	isOk = false;
};
if(null == 1.2) {
	Console.writeLn("-> test 0027 fail #13");
	isOk = false;
};
if("x" == null) {
	Console.writeLn("-> test 0027 fail #14");
	isOk = false;
};
if(null == "x") {
	Console.writeLn("-> test 0027 fail #15");
	isOk = false;
};
if("x" == undef) {
	Console.writeLn("-> test 0027 fail #16");
	isOk = false;
};
if(undef == "x") {
	Console.writeLn("-> test 0027 fail #17");
	isOk = false;
};
if([] == undef) {
	Console.writeLn("-> test 0027 fail #18");
	isOk = false;
};
if(undef == []) {
	Console.writeLn("-> test 0027 fail #19");
	isOk = false;
};
if([] == null) {
	Console.writeLn("-> test 0027 fail #20");
	isOk = false;
};
if(null == []) {
	Console.writeLn("-> test 0027 fail #21");
	isOk = false;
};
if({} == undef) {
	Console.writeLn("-> test 0027 fail #22");
	isOk = false;
};
if(undef == {}) {
	Console.writeLn("-> test 0027 fail #23");
	isOk = false;
};
if({} == null) {
	Console.writeLn("-> test 0027 fail #24");
	isOk = false;
};
if(null == {}) {
	Console.writeLn("-> test 0027 fail #25");
	isOk = false;
};
if(null != null) {
	Console.writeLn("-> test 0027 fail #26");
	isOk = false;
};

if(isOk) {
	Console.writeLn("-> test 0027 ok");
};


