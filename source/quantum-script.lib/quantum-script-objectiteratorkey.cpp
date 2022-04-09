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

#include "quantum-script-objectiteratorkey.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-variablesymbol.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		bool ObjectIteratorKey::next(Variable *out) {
			PropertyNode *next_;
			if (value) {
				out->referenceSet(VariableSymbol::newVariable(value->key));
				value = value->successor();
				return true;
			};
			out->referenceSet(Context::getValueUndefined());
			return false;
		};

	};
};
