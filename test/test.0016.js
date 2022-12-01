// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

Script.requireExtension("Console");

// define the Person Class
function Person() {};

Person.prototype.walk = function() {
	return 'I am walking!';
};
Person.prototype.sayHello = function() {
	return 'hello';
};

// define the Student class
function Student() {
	// Call the parent constructor
	Person.call(this);
};

// inherit Person
Student.prototype = new Person();

// replace the sayHello method
Student.prototype.sayHello = function() {
	return 'hi, I am a student';
};

// add sayGoodBye method
Student.prototype.sayGoodBye = function() {
	return 'goodBye';
};

var student1 = new Student();

var count = 0;
if(student1.sayHello() == 'hi, I am a student') {
	++count;
};

if(student1.walk() == 'I am walking!') {
	++count;
};

if(student1.sayGoodBye() == 'goodBye') {
	++count;
};

// check inheritance
if(student1 instanceof Person) {
	++count;
};

if(student1 instanceof Student) {
	++count;
};

if(count == 5) {
	Console.writeLn("-> test 0016 ok");
} else {
	Console.writeLn("-> test 0016 fail");
};

