// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableObject.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableNull.hpp>
#include <XYO/QuantumScript/VariableSymbol.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableString, "{2CE1F6BC-552C-4EAF-943D-DC3A4D212221}");
	const char *VariableString::strTypeString = "String";

	VariableString::VariableString() {
		XYO_DYNAMIC_TYPE_PUSH(VariableString);
	};

	String VariableString::getVariableType() {
		return strTypeString;
	};

	Variable *VariableString::newVariable(String value) {
		VariableString *retV;
		retV = TMemory<VariableString>::newMemory();
		retV->value = value;
		return (Variable *)retV;
	};

	TPointer<Variable> VariableString::getPropertyBySymbol(Symbol symbolId) {
		if (symbolId == Context::getSymbolLength()) {
			return VariableNumber::newVariable((Number)value.length());
		};
		return Variable::getPropertyBySymbol(symbolId);
	};

	Variable *VariableString::instancePrototype() {
		return (Context::getPrototypeString())->prototype;
	};

	void VariableString::initMemory() {
		Variable::initMemory();
		TMemory<String>::initMemory();
	};

	Variable *VariableString::clone(SymbolList &inSymbolList) {
		return newVariable(value.value());
	};

	bool VariableString::hasPropertyByVariable(Variable *variable) {
		if (TIsType<VariableSymbol>(variable)) {
			if ((static_cast<VariableSymbol *>(variable))->value == Context::getSymbolLength()) {
				return true;
			};
		};
		return (Context::getPrototypeString())->prototype->hasPropertyByVariable(variable);
	};

	bool VariableString::toBoolean() {
		return (value.length() > 0);
	};

	Number VariableString::toNumber() {
		Number retV;
		if (sscanf(value, XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT, &retV) == 1) {
			return retV;
		};
		return NAN;
	};

	String VariableString::toString() {
		return value;
	};

	bool VariableString::isString() {
		return true;
	};

};
