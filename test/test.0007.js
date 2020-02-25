// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function Test() {
	var pass = "-> test 0007 ok";
	function displayPass() {
		Console.writeLn(pass);
	};
	return displayPass;
};

var doTest = new Test();
doTest();

