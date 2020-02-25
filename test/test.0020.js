// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function fibR(n) {
	if (n < 2) {
		return n;
	}
	return (fibR(n - 2) + fibR(n - 1));
}

if(fibR(8) == 21) {
	Console.writeLn("-> test 0020 ok");
} else {
	Console.writeLn("-> test 0020 fail");
};


