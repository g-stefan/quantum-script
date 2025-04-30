// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableNull.hpp>
#include <XYO/QuantumScript/Context.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableNull, "{BD1A147D-2E59-4583-9E4B-C815A3EF5543}");
	const char *VariableNull::strTypeNull = "null";

	VariableNull::VariableNull() {
		XYO_DYNAMIC_TYPE_PUSH(VariableNull);
	};

	String VariableNull::getVariableType() {
		return strTypeNull;
	};

	Variable *VariableNull::newVariable() {
		return (Variable *)TMemory<VariableNull>::newMemory();
	};

	Variable *VariableNull::clone(SymbolList &inSymbolList) {
		return newVariable();
	};

	bool VariableNull::toBoolean() {
		return false;
	};

	Number VariableNull::toNumber() {
		return 0;
	};

	String VariableNull::toString() {
		return strTypeNull;
	};

};
