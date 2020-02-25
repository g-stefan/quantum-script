// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function inc(n) {
	++n;
};

var x = 10;

inc(x);

if(x == 10) {
	Console.writeLn("-> test 0024 ok");
} else {
	Console.writeLn("-> test 0024 fail");
};

