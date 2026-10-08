// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript/Variable.hpp>
#include <XYO/QuantumScript/VariableObject.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableNull.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableBoolean.hpp>
#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/VariableArray.hpp>
#include <XYO/QuantumScript/VariableSymbol.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(Variable, "{23C345C1-0835-4938-863D-C2B39B68A2CE}");
	const char *Variable::strTypeUndefined = "undefined";

	Variable::Variable() {
		XYO_DYNAMIC_TYPE_PUSH(Variable);
	};

	bool Variable::toBoolean() {
		return false;
	};

	Number Variable::toNumber() {
		return NAN;
	};

	String Variable::toString() {
		return strTypeUndefined;
	};

	size_t Variable::toIndex() {
		Number nr = toNumber();
		if (isnan(nr)) {
			return 0;
		};
		if (isinf(nr)) {
			return 0;
		};
		if (nr < 0) {
			return 0;
		};
		return (size_t)(nr);
	};

	bool Variable::isString() {
		return false;
	};

	String Variable::getVariableType() {
		return strTypeUndefined;
	};

	TPointer<Variable> Variable::getPropertyBySymbol(Symbol symbolId) {
		Variable *prototype_ = instancePrototype();
		if (prototype_) {
			return prototype_->getPropertyBySymbol(symbolId);
		};
		return Context::getValueUndefined();
	};

	TPointer<Variable> Variable::getPropertyByIndex(size_t index) {
		Variable *prototype_ = instancePrototype();
		if (prototype_) {
			return prototype_->getPropertyByIndex(index);
		};
		return Context::getValueUndefined();
	};

	TPointer<Variable> Variable::getPropertyByVariable(Variable *index) {
		if (TIsType<VariableSymbol>(index)) {
			return getPropertyBySymbol((static_cast<VariableSymbol *>(index))->value);
		};
		if (TIsType<VariableNumber>(index)) {
			return getPropertyByIndex(index->toIndex());
		};
		Variable *prototype_ = instancePrototype();
		if (prototype_) {
			return prototype_->getPropertyByVariable(index);
		};
		return Context::getValueUndefined();
	};

	void Variable::setPropertyBySymbol(Symbol symbolId, Variable *value) {
		throw Error("setPropertyBySymbol");
	};

	void Variable::setPropertyByIndex(size_t index, Variable *value) {
		throw Error("setPropertyByIndex");
	};

	void Variable::setPropertyByVariable(Variable *index, Variable *value) {
		if (TIsType<VariableSymbol>(index)) {
			setPropertyBySymbol((static_cast<VariableSymbol *>(index))->value, value);
			return;
		};
		if (TIsType<VariableNumber>(index)) {
			setPropertyByIndex(index->toIndex(), value);
			return;
		};
		throw Error("setPropertyByVariable");
	};

	bool Variable::deletePropertyBySymbol(Symbol symbolId) {
		return false;
	};

	bool Variable::deletePropertyByIndex(size_t index) {
		return false;
	};

	bool Variable::deletePropertyByVariable(Variable *index) {
		if (TIsType<VariableSymbol>(index)) {
			return deletePropertyBySymbol((static_cast<VariableSymbol *>(index))->value);
		};
		if (TIsType<VariableNumber>(index)) {
			return deletePropertyByIndex(index->toIndex());
		};
		return false;
	};

	bool Variable::hasPropertyByVariable(Variable *variable) {
		return false;
	};

	TPointer<Variable> Variable::referenceSet(Variable *value) {
		throw Error("referenceSet");
	};

	TPointer<Variable> Variable::referenceGet() {
		throw Error("referenceGet");
	};

	Number Variable::referenceToNumber() {
		return NAN;
	};

	String Variable::referenceToString() {
		return strTypeUndefined;
	};

	TPointer<Variable> Variable::referenceOperatorPlus(Variable *b) {
		throw Error("referenceOperatorPlus");
	};

	void Variable::referenceSetA1(Variable *value) {
		referenceSet(value);
	};

	TPointer<Variable> Variable::functionApply(Variable *this_, VariableArray *arguments) {
		throw Error("functionApply");
	};

	Variable *Variable::instancePrototype() {
		return nullptr;
	};

	bool Variable::instanceOf(Variable *value) {
		Variable *thisPrototype = instancePrototype();
		Prototype *prototype_;
		if (value->instanceOfPrototype(prototype_)) {
			while (thisPrototype) {
				if (thisPrototype == prototype_->prototype) {
					return true;
				};
				thisPrototype = thisPrototype->instancePrototype();
			};
		};
		return false;
	};

	bool Variable::instanceOfPrototype(Prototype *&out) {
		return false;
	};

	TPointer<Iterator> Variable::getIteratorKey() {
		throw Error("key not iterable");
	};

	TPointer<Iterator> Variable::getIteratorValue() {
		throw Error("value not iterable");
	};

	void Variable::initMemory() {
		DynamicObject::initMemory();
		TMemory<String>::initMemory();
	};

	Variable *Variable::clone(SymbolList &inSymbolList) {
		return newVariable();
	};

	TPointer<Variable> Variable::newObjectFunctionApply(VariableArray *arguments) {
		TPointer<Variable> this_(VariableObject::newVariable());
		Prototype *prototype;
		if (instanceOfPrototype(prototype)) {
			((VariableObject *)this_.value())->prototype = prototype;
			return functionApply(this_, arguments);
		};
		return VariableUndefined::newVariable();
	};

	bool Variable::isLess(Variable *b) {
		if (isString() && b->isString()) {
			return (toString() < b->toString());
		};

		Number na = toNumber();

		if (isnan(na)) {
			return false;
		};

		Number nb = b->toNumber();

		if (isnan(nb)) {
			return false;
		};

		return (na < nb);
	};

	bool Variable::isLessOrEqual(Variable *b) {
		if (isString() && b->isString()) {
			return (toString() <= b->toString());
		};

		Number na = toNumber();

		if (isnan(na)) {
			return false;
		};

		Number nb = b->toNumber();

		if (isnan(nb)) {
			return false;
		};

		return (na <= nb);
	};

	bool Variable::isGreater(Variable *b) {
		if (isString() && b->isString()) {
			return (toString() > b->toString());
		};

		Number na = toNumber();

		if (isnan(na)) {
			return false;
		};

		Number nb = b->toNumber();

		if (isnan(nb)) {
			return false;
		};

		return (na > nb);
	};

	bool Variable::isGreaterOrEqual(Variable *b) {
		if (isString() && b->isString()) {
			return (toString() >= b->toString());
		};

		Number na = toNumber();

		if (isnan(na)) {
			return false;
		};

		Number nb = b->toNumber();

		if (isnan(nb)) {
			return false;
		};

		return (na >= nb);
	};

	bool Variable::isEqual(Variable *b) {
		if (isSameType(b)) {
			if (TIsTypeExact<VariableUndefined>(this)) {
				return true;
			};
			if (TIsType<VariableNull>(this)) {
				return true;
			};
			if (TIsType<VariableBoolean>(this)) {
				return ((static_cast<VariableBoolean *>(this))->value == (static_cast<VariableBoolean *>(b))->value);
			};
			if (TIsType<VariableNumber>(this)) {
				if (isnan((static_cast<VariableNumber *>(this))->value)) {
					return false;
				};
				if (isnan((static_cast<VariableNumber *>(b))->value)) {
					return false;
				};
				if ((static_cast<VariableNumber *>(this))->value == (static_cast<VariableNumber *>(b))->value) {
					return true;
				};
				return false;
			};
			if (isString() && b->isString()) {
				return (toString() == b->toString());
			};
			if (this == b) {
				return true;
			};
			return false;
		};
		if (TIsType<VariableNull>(this)) {
			if (TIsTypeExact<VariableUndefined>(b)) {
				return true;
			};
		};
		if (TIsTypeExact<VariableUndefined>(this)) {
			if (TIsType<VariableNull>(b)) {
				return true;
			};
		};

		if (isString() && b->isString()) {
			return (toString() == b->toString());
		};

		Number na = toNumber();

		if (isnan(na)) {
			return false;
		};

		Number nb = b->toNumber();

		if (isnan(nb)) {
			return false;
		};

		return (na == nb);
	};

	bool Variable::isEqualStrict(Variable *b) {
		if (!isSameType(b)) {
			if (isString() && b->isString()) {
				return (toString() == b->toString());
			};
			return false;
		};
		if (TIsTypeExact<VariableUndefined>(this)) {
			return true;
		};
		if (TIsType<VariableNull>(this)) {
			return true;
		};
		if (TIsType<VariableBoolean>(this)) {
			return ((static_cast<VariableBoolean *>(this))->value == (static_cast<VariableBoolean *>(b))->value);
		};
		if (TIsType<VariableNumber>(this)) {
			if (isnan((static_cast<VariableNumber *>(this))->value)) {
				return false;
			};
			if (isnan((static_cast<VariableNumber *>(b))->value)) {
				return false;
			};
			if ((static_cast<VariableNumber *>(this))->value == (static_cast<VariableNumber *>(b))->value) {
				return true;
			};
			return false;
		};
		if (isString() && b->isString()) {
			return (toString() == b->toString());
		};
		if (this == b) {
			return true;
		};
		return false;
	};

	int Variable::compare(Variable *b) {
		if (isString() && b->isString()) {
			int retV = (toString()).compare(b->toString());
			if (retV < 0) {
				return -1;
			};
			if (retV > 0) {
				return 1;
			};
			return 0;
		};

		Number na = toNumber();
		Number nb = b->toNumber();

		if (!(isnan(na) || isnan(nb))) {
			if (na < nb) {
				return -1;
			};
			if (na > nb) {
				return 1;
			};
			return 0;
		};

		// total order, required by sort and associative array keys
		// values without a number representation are placed before numbers
		if (!isnan(nb)) {
			return -1;
		};
		if (!isnan(na)) {
			return 1;
		};

		// both without number representation, order by type, then by identity
		if (!isSameType(b)) {
			int retV = getVariableType().compare(b->getVariableType());
			if (retV != 0) {
				return (retV < 0) ? -1 : 1;
			};
			return ((uintptr_t)(objectType_->type) < (uintptr_t)(b->objectType_->type)) ? -1 : 1;
		};
		if (TIsTypeExact<VariableUndefined>(this) || TIsType<VariableNull>(this) || TIsType<VariableNumber>(this)) {
			return 0;
		};
		if ((uintptr_t)this < (uintptr_t)b) {
			return -1;
		};
		if ((uintptr_t)this > (uintptr_t)b) {
			return 1;
		};
		return 0;
	};

	TPointer<Variable> Variable::operatorPlus(Variable *b) {
		if (isString() || b->isString()) {
			return VariableString::newVariable(toString() << b->toString());
		};
		return VariableNumber::newVariable(toNumber() + b->toNumber());
	};

	bool Variable::isFunction() {
		return false;
	};

	void OperandStack::grow() {
		size_t newCapacity = (capacity_ == 0) ? 256 : capacity_ * 2;
		TPointer<Variable> *newStack = new TPointer<Variable>[newCapacity];
		size_t k;
		for (k = 0; k < size_; ++k) {
			newStack[k] = std::move(stack_[k]);
		};
		delete[] stack_;
		stack_ = newStack;
		capacity_ = newCapacity;
	};
};
