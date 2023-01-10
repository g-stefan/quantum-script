// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableReferenceVariable.hpp>

namespace XYO::QuantumScript {

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
