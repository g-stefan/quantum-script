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
#include <math.h>

#include "quantum-script-variablestring.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablenull.hpp"
#include "quantum-script-variablesymbol.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableString::typeStringKey = "{2CE1F6BC-552C-4EAF-943D-DC3A4D212221}";
		const void *VariableString::typeString;
		const char *VariableString::strTypeString = "String";

		String VariableString::getType() {
			return strTypeString;
		};

		Variable *VariableString::newVariable(String value) {
			VariableString *retV;
			retV = TMemory<VariableString>::newMemory();
			retV->value = value;
			return (Variable *) retV;
		};

		Variable &VariableString::operatorReference(Symbol symbolId) {
			if(symbolId == Context::getSymbolLength()) {
				if(vLength) {
					if(vLength->hasOneReference()) {
						((VariableNumber *)vLength.value())->value = (Number)value.length();
					} else {
						vLength=VariableNumber::newVariable((Number)value.length());
					};
				} else {
					vLength=VariableNumber::newVariable((Number)value.length());
				};
				return *vLength;
			};
			return operatorReferenceX(symbolId, (Context::getPrototypeString())->prototype);
		};

		Variable *VariableString::instancePrototype() {
			return (Context::getPrototypeString())->prototype;
		};

		void VariableString::initMemory() {
			TMemory<String>::initMemory();
		};

		Variable *VariableString::clone(SymbolList &inSymbolList) {
			return newVariable(value.value());
		};

		bool VariableString::hasProperty(Variable *variable) {
			if(VariableSymbol::isVariableSymbol(variable)) {
				if((static_cast<VariableSymbol *>(variable))->value == Context::getSymbolLength()) {
					return true;
				};
			};
			return (Context::getPrototypeString())->prototype->hasProperty(variable);
		};

		bool VariableString::toBoolean() {
			return (value.length() > 0);
		};

		Number VariableString::toNumber() {
			Number retV;
			if(sscanf(value, QUANTUM_SCRIPT_FORMAT_NUMBER_INPUT, &retV) == 1) {
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
};


