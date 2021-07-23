//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-objectiteratorkey.hpp"
#include "quantum-script-objectiteratorvalue.hpp"

#include "quantum-script-variablefunction.hpp"
#include "quantum-script-variablearray.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablesymbol.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableFunction, "{84EAB78C-BA85-4E5E-AC2F-06E7DF2E9E8B}");
		const char *VariableFunction::strTypeFunction = "Function";

		VariableFunction::VariableFunction() {
			XYO_DYNAMIC_TYPE_PUSH(VariableFunction);

			super.pointerLink(this);
			prototype.pointerLink(this);
			functionParent.pointerLink(this);

			prototype.newMemory();
			prototype->prototype=VariableObject::newVariable();

			functionProcedure = nullptr;
			valueSuper = nullptr;
		};

		Variable *VariableFunction::newVariable(FunctionParent *functionParent, VariableArray *parentVariables, VariableArray *parentArguments, FunctionProcedure functionProcedure, Object *super, void *valueSuper) {
			VariableFunction *retV;
			retV = TMemory<VariableFunction>::newMemory();
			if(functionParent || parentVariables || parentArguments) {
				retV->functionParent.newMemory();
				retV->functionParent->functionParent = functionParent;
				retV->functionParent->variables = parentVariables;
				retV->functionParent->arguments = parentArguments;
			};
			retV->functionProcedure = functionProcedure;
			retV->super = super;
			retV->valueSuper = valueSuper;
			return (Variable *) retV;
		};

		String VariableFunction::getVariableType() {
			return strTypeFunction;
		};

		TPointer<Variable> VariableFunction::getPropertyBySymbol(Symbol symbolId) {
			if(symbolId == Context::getSymbolPrototype()) {
				return prototype->prototype;
			};

			PropertyNode *outX;
			outX = object->find(symbolId);
			if (outX) {
				return outX->value;
			};

			return Variable::getPropertyBySymbol(symbolId);			
		};

		TPointer<Variable> VariableFunction::getPropertyByIndex(size_t index) {
			char buffer[22];
			sprintf(buffer, "%zu", index);
			return getPropertyBySymbol(Context::getSymbol(buffer));
		};

		TPointer<Variable> VariableFunction::getPropertyByVariable(Variable *index) {
			if (TIsType<VariableSymbol>(index)) {
				return getPropertyBySymbol(((VariableSymbol *) index)->value);
			};
			return getPropertyBySymbol(Context::getSymbol(index->toString()));
		};

		void VariableFunction::setPropertyBySymbol(Symbol symbolId, Variable *valueToSet) {
			if(symbolId == Context::getSymbolPrototype()) {
				prototype->prototype = valueToSet;
				return;
			};

			PropertyNode *node;
			node = object->find(symbolId);
			if (node) {
				node->value=valueToSet;
				return;
			};
			node = Property::newNode();
			node->key = symbolId;
			node->value.pointerLink(this);
			node->value = valueToSet;
			object->insertNode(node);
		};

		void VariableFunction::setPropertyByIndex(size_t index, Variable *valueToSet) {
			char buffer[22];
			sprintf(buffer, "%zu", index);
			setPropertyBySymbol(Context::getSymbol(buffer), valueToSet);
		};

		void VariableFunction::setPropertyByVariable(Variable *index, Variable *valueToSet) {
			if (TIsType<VariableSymbol>(index)) {
				return setPropertyBySymbol(((VariableSymbol *) index)->value, valueToSet);
			};
			return setPropertyBySymbol(Context::getSymbol(index->toString()), valueToSet);
		};

		bool VariableFunction::deletePropertyBySymbol(Symbol symbolId) {
			object->remove(symbolId);
			return true;
		};

		bool VariableFunction::deletePropertyByIndex(size_t index) {
			char buffer[22];
			sprintf(buffer, "%zu", index);
			return deletePropertyBySymbol(Context::getSymbol(buffer));
		};

		bool VariableFunction::deletePropertyByVariable(Variable *index) {
			if (TIsType<VariableSymbol>(index)) {
				return deletePropertyBySymbol(((VariableSymbol *) index)->value);
			};
			return deletePropertyBySymbol(Context::getSymbol(index->toString()));
		};

		TPointer<Variable> VariableFunction::functionApply(Variable *this_, VariableArray *arguments) {
			return (*functionProcedure)(this, this_, arguments);
		};

		Variable *VariableFunction::instancePrototype() {
			return (Context::getPrototypeFunction())->prototype;
		};

		bool VariableFunction::instanceOfPrototype(Prototype *&out) {
			out = prototype;
			return true;
		};		

		TPointer<Iterator> VariableFunction::getIteratorKey() {
			ObjectIteratorKey *iterator_ = TMemory<ObjectIteratorKey>::newMemory();
			iterator_->value_ = this;
			iterator_->value = object->begin();
			return iterator_;
		};

		TPointer<Iterator> VariableFunction::getIteratorValue() {
			ObjectIteratorValue *iterator_ = TMemory<ObjectIteratorValue>::newMemory();
			iterator_->value_ = this;
			iterator_->value = object->begin();
			return iterator_;
		};

		bool VariableFunction::toBoolean() {
			return true;
		};

		String VariableFunction::toString() {
			return strTypeFunction;
		};

		void VariableFunction::initMemory() {
			Variable::initMemory();
			TMemory<Property>::initMemory();
			TMemory<Prototype>::initMemory();
			TMemory<FunctionParent>::initMemory();
		};

	};
};


