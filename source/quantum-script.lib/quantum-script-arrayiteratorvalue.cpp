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

#include "quantum-script-arrayiteratorvalue.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variablenumber.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		bool ArrayIteratorValue::next(Variable *out) {
			if(index >= sourceArray->value->length()) {
				out->referenceSet(Context::getValueUndefined());
				return false;
			};
			out->referenceSet((*(sourceArray->value))[index]);
			index++;
			return true;
		};

	};
};


