// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/ObjectIteratorValue.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableObject.hpp>

namespace XYO::QuantumScript {

	bool ObjectIteratorValue::next(Variable *out) {
		PropertyNode *next_;
		if (value) {
			out->referenceSet(value->value);
			value = value->successor();
			return true;
		};
		out->referenceSet(Context::getValueUndefined());
		return false;
	};

};
