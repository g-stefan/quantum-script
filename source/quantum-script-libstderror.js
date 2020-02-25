//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

function Error(message) {
	this.message = message;
};

Error.prototype.toString = function() {
	return "Error: " + this.message;
};

