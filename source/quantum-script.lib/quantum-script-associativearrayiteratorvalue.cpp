//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
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

		bool AssociativeArrayIteratorValue::next(Variable *out) {
			if (index >= sourceArray->value->arrayValue->length()) {
				out->referenceSet(Context::getValueUndefined());
				return false;
			};
			out->referenceSet((*(sourceArray->value->arrayValue))[index]);
			index++;
			return true;
		};

	};
};
