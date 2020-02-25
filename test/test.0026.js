// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function inc(n) {
	var q;
	q = n;
	++q;
	return q;
};

var x = 10;
var z;

z = inc(x);

if(z == 11 && x == 10) {
	Console.writeLn("-> test 0026 ok");
} else {
	Console.writeLn("-> test 0026 fail");
};

