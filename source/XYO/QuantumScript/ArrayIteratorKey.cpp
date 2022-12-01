// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/ArrayIteratorKey.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>

namespace XYO::QuantumScript {

	bool ArrayIteratorKey::next(Variable *out) {
		if (index >= sourceArray->value->length()) {
			out->referenceSet(Context::getValueUndefined());
			return false;
		};
		out->referenceSet(VariableNumber::newVariable(index));
		++index;
		return true;
	};

};
