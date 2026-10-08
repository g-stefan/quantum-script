// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
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

	// valid array index: finite, not negative, number or numeric string
	static bool toArrayIndex(Variable *index, size_t &out) {
		Number nr = index->toNumber();
		if (isnan(nr) || isinf(nr) || (nr < 0) || (nr > 4294967295.0)) {
			return false;
		};
		out = (size_t)nr;
		return true;
	};

	// read access, must not change the array
	TPointer<Variable> VariableArray::getPropertyByIndex(size_t index) {
		if (index < value->length()) {
			TPointerX<Variable> &retV = (*value)[index];
			if (retV) {
				return retV;
			};
		};
		return Context::getValueUndefined();
	};

	TPointer<Variable> VariableArray::getPropertyByVariable(Variable *index) {
		if (TIsType<VariableNumber>(index)) {
			size_t index_;
			if (toArrayIndex(index, index_)) {
				return getPropertyByIndex(index_);
			};
			return Context::getValueUndefined();
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
		size_t index_;
		if (toArrayIndex(index, index_)) {
			(*value)[index_] = valueToSet;
			return;
		};
		throw Error("invalid array index");
	};

	bool VariableArray::deletePropertyByIndex(size_t index) {
		return value->remove(index);
	};

	bool VariableArray::deletePropertyByVariable(Variable *index) {
		size_t index_;
		if (toArrayIndex(index, index_)) {
			return value->remove(index_);
		};
		return false;
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
			TPointerX<Variable> &item = (*value)[k];
			if (item) {
				((*(out->value))[k]) = item->clone(inSymbolList);
				continue;
			};
			((*(out->value))[k]) = VariableUndefined::newVariable();
		};
		return out;
	};

	bool VariableArray::hasPropertyByVariable(Variable *variable) {
		if (TIsType<VariableNumber>(variable)) {
			size_t index_;
			if (toArrayIndex(variable, index_)) {
				if (index_ < value->length()) {
					return true;
				};
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
			retV << (getPropertyByIndex(scan)->toString());
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
			retV << (getPropertyByIndex(scan)->toString());
		};
		return retV;
	};

};
