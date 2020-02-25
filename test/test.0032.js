// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function testYield() {
	try {
		yield;
		yield;
		throw("-> test 0032 ok");
	} catch(e) {
		Console.writeLn(e.message);
	};
};

testYield();
testYield();
testYield();



