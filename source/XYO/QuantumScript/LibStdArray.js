// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

Array.prototype.concat = function() {
	var retV = [];
	var idx;
	var m;
	for(idx = 0; idx < this.length; ++idx) {
		retV[retV.length] = this[idx];
	};
	for(m = 0; m < arguments.length; ++m) {
		if(Script.isArray(arguments[m])) {
			for(idx in arguments[m]) {
				retV[retV.length] = arguments[m][idx];
			};
			continue;
		};
		retV[retV.length] = arguments[m];
	};
	return retV;
};

