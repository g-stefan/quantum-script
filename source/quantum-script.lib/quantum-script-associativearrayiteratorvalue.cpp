//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-associativearrayiteratorvalue.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variablenumber.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		bool AssociativeArrayIteratorValue::next(TPointerX<Variable> &out) {
			if(index >= sourceArray->value->arrayValue->length()) {
				out = Context::getValueUndefined();
				return false;
			};
			out = (*(sourceArray->value->arrayValue))[index];
			index++;
			return true;
		};

	};
};



