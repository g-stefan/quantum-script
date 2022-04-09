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

#include "quantum-script-variableoperator21.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

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
};
