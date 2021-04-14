//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

Function.prototype.bind = function() {
	if(this instanceof Function) {
		var fn_ = this;
		var this_ = arguments[0];
		var args_ = [];
		for(var k = 1; k < arguments.length; ++k) {
			args_[k - 1] = arguments[k];
		};
		return function() {
			return fn_.apply(this_, args_.concat(arguments));
		};
	};
	return undefined;
};

