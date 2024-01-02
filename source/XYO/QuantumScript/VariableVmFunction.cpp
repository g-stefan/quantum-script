// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/ObjectIteratorKey.hpp>
#include <XYO/QuantumScript/ObjectIteratorValue.hpp>

#include <XYO/QuantumScript/InstructionX.hpp>
#include <XYO/QuantumScript/InstructionContext.hpp>
#include <XYO/QuantumScript/VariableVmFunction.hpp>
#include <XYO/QuantumScript/ExecutiveX.hpp>
#include <XYO/QuantumScript/VariableObject.hpp>
#include <XYO/QuantumScript/VariableSymbol.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableVmFunction, "{341DBCC2-9D58-4E15-B9CA-9F215B8375AD}");
	const char *VariableVmFunction::strTypeVmFunction = "Function";

	VariableVmFunction::VariableVmFunction() {
		XYO_DYNAMIC_TYPE_PUSH(VariableVmFunction);
		object.pointerLink(this);
		prototype.pointerLink(this);
		coroutineContext.pointerLink(this);
		functionParent.pointerLink(this);
		functionHint = ParserFunctionHint::All;
		coroutineContext.newMemory();
		fnSource = 0;
	};

	Variable *VariableVmFunction::newVariable(ProgramCounter *value) {
		VariableVmFunction *retV;
		retV = TMemory<VariableVmFunction>::newMemory();
		retV->value = value;
		retV->valueEnd = nullptr;
		retV->originalValue = value;
		retV->functionHint = ParserFunctionHint::All;
		retV->fnSource = 0;
		return (Variable *)retV;
	};

	String VariableVmFunction::getVariableType() {
		return strTypeVmFunction;
	};

	TPointer<Variable> VariableVmFunction::getPropertyBySymbol(Symbol symbolId) {
		if (symbolId == Context::getSymbolPrototype()) {
			return prototype->prototype;
		};

		PropertyNode *outX;
		outX = object->find(symbolId);
		if (outX) {
			return outX->value;
		};

		return Variable::getPropertyBySymbol(symbolId);
	};

	TPointer<Variable> VariableVmFunction::getPropertyByIndex(size_t index) {
		char buffer[22];
		sprintf(buffer, "%zu", index);
		return getPropertyBySymbol(Context::getSymbol(buffer));
	};

	TPointer<Variable> VariableVmFunction::getPropertyByVariable(Variable *index) {
		if (TIsType<VariableSymbol>(index)) {
			return getPropertyBySymbol(((VariableSymbol *)index)->value);
		};
		return getPropertyBySymbol(Context::getSymbol(index->toString()));
	};

	void VariableVmFunction::setPropertyBySymbol(Symbol symbolId, Variable *valueToSet) {
		if (symbolId == Context::getSymbolPrototype()) {
			prototype->prototype = valueToSet;
			return;
		};

		PropertyNode *node;
		node = object->find(symbolId);
		if (node) {
			node->value = valueToSet;
			return;
		};
		node = Property::newNode();
		node->key = symbolId;
		node->value.pointerLink(this);
		node->value = valueToSet;
		object->insertNode(node);
	};

	void VariableVmFunction::setPropertyByIndex(size_t index, Variable *valueToSet) {
		char buffer[22];
		sprintf(buffer, "%zu", index);
		setPropertyBySymbol(Context::getSymbol(buffer), valueToSet);
	};

	void VariableVmFunction::setPropertyByVariable(Variable *index, Variable *valueToSet) {
		if (TIsType<VariableSymbol>(index)) {
			return setPropertyBySymbol(((VariableSymbol *)index)->value, valueToSet);
		};
		return setPropertyBySymbol(Context::getSymbol(index->toString()), valueToSet);
	};

	bool VariableVmFunction::deletePropertyBySymbol(Symbol symbolId) {
		object->remove(symbolId);
		return true;
	};

	bool VariableVmFunction::deletePropertyByIndex(size_t index) {
		char buffer[22];
		sprintf(buffer, "%zu", index);
		return deletePropertyBySymbol(Context::getSymbol(buffer));
	};

	bool VariableVmFunction::deletePropertyByVariable(Variable *index) {
		if (TIsType<VariableSymbol>(index)) {
			return deletePropertyBySymbol(((VariableSymbol *)index)->value);
		};
		return deletePropertyBySymbol(Context::getSymbol(index->toString()));
	};

	TPointer<Variable> VariableVmFunction::functionApply(Variable *this_, VariableArray *arguments) {
		return (ExecutiveX::getExecutive()).callVmFunction(this, this_, arguments);
	};

	Variable *VariableVmFunction::instancePrototype() {
		return (Context::getPrototypeFunction())->prototype;
	};

	bool VariableVmFunction::instanceOfPrototype(Prototype *&out) {
		out = prototype;
		return true;
	};

	String VariableVmFunction::toString() {
		if (fnSource) {
			return Context::getSymbolMirror(fnSource);
		};
		throw Error("toString");
	};

	bool VariableVmFunction::toBoolean() {
		return true;
	};

	TPointer<Iterator> VariableVmFunction::getIteratorKey() {
		ObjectIteratorKey *iterator_ = TMemory<ObjectIteratorKey>::newMemory();
		iterator_->value_ = this;
		iterator_->value = object->begin();
		return iterator_;
	};

	TPointer<Iterator> VariableVmFunction::getIteratorValue() {
		ObjectIteratorValue *iterator_ = TMemory<ObjectIteratorValue>::newMemory();
		iterator_->value_ = this;
		iterator_->value = object->begin();
		return iterator_;
	};

	void VariableVmFunction::initMemory() {
		Variable::initMemory();
		TMemory<Property>::initMemory();
		TMemory<Prototype>::initMemory();
		TMemory<InstructionList>::initMemory();
		TMemory<ExecutiveContext>::initMemory();
		TMemory<FunctionParent>::initMemory();
		TPointerX<TStack<TPointerX<ExecutiveContextPc>>>::initMemory();
	};

	bool VariableVmFunction::isFunction() {
		return true;
	};

};
