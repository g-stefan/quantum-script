// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableOperator31.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableOperator31, "{7539857E-DB8F-4744-A1DF-8E25EC271281}");

	VariableOperator31::VariableOperator31() {
		XYO_DYNAMIC_TYPE_PUSH(VariableOperator31);
	};

	Variable *VariableOperator31::newVariable() {
		return (Variable *)TMemory<VariableOperator31>::newMemory();
	};

	bool VariableOperator31::toBoolean() {
		return true;
	};

	String VariableOperator31::toString() {
		return strTypeUndefined;
	};

};
