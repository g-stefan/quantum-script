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

#include "quantum-script-associativearrayiteratorkey.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variablenumber.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		bool AssociativeArrayIteratorKey::next(TPointerX<Variable> &out) {
			if(index >= sourceArray->value->arrayKey->length()) {
				out = Context::getValueUndefined();
				return false;
			};
			out = (*(sourceArray->value->arrayKey))[index];
			index++;
			return true;
		};

	};
};

