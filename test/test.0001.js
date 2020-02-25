// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function With(this_, function_) {
	return function_.call(this_);
};

With("-> test 0001 ok", function() {
	Console.writeLn(this);
});
