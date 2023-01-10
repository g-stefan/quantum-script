// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableOperator23.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableOperator23, "{4BFBFC10-9165-45FE-BCB2-73461E55C03F}");

	VariableOperator23::VariableOperator23() {
		XYO_DYNAMIC_TYPE_PUSH(VariableOperator23);
	};

	Variable *VariableOperator23::newVariable() {
		return (Variable *)TMemory<VariableOperator23>::newMemory();
	};

	bool VariableOperator23::toBoolean() {
		return true;
	};

	String VariableOperator23::toString() {
		return strTypeUndefined;
	};

};
