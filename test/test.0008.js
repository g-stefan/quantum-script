// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function makeAdder(x) {
	return function(y) {
		return x + y;
	};
};

var add5 = makeAdder(5);
var add10 = makeAdder(10);

if((add5(2) == 7) && (add10(2) == 12) ) {
	Console.writeLn("-> test 0008 ok");
} else {
	Console.writeLn("-> test 0008 fail");
};

