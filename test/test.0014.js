// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

var o = {
	a: 2,
	m:
	function(b) {
		return .a + 1;
	}
};


if(o.m() == 3) {
	Console.writeLn("-> test 0014 ok");
} else {
	Console.writeLn("-> test 0014 fail");
};

