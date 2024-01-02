// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript/VariableSymbol.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableNull.hpp>
#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableSymbol, "{C3417ABA-592E-403B-8C3A-11E4EAF889C6}");
	const char *VariableSymbol::strTypeSymbol = "Symbol";

	VariableSymbol::VariableSymbol() {
		XYO_DYNAMIC_TYPE_PUSH(VariableSymbol);
	};

	String VariableSymbol::getVariableType() {
		return strTypeSymbol;
	};

	TPointer<Variable> VariableSymbol::getPropertyBySymbol(Symbol symbolId) {
		if (symbolId == Context::getSymbolLength()) {
			return VariableNumber::newVariable((Number)((toString()).length()));
		};
		return Variable::getPropertyBySymbol(symbolId);
	};

	Variable *VariableSymbol::newVariable(Symbol value) {
		VariableSymbol *retV;
		retV = TMemory<VariableSymbol>::newMemory();
		retV->value = value;
		return (Variable *)retV;
	};

	Variable *VariableSymbol::instancePrototype() {
		return (Context::getPrototypeString())->prototype;
	};

	Variable *VariableSymbol::clone(SymbolList &inSymbolList) {
		return VariableString::newVariable(inSymbolList.getSymbolMirror(value));
	};

	String VariableSymbol::toString() {
		return Context::getSymbolMirror(value);
	};

	bool VariableSymbol::toBoolean() {
		return (toString()).length();
	};

	Number VariableSymbol::toNumber() {
		Number retV;
		if (sscanf(toString(), XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT, &retV) == 1) {
			return retV;
		};
		return NAN;
	};

	bool VariableSymbol::isString() {
		return true;
	};

};
