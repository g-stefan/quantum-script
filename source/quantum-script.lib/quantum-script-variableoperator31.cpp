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

		const char *VariableOperator31::typeOperator31Key = "{7539857E-DB8F-4744-A1DF-8E25EC271281}";
		const void *VariableOperator31::typeOperator31;

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


