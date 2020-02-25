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

#include "quantum-script-variableobject.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-objectiteratorkey.hpp"
#include "quantum-script-objectiteratorvalue.hpp"

#include "quantum-script-variablefunction.hpp"
#include "quantum-script-variablefunctionwithyield.hpp"
#include "quantum-script-variablevmfunction.hpp"
#include "quantum-script-variablenativevmfunction.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableObject::typeObjectKey = "{1F81286F-F646-4DCD-B5B5-EEB61C2BFDC0}";
		const void *VariableObject::typeObject;
		const char *VariableObject::strTypeObject = "Object";

		String VariableObject::getType() {
			return strTypeObject;
		};


		Variable *VariableObject::newVariable() {
			return (Variable *) TMemory<VariableObject>::newMemory();
		};

		TPointerX<Variable> &VariableObject::operatorReferenceOwnProperty(Symbol symbolId) {
			PropertyNode *outX;
			outX = value->find(symbolId);
			if (outX) {
				return outX->value;
			};
			outX = Property::newNode();
			outX->key = symbolId;
			outX->value.pointerLink(this);
			outX->value=VariableUndefined::newVariable();
			value->insertNode(outX);
			return outX->value;
		};

		bool VariableObject::findOwnProperty(Symbol symbolId, Variable *&out) {
			PropertyNode *outX;
			outX = value->find(symbolId);
			if (outX) {
				out = outX->value;
				return true;
			};
			return false;
		};

		Variable &VariableObject::operatorReference(Symbol symbolId) {
			return operatorReferenceX(symbolId, this);
		};

		Variable *VariableObject::instancePrototype() {
			if(prototype->prototype) {
				return prototype->prototype;
			};
			if(((Variable *)this) == (Context::getPrototypeObject())->prototype) {
				return nullptr;
			};
			return (Context::getPrototypeObject())->prototype;
		};

		bool VariableObject::operatorDeleteIndex(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			value->remove(symbolId);
			return true;
		};

		bool VariableObject::operatorDeleteOwnProperty(Symbol symbolId) {
			value->remove(symbolId);
			return true;
		};


		Variable &VariableObject::operatorIndex2(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			out = value->find(symbolId);
			if (out) {
				return  *(out->value);
			};
			return *(Context::getValueUndefined());
		};

		TPointerX<Variable> &VariableObject::operatorReferenceIndex(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			out = value->find(symbolId);
			if (out) {
				return out->value;
			};
			out = Property::newNode();
			out->key = symbolId;
			out->value.pointerLink(this);
			out->value=VariableUndefined::newVariable();
			value->insertNode(out);
			return out->value;
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
			TMemory<Prototype>::initMemory();
			TMemory<Property>::initMemory();
			TMemory<Variable>::initMemory();
		};

		Variable *VariableObject::clone(SymbolList &inSymbolList) {
			VariableObject *out = (VariableObject *)newVariable();
			PropertyNode *scan;
			Symbol symbolId;
			String symbolString;

			for(scan = value->begin(); scan; scan = scan->successor()) {
				if(inSymbolList.symbolListMirror.get(scan->key, symbolString)) {
					symbolId = Context::getSymbol(symbolString.value());
					out->value->set(symbolId, scan->value->clone(inSymbolList));
				};
			};
			// don't copy prototype ...
			return out;
		};

		bool VariableObject::hasProperty(Variable *variable) {
			PropertyNode *outX;
			Symbol symbolId = Context::getSymbol(variable->toString());
			outX = value->find(symbolId);
			if (outX) {
				return true;
			};
			Variable *prototype_ = instancePrototype();
			if(prototype_) {
				return prototype_->hasProperty(variable);
			};
			return false;
		};

		bool VariableObject::toBoolean() {
			return true;
		};

		String VariableObject::toString() {
			Variable &variable = operatorReference(Context::getSymbolToString());
			if(
				VariableFunction::isVariableFunction(&variable) ||
				VariableFunctionWithYield::isVariableFunctionWithYield(&variable) ||
				VariableVmFunction::isVariableVmFunction(&variable) ||
				VariableNativeVmFunction::isVariableNativeVmFunction(&variable)
			) {
				TPointer<VariableArray> arguments(VariableArray::newArray());
				return (variable.functionApply(this, arguments))->toString();
			};
			return strTypeObject;
		};


	};
};


