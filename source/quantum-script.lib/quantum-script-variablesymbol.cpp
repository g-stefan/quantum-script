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
#include <math.h>

#include "quantum-script-variablesymbol.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variablenull.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-variablenumber.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableSymbol, "{C3417ABA-592E-403B-8C3A-11E4EAF889C6}");
		const char *VariableSymbol::strTypeSymbol = "Symbol";

		VariableSymbol::VariableSymbol() {
			XYO_DYNAMIC_TYPE_PUSH(VariableSymbol);
		};

		String VariableSymbol::getVariableType() {
			return strTypeSymbol;
		};

		TPointer<Variable> VariableSymbol::getPropertyBySymbol(Symbol symbolId) {
			if(symbolId == Context::getSymbolLength()) {
				return VariableNumber::newVariable((Number)((toString()).length()));
			};
			return Variable::getPropertyBySymbol(symbolId);
		};

		Variable *VariableSymbol::newVariable(Symbol value) {
			VariableSymbol *retV;
			retV = TMemory<VariableSymbol>::newMemory();
			retV->value = value;
			return (Variable *) retV;
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
			if(sscanf(toString(), QUANTUM_SCRIPT_FORMAT_NUMBER_INPUT, &retV) == 1) {
				return retV;
			};
			return NAN;
		};

		bool VariableSymbol::isString() {
			return true;
		};


	};
};



