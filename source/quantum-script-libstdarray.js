//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

Array.prototype.push = function() {
	var idx;
	for(idx = 0; idx < arguments.length; ++idx) {
		this[.length] = arguments[idx];
	};
};

Array.prototype.concat = function() {
	var retV = [];
	var idx;
	var m;
	for(idx = 0; idx < .length; ++idx) {
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

Array.prototype.sort = function(fnCompare) {
	if(.length == 0) {
		return [];
	};
	if(Script.isNil(fnCompare)) {
		fnCompare = Script.compare;
	};
	var left = [];
	var right = [];
	var pivot = this[0];
	var index;
	for(index = 1; index < .length; ++index) {
		if(fnCompare(this[index], pivot) <= 0) {
			left[left.length] = this[index];
		} else {
			right[right.length] = this[index];
		};
	};
	return self.call(left, fnCompare).concat(pivot, self.call(right, fnCompare));
};

Array.prototype.reverseSort = function(fnCompare) {
	if(.length == 0) {
		return [];
	};
	if(Script.isNil(fnCompare)) {
		fnCompare = Script.compare;
	};
	var left = [];
	var right = [];
	var pivot = this[0];
	var index;
	for(index = 1; index < .length; ++index) {
		if(fnCompare(this[index], pivot) <= 0) {
			right[right.length] = this[index];
		} else {
			left[left.length] = this[index];
		};
	};
	return self.call(left, fnCompare).concat(pivot, self.call(right, fnCompare));
};

Array.prototype.pop = function() {
	if(.length > 0) {
		var retV = this[.length - 1];
		delete this[.length - 1];
		return retV;
	};
	return undefined;
};

Array.prototype.shift = function() {
	if(.length > 0) {
		var retV = this[0];
		var k;
		for(k = 1; k < this.length; ++k) {
			this[k - 1] = this[k];
		};
		delete this[this.length - 1];
		return retV;
	};
	return undefined;
};

