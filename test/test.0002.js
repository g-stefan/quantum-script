// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function factor(n) {
	if(n == 0) {
		return 1;
	};
	return n * factor(n - 1);
};

for(l = 0; l < 8; ++l) {
	for(m = 0; m < 8; ++m) {
		factor(10);
	};
};

Console.writeLn("-> test 0002 ok");


