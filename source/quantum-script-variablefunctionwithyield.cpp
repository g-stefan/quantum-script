//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-objectiteratorkey.hpp"
#include "quantum-script-objectiteratorvalue.hpp"

#include "quantum-script-variablefunctionwithyield.hpp"
#include "quantum-script-variablearray.hpp"
#include "quantum-script-variableobject.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableFunctionWithYield::typeFunctionWithYieldKey = "{483FB52F-EEAB-46C5-89B3-850A8F65304F}";
		const void *VariableFunctionWithYield::typeFunctionWithYield;
		const char *VariableFunctionWithYield::strTypeFunction = "Function";

		String VariableFunctionWithYield::getType() {
			return strTypeFunction;
		};

		Variable *VariableFunctionWithYield::newVariable(FunctionParent *functionParent, VariableArray *parentVariables, VariableArray *parentArguments, FunctionProcedureWithYield functionProcedure, Object *super, void *valueSuper) {
			VariableFunctionWithYield *retV;
			retV = TMemory<VariableFunctionWithYield>::newMemory();
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


		TPointer<Variable> VariableFunctionWithYield::functionApply(Variable *this_, VariableArray *arguments) {
			if(yieldStep) {
				return (*functionProcedure)(this, this_, arguments);
			};
			yieldVariables=VariableArray::newVariable();
			TPointer<Variable> retV = (*functionProcedure)(this, this_, arguments);
			if(yieldStep) {
				return retV;
			};
			yieldVariables=VariableArray::newVariable();
			return retV;
		};

		TPointerX<Variable> &VariableFunctionWithYield::operatorReferenceOwnProperty(Symbol symbolId) {
			if(symbolId == Context::getSymbolPrototype()) {
				return prototype->prototype;
			};

			PropertyNode *outX;
			outX = object->find(symbolId);
			if (outX) {
				return outX->value;
			};
			outX = Property::newNode();
			outX->key = symbolId;
			outX->value.pointerLink(this);
			outX->value=VariableUndefined::newVariable();
			object->insertNode(outX);
			return outX->value;
		};

		Variable &VariableFunctionWithYield::operatorReference(Symbol symbolId) {
			if(symbolId == Context::getSymbolPrototype()) {
				return *prototype->prototype;
			};
			PropertyNode *outN;
			outN = object->find(symbolId);
			if(outN) {
				return *(outN->value);
			};
			return operatorReferenceX(symbolId, (Context::getPrototypeFunction())->prototype);
		};

		Variable *VariableFunctionWithYield::instancePrototype() {
			return (Context::getPrototypeFunction())->prototype;
		};

		bool VariableFunctionWithYield::instanceOfPrototype(Prototype *&out) {
			out = prototype;
			return true;
		};

		bool VariableFunctionWithYield::findOwnProperty(Symbol symbolId, Variable *&out) {
			PropertyNode *outX;
			outX = object->find(symbolId);
			if (outX) {
				out = outX->value;
				return true;
			};
			return false;
		};

		bool VariableFunctionWithYield::operatorDeleteIndex(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			object->remove(symbolId);
			return true;
		};

		bool VariableFunctionWithYield::operatorDeleteOwnProperty(Symbol symbolId) {
			object->remove(symbolId);
			return true;
		};


		Variable &VariableFunctionWithYield::operatorIndex2(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			out = object->find(symbolId);
			if (out) {
				return  *(out->value);
			};
			return *(Context::getValueUndefined());
		};

		TPointerX<Variable> &VariableFunctionWithYield::operatorReferenceIndex(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			out = object->find(symbolId);
			if (out) {
				return out->value;
			};
			out = Property::newNode();
			out->key = symbolId;
			out->value.pointerLink(this);
			out->value=VariableUndefined::newVariable();
			object->insertNode(out);
			return out->value;
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

		Variable *VariableFunctionWithYield::clone(SymbolList &inSymbolList) {
			VariableObject *out = (VariableObject *)VariableObject::newVariable();
			PropertyNode *scan;
			Symbol symbolId;
			String symbolString;

			for(scan = object->begin(); scan; scan = scan->successor()) {
				if(inSymbolList.symbolListMirror.get(scan->key, symbolString)) {
					symbolId = Context::getSymbol(symbolString.value());
					out->value->set(symbolId, scan->value->clone(inSymbolList));
				};
			};
			// don't copy prototype ...
			return out;
		};

		bool VariableFunctionWithYield::hasProperty(Variable *variable) {
			PropertyNode *outX;
			Symbol symbolId = Context::getSymbol(variable->toString());
			outX = object->find(symbolId);
			if (outX) {
				return true;
			};
			Variable *prototype_ = instancePrototype();
			if(prototype_) {
				return prototype_->hasProperty(variable);
			};
			return false;
		};

		void VariableFunctionWithYield::initMemory() {
			TMemory<Variable>::initMemory();
			TMemory<Prototype>::initMemory();
			TMemory<FunctionParent>::initMemory();
		};

		bool VariableFunctionWithYield::toBoolean() {
			return true;
		};

		String VariableFunctionWithYield::toString() {
			return strTypeFunction;
		};


	};
};


