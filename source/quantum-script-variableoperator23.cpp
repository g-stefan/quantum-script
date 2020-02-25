//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-variableoperator23.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableOperator23::typeOperator23Key = "{4BFBFC10-9165-45FE-BCB2-73461E55C03F}";
		const void *VariableOperator23::typeOperator23;

		Variable *VariableOperator23::newVariable() {
			return (Variable *) TMemory<VariableOperator23>::newMemory();
		};

		bool VariableOperator23::toBoolean() {
			return true;
		};

		String VariableOperator23::toString() {
			return strTypeUndefined;
		};

	};
};


