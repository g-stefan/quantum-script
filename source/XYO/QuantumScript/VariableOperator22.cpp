// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableOperator22.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableOperator22, "{152BEAAB-FC7B-4F54-846B-15000DE5B937}");

	VariableOperator22::VariableOperator22() {
		XYO_DYNAMIC_TYPE_PUSH(VariableOperator22);
	};

	Variable *VariableOperator22::newVariable() {
		return (Variable *)TMemory<VariableOperator22>::newMemory();
	};

	bool VariableOperator22::toBoolean() {
		return true;
	};

	String VariableOperator22::toString() {
		return strTypeUndefined;
	};

};
