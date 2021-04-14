//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
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

		const char *VariableOperator22::typeOperator22Key = "{152BEAAB-FC7B-4F54-846B-15000DE5B937}";
		const void *VariableOperator22::typeOperator22;

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


