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

#include "quantum-script-variablereferencevariable.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableReferenceVariable, "{23CD026A-77A4-4417-9C2D-AD7B5C80FD97}");
		const char *VariableReferenceVariable::strTypeReferenceObjectVariable = "ReferenceObjectVariable";

		VariableReferenceVariable::VariableReferenceVariable() {
			XYO_DYNAMIC_TYPE_PUSH(VariableReferenceVariable);
		};

		String VariableReferenceVariable::getVariableType() {
			return strTypeReferenceObjectVariable;
		};

		Variable *VariableReferenceVariable::newVariable(Variable *object, Variable *index) {
			VariableReferenceVariable *retV;
			retV = TMemory<VariableReferenceVariable>::newMemory();
			retV->object = object;
			retV->index = index;
			return (Variable *)retV;
		};

		bool VariableReferenceVariable::toBoolean() {
			return true;
		};

		String VariableReferenceVariable::toString() {
			return strTypeReferenceObjectVariable;
		};

		TPointer<Variable> VariableReferenceVariable::referenceSet(Variable *value) {
			object->setPropertyByVariable(index, value);
			return value;
		};

		TPointer<Variable> VariableReferenceVariable::referenceGet() {
			return object->getPropertyByVariable(index);
		};

		Number VariableReferenceVariable::referenceToNumber() {
			return object->getPropertyByVariable(index)->toNumber();
		};

		String VariableReferenceVariable::referenceToString() {
			return object->getPropertyByVariable(index)->toString();
		};

		TPointer<Variable> VariableReferenceVariable::referenceOperatorPlus(Variable *b) {
			TPointer<Variable> value(object->getPropertyByVariable(index)->operatorPlus(b));
			object->setPropertyByVariable(index, value);
			return value;
		};

		void VariableReferenceVariable::referenceSetA1(Variable *value) {
			object->setPropertyByVariable(index, value);
		};

	};
};
