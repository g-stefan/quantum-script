//
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

#include "quantum-script-variablenativevmfunction.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

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
};
