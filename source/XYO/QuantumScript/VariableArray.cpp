// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript/VariableArray.hpp>
#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableObject.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/ArrayIteratorKey.hpp>
#include <XYO/QuantumScript/ArrayIteratorValue.hpp>
#include <XYO/QuantumScript/VariableSymbol.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableArray, "{BCC18B65-1BA1-4943-859C-82F791ED62AD}");
	const char *VariableArray::strTypeArray = "Array";

	VariableArray::VariableArray() {
		XYO_DYNAMIC_TYPE_PUSH(VariableArray);
		value.pointerLink(this);
		value.newMemory();
	};

	Variable *VariableArray::newVariable() {
		return (Variable *)TMemory<VariableArray>::newMemory();
	};

	String VariableArray::getVariableType() {
		return strTypeArray;
	};

	TPointer<Variable> VariableArray::getPropertyBySymbol(Symbol symbolId) {
		if (symbolId == Context::getSymbolLength()) {
			return VariableNumber::newVariable((Number)value->length());
		};
		return Variable::getPropertyBySymbol(symbolId);
	};

	TPointer<Variable> VariableArray::getPropertyByIndex(size_t index) {
		TPointerX<Variable> &retV = (*value)[index];
		if (!retV) {
			retV = VariableUndefined::newVariable();
		};
		return retV;
	};

	TPointer<Variable> VariableArray::getPropertyByVariable(Variable *index) {
		if (TIsType<VariableNumber>(index)) {
			return getPropertyByIndex(index->toIndex());
		};
		if (TIsType<VariableSymbol>(index)) {
			if ((static_cast<VariableSymbol *>(index))->value == Context::getSymbolLength()) {
				return VariableNumber::newVariable((Number)value->length());
			};
		};
		return Variable::getPropertyByVariable(index);
	};

	void VariableArray::setPropertyByIndex(size_t index, Variable *valueToSet) {
		(*value)[index] = valueToSet;
	};

	void VariableArray::setPropertyByVariable(Variable *index, Variable *valueToSet) {
		(*value)[index->toIndex()] = valueToSet;
	};

	bool VariableArray::deletePropertyByIndex(size_t index) {
		value->remove(index);
		return true;
	};

	bool VariableArray::deletePropertyByVariable(Variable *index) {
		value->remove(index->toIndex());
		return true;
	};

	Variable *VariableArray::instancePrototype() {
		return (Context::getPrototypeArray())->prototype;
	};

	TPointer<Iterator> VariableArray::getIteratorKey() {
		TPointer<Iterator> retV;
		ArrayIteratorKey *iterator_ = TMemory<ArrayIteratorKey>::newMemory();
		iterator_->sourceArray = this;
		iterator_->index = 0;
		retV = iterator_;
		return retV;
	};

	TPointer<Iterator> VariableArray::getIteratorValue() {
		ArrayIteratorValue *iterator_ = TMemory<ArrayIteratorValue>::newMemory();
		iterator_->sourceArray = this;
		iterator_->index = 0;
		return iterator_;
	};

	void VariableArray::initMemory() {
		Variable::initMemory();
		TMemory<Variable>::initMemory();
		TMemory<Array>::initMemory();
	};

	Variable *VariableArray::clone(SymbolList &inSymbolList) {
		int k;
		VariableArray *out = (VariableArray *)newVariable();
		for (k = 0; k < value->length(); ++k) {
			((*(out->value))[k]) = ((*value)[k])->clone(inSymbolList);
		};
		return out;
	};

	bool VariableArray::hasPropertyByVariable(Variable *variable) {
		if (TIsType<VariableNumber>(variable)) {
			if ((static_cast<VariableNumber *>(variable))->value < value->length()) {
				return true;
			};
		};
		if (TIsType<VariableSymbol>(variable)) {
			if ((static_cast<VariableSymbol *>(variable))->value == Context::getSymbolLength()) {
				return true;
			};
		};
		return (Context::getPrototypeArray())->prototype->hasPropertyByVariable(variable);
	};

	bool VariableArray::toBoolean() {
		return true;
	};

	String VariableArray::toString() {
		uint32_t scan;
		String retV;
		retV = "";
		for (scan = 0; scan < value->length(); ++scan) {
			if (scan > 0) {
				retV << ",";
			};
			retV << (((*value)[scan])->toString());
		};
		return retV;
	};

	String VariableArray::join(String with_) {
		uint32_t scan;
		String retV;
		retV = "";
		for (scan = 0; scan < value->length(); ++scan) {
			if (scan > 0) {
				retV << with_;
			};
			retV << (((*value)[scan])->toString());
		};
		return retV;
	};

};
