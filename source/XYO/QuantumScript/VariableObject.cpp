// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableObject.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/ObjectIteratorKey.hpp>
#include <XYO/QuantumScript/ObjectIteratorValue.hpp>

#include <XYO/QuantumScript/VariableSymbol.hpp>
#include <XYO/QuantumScript/VariableFunction.hpp>
#include <XYO/QuantumScript/VariableFunctionWithYield.hpp>
#include <XYO/QuantumScript/VariableVmFunction.hpp>
#include <XYO/QuantumScript/VariableNativeVmFunction.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableObject, "{1F81286F-F646-4DCD-B5B5-EEB61C2BFDC0}");
	const char *VariableObject::strTypeObject = "Object";

	VariableObject::VariableObject() {
		XYO_DYNAMIC_TYPE_PUSH(VariableObject);
		value.pointerLink(this);
		prototype.pointerLink(this);
		value.newMemory();
		prototype.newMemory();
	};

	String VariableObject::getVariableType() {
		return strTypeObject;
	};

	Variable *VariableObject::newVariable() {
		return (Variable *)TMemory<VariableObject>::newMemory();
	};

	TPointer<Variable> VariableObject::getPropertyBySymbol(Symbol symbolId) {
		PropertyNode *outX;
		outX = value->find(symbolId);
		if (outX) {
			return outX->value;
		};
		Variable *prototype_ = instancePrototype();
		if (prototype_) {
			return prototype_->getPropertyBySymbol(symbolId);
		};
		return Context::getValueUndefined();
	};

	TPointer<Variable> VariableObject::getPropertyByIndex(size_t index) {
		char buffer[22];
		sprintf(buffer, "%zu", index);
		return getPropertyBySymbol(Context::getSymbol(buffer));
	};

	TPointer<Variable> VariableObject::getPropertyByVariable(Variable *index) {
		if (TIsType<VariableSymbol>(index)) {
			return getPropertyBySymbol(((VariableSymbol *)index)->value);
		};
		return getPropertyBySymbol(Context::getSymbol(index->toString()));
	};

	void VariableObject::setPropertyBySymbol(Symbol symbolId, Variable *valueToSet) {
		PropertyNode *node;
		node = value->find(symbolId);
		if (node) {
			node->value = valueToSet;
			return;
		};
		node = Property::newNode();
		node->key = symbolId;
		node->value.pointerLink(this);
		node->value = valueToSet;
		value->insertNode(node);
	};

	void VariableObject::setPropertyByIndex(size_t index, Variable *valueToSet) {
		char buffer[22];
		sprintf(buffer, "%zu", index);
		setPropertyBySymbol(Context::getSymbol(buffer), valueToSet);
	};

	void VariableObject::setPropertyByVariable(Variable *index, Variable *valueToSet) {
		if (TIsType<VariableSymbol>(index)) {
			return setPropertyBySymbol(((VariableSymbol *)index)->value, valueToSet);
		};
		return setPropertyBySymbol(Context::getSymbol(index->toString()), valueToSet);
	};

	bool VariableObject::deletePropertyBySymbol(Symbol symbolId) {
		value->remove(symbolId);
		return true;
	};

	bool VariableObject::deletePropertyByIndex(size_t index) {
		char buffer[22];
		sprintf(buffer, "%zu", index);
		return deletePropertyBySymbol(Context::getSymbol(buffer));
	};

	bool VariableObject::deletePropertyByVariable(Variable *index) {
		if (TIsType<VariableSymbol>(index)) {
			return deletePropertyBySymbol(((VariableSymbol *)index)->value);
		};
		return deletePropertyBySymbol(Context::getSymbol(index->toString()));
	};

	Variable *VariableObject::instancePrototype() {
		if (prototype->prototype) {
			return prototype->prototype;
		};
		if (((Variable *)this) == (Context::getPrototypeObject())->prototype) {
			return nullptr;
		};
		return (Context::getPrototypeObject())->prototype;
	};

	TPointer<Iterator> VariableObject::getIteratorKey() {
		ObjectIteratorKey *iterator_ = TMemory<ObjectIteratorKey>::newMemory();
		iterator_->value_ = this;
		iterator_->value = value->begin();
		return iterator_;
	};

	TPointer<Iterator> VariableObject::getIteratorValue() {
		ObjectIteratorValue *iterator_ = TMemory<ObjectIteratorValue>::newMemory();
		iterator_->value_ = this;
		iterator_->value = value->begin();
		return iterator_;
	};

	void VariableObject::initMemory() {
		Variable::initMemory();
		TMemory<Prototype>::initMemory();
		TMemory<Property>::initMemory();
		TMemory<Variable>::initMemory();
	};

	Variable *VariableObject::clone(SymbolList &inSymbolList) {
		VariableObject *out = (VariableObject *)newVariable();
		PropertyNode *scan;
		Symbol symbolId;
		String symbolString;

		for (scan = value->begin(); scan; scan = scan->successor()) {
			if (inSymbolList.symbolListMirror.get(scan->key, symbolString)) {
				symbolId = Context::getSymbol(symbolString.value());
				out->value->set(symbolId, scan->value->clone(inSymbolList));
			};
		};
		// don't copy prototype ...
		return out;
	};

	bool VariableObject::hasPropertyByVariable(Variable *variable) {
		PropertyNode *outX;
		Symbol symbolId = Context::getSymbol(variable->toString());
		outX = value->find(symbolId);
		if (outX) {
			return true;
		};
		Variable *prototype_ = instancePrototype();
		if (prototype_) {
			return prototype_->hasPropertyByVariable(variable);
		};
		return false;
	};

	bool VariableObject::toBoolean() {
		return true;
	};

	String VariableObject::toString() {
		TPointer<Variable> variable = getPropertyBySymbol(Context::getSymbolToString());
		if (
		    TIsType<VariableFunction>(variable) ||
		    TIsType<VariableFunctionWithYield>(variable) ||
		    TIsType<VariableVmFunction>(variable) ||
		    TIsType<VariableNativeVmFunction>(variable)) {
			TPointer<VariableArray> arguments(VariableArray::newArray());
			return (variable->functionApply(this, arguments))->toString();
		};
		return strTypeObject;
	};

};
