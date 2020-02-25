// Public domain
// http://unlicense.org/

// Tail call optimization test - relate to test 0035
Script.requireExtension("Console");

function tailCall(n) {
	var x;
	for(x = 0; x < 4096 * 100; ++x) {
	};
	return 0;
};

tailCall(0);

Console.writeLn("-> test 0036 ok");
