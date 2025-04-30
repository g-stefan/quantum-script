// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/AssociativeArrayIteratorKey.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>

namespace XYO::QuantumScript {

	bool AssociativeArrayIteratorKey::next(Variable *out) {
		if (index >= sourceArray->value->arrayKey->length()) {
			out->referenceSet(Context::getValueUndefined());
			return false;
		};
		out->referenceSet((*(sourceArray->value->arrayKey))[index]);
		index++;
		return true;
	};

};
