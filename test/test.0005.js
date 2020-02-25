// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function testX(a, b, c) {
	return a + b + c;
};

if(testX(1, 2, 3) == 6) {
	Console.writeLn("-> test 0005 ok");
} else {
	Console.writeLn("-> test 0005 fail");
};

