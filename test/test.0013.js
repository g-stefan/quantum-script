// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function outside() {
	var x = 10;
	function inside(x) {
		return x;
	};
	return inside;
};
result = outside()(20);


if(result == 20) {
	Console.writeLn("-> test 0013 ok");
} else {
	Console.writeLn("-> test 0013 fail");
};

