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

#include "quantum-script-variablevmprogramcounter.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableVmProgramCounter::typeVmProgramCounterKey = "{583F4867-23AF-4861-B20B-582B178C30AA}";
		const void *VariableVmProgramCounter::typeVmProgramCounter;
		const char *VariableVmProgramCounter::strTypeVmProgramCounter = "ProgramCounter";

		Variable *VariableVmProgramCounter::newVariable(ProgramCounter *value) {
			VariableVmProgramCounter *retV;
			retV = TMemory<VariableVmProgramCounter>::newMemory();
			retV->value = value;
			return (Variable *) retV;
		};

		String VariableVmProgramCounter::getType() {
			return strTypeVmProgramCounter;
		};

		bool VariableVmProgramCounter::toBoolean() {
			return true;
		};

		String VariableVmProgramCounter::toString() {
			return strTypeVmProgramCounter;
		};

	};
};


