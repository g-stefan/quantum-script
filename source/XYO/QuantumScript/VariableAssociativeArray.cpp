// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableAssociativeArray.hpp>
#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableObject.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableSymbol.hpp>
#include <XYO/QuantumScript/AssociativeArrayIteratorKey.hpp>
#include <XYO/QuantumScript/AssociativeArrayIteratorValue.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableAssociativeArray, "{35EFC1B3-6F8B-4604-A31E-95CC449B48E1}");
	const char *VariableAssociativeArray::strTypeAssociativeArray = "AssociativeArray";

	VariableAssociativeArray::VariableAssociativeArray() {
		XYO_DYNAMIC_TYPE_PUSH(VariableAssociativeArray);
		value.pointerLink(this);
		value.newMemory();
	};

	Variable *VariableAssociativeArray::newVariable() {
		return (Variable *)TMemory<VariableAssociativeArray>::newMemory();
	};

	String VariableAssociativeArray::getVariableType() {
		return strTypeAssociativeArray;
	};

	TPointer<Variable> VariableAssociativeArray::getPropertyBySymbol(Symbol symbolId) {
		if (symbolId == Context::getSymbolLength()) {
			return VariableNumber::newVariable((Number)value->length());
		};
		return Variable::getPropertyBySymbol(symbolId);
	};

	TPointer<Variable> VariableAssociativeArray::getPropertyByIndex(size_t index) {
		TPointer<Variable> index_ = VariableNumber::newVariable((Number)index);
		return getPropertyByVariable(index_);
	};

	TPointer<Variable> VariableAssociativeArray::getPropertyByVariable(Variable *index) {
		if (TIsType<VariableSymbol>(index)) {
			if ((static_cast<VariableSymbol *>(index))->value == Context::getSymbolLength()) {
				return VariableNumber::newVariable((Number)value->length());
			};
		};

		AssociativeArray::MapKey::Node *x;
		x = value->mapKey->find(index);
		if (x) {
			return (*(value->arrayValue))[x->value];
		};

		return VariableUndefined::newVariable();
	};

	void VariableAssociativeArray::setPropertyByIndex(size_t index, Variable *valueToSet) {
		TPointer<Variable> index_ = VariableNumber::newVariable((Number)index);
		setPropertyByVariable(index_, valueToSet);
	};

	void VariableAssociativeArray::setPropertyByVariable(Variable *index, Variable *valueToSet) {
		AssociativeArray::MapKey::Node *x;
		x = value->mapKey->find(index);
		if (x) {
			(*(value->arrayValue))[x->value] = valueToSet;
			return;
		};
		value->mapKey->set(index, value->length_);
		(*(value->arrayKey))[value->length_] = index;
		(*(value->arrayValue))[value->length_] = valueToSet;
		++value->length_;
	};

	bool VariableAssociativeArray::deletePropertyByIndex(size_t index) {
		TPointer<Variable> index_ = VariableNumber::newVariable((Number)index);
		deletePropertyByVariable(index_);
		return true;
	};

	bool VariableAssociativeArray::deletePropertyByVariable(Variable *index) {
		return value->remove(index);
	};

	Variable *VariableAssociativeArray::instancePrototype() {
		return (Context::getPrototypeAssociativeArray())->prototype;
	};

	TPointer<Iterator> VariableAssociativeArray::getIteratorKey() {
		AssociativeArrayIteratorKey *iterator_ = TMemory<AssociativeArrayIteratorKey>::newMemory();
		iterator_->sourceArray = this;
		iterator_->index = 0;
		return iterator_;
	};

	TPointer<Iterator> VariableAssociativeArray::getIteratorValue() {
		AssociativeArrayIteratorValue *iterator_ = TMemory<AssociativeArrayIteratorValue>::newMemory();
		iterator_->sourceArray = this;
		iterator_->index = 0;
		return iterator_;
	};

	void VariableAssociativeArray::initMemory() {
		Variable::initMemory();
		TMemory<Variable>::initMemory();
		TMemory<AssociativeArray>::initMemory();
	};

	Variable *VariableAssociativeArray::clone(SymbolList &inSymbolList) {
		uint32_t k;
		VariableAssociativeArray *out = (VariableAssociativeArray *)newVariable();
		Variable *newKey;
		for (k = 0; k < value->length(); ++k) {
			newKey = ((*(value->arrayKey))[k])->clone(inSymbolList);
			out->value->mapKey->set(newKey, k);
			((*(out->value->arrayKey))[k]) = newKey;
			((*(out->value->arrayValue))[k]) = ((*(value->arrayValue))[k])->clone(inSymbolList);
		};
		return out;
	};

	bool VariableAssociativeArray::toBoolean() {
		return true;
	};

	String VariableAssociativeArray::toString() {
		return strTypeAssociativeArray;
	};

};
