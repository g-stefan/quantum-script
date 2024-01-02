// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableOperator21.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableOperator21, "{C271EE0A-5253-4C82-9A70-348075010527}");

	VariableOperator21::VariableOperator21() {
		XYO_DYNAMIC_TYPE_PUSH(VariableOperator21);
	};

	Variable *VariableOperator21::newVariable() {
		return (Variable *)TMemory<VariableOperator21>::newMemory();
	};

	bool VariableOperator21::toBoolean() {
		return true;
	};

	String VariableOperator21::toString() {
		return strTypeUndefined;
	};

};
