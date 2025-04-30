// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableNativeVmFunction.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableNativeVmFunction, "{4E32D29F-5FF0-4D33-A16E-4B221233C809}");
	const char *VariableNativeVmFunction::strTypeFunction = "Function";

	VariableNativeVmFunction::VariableNativeVmFunction() {
		XYO_DYNAMIC_TYPE_PUSH(VariableNativeVmFunction);
	};

	String VariableNativeVmFunction::getVariableType() {
		return strTypeFunction;
	};

	Variable *VariableNativeVmFunction::newVariable(InstructionProcedure procedure, Variable *operand) {
		VariableNativeVmFunction *retV;
		retV = TMemory<VariableNativeVmFunction>::newMemory();
		retV->procedure = procedure;
		retV->operand = operand;
		return (Variable *)retV;
	};

	bool VariableNativeVmFunction::toBoolean() {
		return true;
	};

	String VariableNativeVmFunction::toString() {
		return strTypeFunction;
	};

};
