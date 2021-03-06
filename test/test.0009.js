// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

var makeCounter = function() {
	var privateCounter = 0;
	function changeBy(val) {
		privateCounter = privateCounter + val;
	};
	return {
		increment:
		function() {
			changeBy(1);
		},
		decrement:
		function() {
			changeBy(-1);
		},
		value:
		function() {
			return privateCounter;
		}
	};
};

var Counter1 = makeCounter();
var Counter2 = makeCounter();
Counter1.increment();
Counter1.increment();
Counter1.decrement();

if(Counter1.value() == 1 && Counter2.value() == 0) {
	Console.writeLn("-> test 0009 ok");
} else {
	Console.writeLn("-> test 0009 fail");
};

