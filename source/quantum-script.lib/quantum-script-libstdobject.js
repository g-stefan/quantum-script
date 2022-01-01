//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

Object.prototype.create = function(prototype_) {
	var tmp_ = function() {};
	tmp_.prototype = prototype_;
	return new tmp_();
};
