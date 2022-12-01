// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableReferenceIndex.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableReferenceIndex, "{E9CB5889-6269-488C-9B02-4F26196618F2}");
	const char *VariableReferenceIndex::strTypeReferenceObjectIndex = "ReferenceObjectIndex";

	VariableReferenceIndex::VariableReferenceIndex() {
		XYO_DYNAMIC_TYPE_PUSH(VariableReferenceIndex);
	};

	String VariableReferenceIndex::getVariableType() {
		return strTypeReferenceObjectIndex;
	};

	Variable *VariableReferenceIndex::newVariable(Variable *object, size_t index) {
		VariableReferenceIndex *retV;
		retV = TMemory<VariableReferenceIndex>::newMemory();
		retV->object = object;
		retV->index = index;
		return (Variable *)retV;
	};

	bool VariableReferenceIndex::toBoolean() {
		return true;
	};

	String VariableReferenceIndex::toString() {
		return strTypeReferenceObjectIndex;
	};

	TPointer<Variable> VariableReferenceIndex::referenceSet(Variable *value) {
		object->setPropertyByIndex(index, value);
		return value;
	};

	TPointer<Variable> VariableReferenceIndex::referenceGet() {
		return object->getPropertyByIndex(index);
	};

	Number VariableReferenceIndex::referenceToNumber() {
		return object->getPropertyByIndex(index)->toNumber();
	};

	String VariableReferenceIndex::referenceToString() {
		return object->getPropertyByIndex(index)->toString();
	};

	TPointer<Variable> VariableReferenceIndex::referenceOperatorPlus(Variable *b) {
		TPointer<Variable> value(object->getPropertyByIndex(index)->operatorPlus(b));
		object->setPropertyByIndex(index, value);
		return value;
	};

	void VariableReferenceIndex::referenceSetA1(Variable *value) {
		object->setPropertyByIndex(index, value);
	};

};
