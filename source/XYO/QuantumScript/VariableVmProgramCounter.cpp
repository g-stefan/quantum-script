// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableVmProgramCounter.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableVmProgramCounter, "{583F4867-23AF-4861-B20B-582B178C30AA}");
	const char *VariableVmProgramCounter::strTypeVmProgramCounter = "ProgramCounter";

	VariableVmProgramCounter::VariableVmProgramCounter() {
		XYO_DYNAMIC_TYPE_PUSH(VariableVmProgramCounter);
	};

	Variable *VariableVmProgramCounter::newVariable(ProgramCounter *value) {
		VariableVmProgramCounter *retV;
		retV = TMemory<VariableVmProgramCounter>::newMemory();
		retV->value = value;
		return (Variable *)retV;
	};

	String VariableVmProgramCounter::getVariableType() {
		return strTypeVmProgramCounter;
	};

	bool VariableVmProgramCounter::toBoolean() {
		return true;
	};

	String VariableVmProgramCounter::toString() {
		return strTypeVmProgramCounter;
	};

};
