﻿//
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

#include "quantum-script-arrayiteratorkey.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variablenumber.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		bool ArrayIteratorKey::next(Variable *out) {
			if(index >= sourceArray->value->length()) {
				out->referenceSet(Context::getValueUndefined());
				return false;
			};
			out->referenceSet(VariableNumber::newVariable(index));
			++index;
			return true;
		};

	};
};


