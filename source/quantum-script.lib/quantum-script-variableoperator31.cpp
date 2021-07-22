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

#include "quantum-script-variableoperator31.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableOperator31, "{7539857E-DB8F-4744-A1DF-8E25EC271281}");

		VariableOperator31::VariableOperator31() {
			XYO_DYNAMIC_TYPE_PUSH(VariableOperator31);
		};

		Variable *VariableOperator31::newVariable() {
			return (Variable *) TMemory<VariableOperator31>::newMemory();
		};

		bool VariableOperator31::toBoolean() {
			return true;
		};

		String VariableOperator31::toString() {
			return strTypeUndefined;
		};

	};

};


