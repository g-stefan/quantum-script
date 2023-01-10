// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/ObjectIteratorKey.hpp>
#include <XYO/QuantumScript/ObjectIteratorValue.hpp>

#include <XYO/QuantumScript/VariableFunctionWithYield.hpp>
#include <XYO/QuantumScript/VariableArray.hpp>
#include <XYO/QuantumScript/VariableObject.hpp>
#include <XYO/QuantumScript/VariableSymbol.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableFunctionWithYield, "{483FB52F-EEAB-46C5-89B3-850A8F65304F}");
	const char *VariableFunctionWithYield::strTypeFunction = "Function";

	VariableFunctionWithYield::VariableFunctionWithYield() {
		XYO_DYNAMIC_TYPE_PUSH(VariableFunctionWithYield);

		super.pointerLink(this);
		object.pointerLink(this);
		prototype.pointerLink(this);

		prototype.newMemory();
		prototype->prototype = VariableObject::newVariable();

		functionParent.pointerLink(this);
		yieldVariables.pointerLink(this);
		functionProcedure = nullptr;
		yieldStep = 0;
		yieldVariables = VariableArray::newVariable();
		valueSuper = nullptr;
	};

	Variable *VariableFunctionWithYield::newVariable(FunctionParent *functionParent, VariableArray *parentVariables, VariableArray *parentArguments, FunctionProcedureWithYield functionProcedure, Object *super, void *valueSuper) {
		VariableFunctionWithYield *retV;
		retV = TMemory<VariableFunctionWithYield>::newMemory();
		if (functionParent || parentVariables || parentArguments) {
			retV->functionParent.newMemory();
			retV->functionParent->functionParent = functionParent;
			retV->functionParent->variables = parentVariables;
			retV->functionParent->arguments = parentArguments;
		};
		retV->functionProcedure = functionProcedure;
		retV->super = super;
		retV->valueSuper = valueSuper;
		return (Variable *)retV;
	};

	String VariableFunctionWithYield::getVariableType() {
		return strTypeFunction;
	};

	TPointer<Variable> VariableFunctionWithYield::getPropertyBySymbol(Symbol symbolId) {
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

	TPointer<Variable> VariableFunctionWithYield::getPropertyByIndex(size_t index) {
		char buffer[22];
		sprintf(buffer, "%zu", index);
		return getPropertyBySymbol(Context::getSymbol(buffer));
	};

	TPointer<Variable> VariableFunctionWithYield::getPropertyByVariable(Variable *index) {
		if (TIsType<VariableSymbol>(index)) {
			return getPropertyBySymbol(((VariableSymbol *)index)->value);
		};
		return getPropertyBySymbol(Context::getSymbol(index->toString()));
	};

	void VariableFunctionWithYield::setPropertyBySymbol(Symbol symbolId, Variable *valueToSet) {
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

	void VariableFunctionWithYield::setPropertyByIndex(size_t index, Variable *valueToSet) {
		char buffer[22];
		sprintf(buffer, "%zu", index);
		setPropertyBySymbol(Context::getSymbol(buffer), valueToSet);
	};

	void VariableFunctionWithYield::setPropertyByVariable(Variable *index, Variable *valueToSet) {
		if (TIsType<VariableSymbol>(index)) {
			return setPropertyBySymbol(((VariableSymbol *)index)->value, valueToSet);
		};
		return setPropertyBySymbol(Context::getSymbol(index->toString()), valueToSet);
	};

	bool VariableFunctionWithYield::deletePropertyBySymbol(Symbol symbolId) {
		object->remove(symbolId);
		return true;
	};

	bool VariableFunctionWithYield::deletePropertyByIndex(size_t index) {
		char buffer[22];
		sprintf(buffer, "%zu", index);
		return deletePropertyBySymbol(Context::getSymbol(buffer));
	};

	bool VariableFunctionWithYield::deletePropertyByVariable(Variable *index) {
		if (TIsType<VariableSymbol>(index)) {
			return deletePropertyBySymbol(((VariableSymbol *)index)->value);
		};
		return deletePropertyBySymbol(Context::getSymbol(index->toString()));
	};

	TPointer<Variable> VariableFunctionWithYield::functionApply(Variable *this_, VariableArray *arguments) {
		if (yieldStep) {
			return (*functionProcedure)(this, this_, arguments);
		};
		yieldVariables = VariableArray::newVariable();
		TPointer<Variable> retV = (*functionProcedure)(this, this_, arguments);
		if (yieldStep) {
			return retV;
		};
		yieldVariables = VariableArray::newVariable();
		return retV;
	};

	Variable *VariableFunctionWithYield::instancePrototype() {
		return (Context::getPrototypeFunction())->prototype;
	};

	bool VariableFunctionWithYield::instanceOfPrototype(Prototype *&out) {
		out = prototype;
		return true;
	};

	TPointer<Iterator> VariableFunctionWithYield::getIteratorKey() {
		ObjectIteratorKey *iterator_ = TMemory<ObjectIteratorKey>::newMemory();
		iterator_->value_ = this;
		iterator_->value = object->begin();
		return iterator_;
	};

	TPointer<Iterator> VariableFunctionWithYield::getIteratorValue() {
		ObjectIteratorValue *iterator_ = TMemory<ObjectIteratorValue>::newMemory();
		iterator_->value_ = this;
		iterator_->value = object->begin();
		return iterator_;
	};

	bool VariableFunctionWithYield::toBoolean() {
		return true;
	};

	String VariableFunctionWithYield::toString() {
		return strTypeFunction;
	};

	void VariableFunctionWithYield::initMemory() {
		Variable::initMemory();
		TMemory<Variable>::initMemory();
		TMemory<Property>::initMemory();
		TMemory<Prototype>::initMemory();
		TMemory<FunctionParent>::initMemory();
	};

};
