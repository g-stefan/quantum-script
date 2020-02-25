//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-objectiteratorvalue.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variableobject.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		bool ObjectIteratorValue::next(TPointerX<Variable> &out) {
			PropertyNode *next_;
			if(value) {
				out = value->value;
				value = value->successor();
				return true;
			};
			out = Context::getValueUndefined();
			return false;
		};

	};
};



