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

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableFunction::typeFunctionKey = "{84EAB78C-BA85-4E5E-AC2F-06E7DF2E9E8B}";
		const void *VariableFunction::typeFunction;
		const char *VariableFunction::strTypeFunction = "Function";

		String VariableFunction::getType() {
			return strTypeFunction;
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

		TPointer<Variable> VariableFunction::functionApply(Variable *this_, VariableArray *arguments) {
			return (*functionProcedure)(this, this_, arguments);
		};

		TPointerX<Variable> &VariableFunction::operatorReferenceOwnProperty(Symbol symbolId) {
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

		Variable &VariableFunction::operatorReference(Symbol symbolId) {
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

		Variable *VariableFunction::instancePrototype() {
			return (Context::getPrototypeFunction())->prototype;
		};

		bool VariableFunction::instanceOfPrototype(Prototype *&out) {
			out = prototype;
			return true;
		};

		bool VariableFunction::findOwnProperty(Symbol symbolId, Variable *&out) {
			PropertyNode *outX;
			outX = object->find(symbolId);
			if (outX) {
				out = outX->value;
				return true;
			};
			return false;
		};

		bool VariableFunction::operatorDeleteIndex(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			object->remove(symbolId);
			return true;
		};

		bool VariableFunction::operatorDeleteOwnProperty(Symbol symbolId) {
			object->remove(symbolId);
			return true;
		};


		Variable &VariableFunction::operatorIndex2(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			out = object->find(symbolId);
			if (out) {
				return  *(out->value);
			};
			return *(Context::getValueUndefined());
		};

		TPointerX<Variable> &VariableFunction::operatorReferenceIndex(Variable *variable) {
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

		Variable *VariableFunction::clone(SymbolList &inSymbolList) {
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

		bool VariableFunction::hasProperty(Variable *variable) {
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


		void VariableFunction::initMemory() {
			TMemory<Prototype>::initMemory();
			TMemory<FunctionParent>::initMemory();
		};

		bool VariableFunction::toBoolean() {
			return true;
		};

		String VariableFunction::toString() {
			return strTypeFunction;
		};

	};
};


