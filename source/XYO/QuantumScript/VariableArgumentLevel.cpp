// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableArgumentLevel.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableArgumentLevel, "{E3D25A81-CBD1-4EF4-8E58-7CE3EDB6569B}");

	const char *VariableArgumentLevel::strTypeArgumentLevel = "ArgumentLevel";

	VariableArgumentLevel::VariableArgumentLevel() {
		XYO_DYNAMIC_TYPE_PUSH(VariableArgumentLevel);
	};

	String VariableArgumentLevel::getVariableType() {
		return strTypeArgumentLevel;
	};

	Variable *VariableArgumentLevel::newVariable(int value, int level) {
		VariableArgumentLevel *retV;
		retV = TMemory<VariableArgumentLevel>::newMemory();
		retV->value = value;
		retV->level = level;
		return (Variable *)retV;
	};

	bool VariableArgumentLevel::toBoolean() {
		return true;
	};

	String VariableArgumentLevel::toString() {
		return strTypeArgumentLevel;
	};

};
