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

#include "quantum-script-variableoperator21.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableOperator21::typeOperator21Key = "{C271EE0A-5253-4C82-9A70-348075010527}";
		const void *VariableOperator21::typeOperator21;

		Variable *VariableOperator21::newVariable() {
			return (Variable *) TMemory<VariableOperator21>::newMemory();
		};

		bool VariableOperator21::toBoolean() {
			return true;
		};


		String VariableOperator21::toString() {
			return strTypeUndefined;
		};


	};
};


