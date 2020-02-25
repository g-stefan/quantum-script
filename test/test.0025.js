// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

var x = 10;
var c = x;

++x;

if((x == 11) && (c == 10)) {
	Console.writeLn("-> test 0025 ok");
} else {
	Console.writeLn("-> test 0025 fail");
};

