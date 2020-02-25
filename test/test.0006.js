// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function Test() {
	var pass = "-> test 0006 ok";
	function displayPass() {
		Console.writeLn(pass);
	};
	displayPass();
};

Test();

