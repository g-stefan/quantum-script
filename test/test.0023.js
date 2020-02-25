// Public domain
// http://unlicense.org/
Script.requireExtension("Console");

function With(this_, function_) {
	return function_.call(this_);
};

With(Console, function() {
	.writeLn("-> test 0023 ok");
});
