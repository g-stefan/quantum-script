// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function A(x) {
	function B(y) {
		function C(z) {
			return (x + y + z);
		};
		return C(3);
	};
	return B(2);
};


if(A(1) == 6) {
	Console.writeLn("-> test 0012 ok");
} else {
	Console.writeLn("-> test 0012 fail");
};

