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

#include "quantum-script-variableoperator22.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableOperator22, "{152BEAAB-FC7B-4F54-846B-15000DE5B937}");

		VariableOperator22::VariableOperator22() {
			XYO_DYNAMIC_TYPE_PUSH(VariableOperator22);
		};

		Variable *VariableOperator22::newVariable() {
			return (Variable *) TMemory<VariableOperator22>::newMemory();
		};

		bool VariableOperator22::toBoolean() {
			return true;
		};


		String VariableOperator22::toString() {
			return strTypeUndefined;
		};

	};
};


