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

#include "quantum-script-variablereferencesymbol.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableReferenceSymbol, "{47559A07-F3B4-45A2-BC6B-F2C7905108B8}");
		const char *VariableReferenceSymbol::strTypeReferenceObjectSymbol = "ReferenceObjectSymbol";

		VariableReferenceSymbol::VariableReferenceSymbol() {
			XYO_DYNAMIC_TYPE_PUSH(VariableReferenceSymbol);
		};

		String VariableReferenceSymbol::getVariableType() {
			return strTypeReferenceObjectSymbol;
		};

		Variable *VariableReferenceSymbol::newVariable(Variable *object, Symbol symbol) {
			VariableReferenceSymbol *retV;
			retV = TMemory<VariableReferenceSymbol>::newMemory();
			retV->object = object;
			retV->symbol = symbol;
			return (Variable *) retV;
		};

		bool VariableReferenceSymbol::toBoolean() {
			return true;
		};

		String VariableReferenceSymbol::toString() {
			return strTypeReferenceObjectSymbol;
		};

		TPointer<Variable> VariableReferenceSymbol::referenceSet(Variable *value) {
			object->setPropertyBySymbol(symbol, value);
			return value;
		};

		TPointer<Variable> VariableReferenceSymbol::referenceGet() {
			return object->getPropertyBySymbol(symbol);
		};

		Number VariableReferenceSymbol::referenceToNumber() {
			return object->getPropertyBySymbol(symbol)->toNumber();
		};

		String VariableReferenceSymbol::referenceToString() {
			return object->getPropertyBySymbol(symbol)->toString();
		};

		TPointer<Variable> VariableReferenceSymbol::referenceOperatorPlus(Variable *b) {
			TPointer<Variable> value(object->getPropertyBySymbol(symbol)->operatorPlus(b));
			object->setPropertyBySymbol(symbol, value);
			return value;
		};

		void VariableReferenceSymbol::referenceSetA1(Variable *value) {
			object->setPropertyBySymbol(symbol, value);
		};
	};
};


