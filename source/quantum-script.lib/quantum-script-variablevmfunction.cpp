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

#include "quantum-script-instructionx.hpp"
#include "quantum-script-instructioncontext.hpp"
#include "quantum-script-variablevmfunction.hpp"
#include "quantum-script-executivex.hpp"
#include "quantum-script-variableobject.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableVmFunction::typeVmFunctionKey = "{341DBCC2-9D58-4E15-B9CA-9F215B8375AD}";
		const void *VariableVmFunction::typeVmFunction;
		const char *VariableVmFunction::strTypeVmFunction = "Function";

		Variable *VariableVmFunction::newVariable(ProgramCounter *value) {
			VariableVmFunction *retV;
			retV = TMemory<VariableVmFunction>::newMemory();
			retV->value = value;
			retV->valueEnd = nullptr;
			retV->originalValue = value;
			retV->functionHint = ParserFunctionHint::All;
			retV->fnSource = 0;
			return (Variable *) retV;
		};

		String VariableVmFunction::getType() {
			return strTypeVmFunction;
		};

		TPointer<Variable> VariableVmFunction::functionApply(Variable *this_, VariableArray *arguments) {
			return  (ExecutiveX::getExecutive()).callVmFunction(this, this_, arguments);
		};

		TPointerX<Variable> &VariableVmFunction::operatorReferenceOwnProperty(Symbol symbolId) {
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

		Variable &VariableVmFunction::operatorReference(Symbol symbolId) {
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

		Variable *VariableVmFunction::instancePrototype() {
			return (Context::getPrototypeFunction())->prototype;
		};

		bool VariableVmFunction::instanceOfPrototype(Prototype *&out) {
			out = prototype;
			return true;
		};

		bool VariableVmFunction::findOwnProperty(Symbol symbolId, Variable *&out) {
			PropertyNode *outX;
			outX = object->find(symbolId);
			if (outX) {
				out = outX->value;
				return true;
			};
			return false;
		};

		bool VariableVmFunction::operatorDeleteIndex(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			object->remove(symbolId);
			return true;
		};

		bool VariableVmFunction::operatorDeleteOwnProperty(Symbol symbolId) {
			object->remove(symbolId);
			return true;
		};


		Variable &VariableVmFunction::operatorIndex2(Variable *variable) {
			PropertyNode *out;
			Symbol symbolId = Context::getSymbol(variable->toString());
			out = object->find(symbolId);
			if (out) {
				return  *(out->value);
			};
			return *(Context::getValueUndefined());
		};

		TPointerX<Variable> &VariableVmFunction::operatorReferenceIndex(Variable *variable) {
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

		Variable *VariableVmFunction::clone(SymbolList &inSymbolList) {
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

		bool VariableVmFunction::hasProperty(Variable *variable) {
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

		String VariableVmFunction::toString() {
			if(fnSource) {
				return Context::getSymbolMirror(fnSource);
			};
			throw Error("toString");
		};

		bool VariableVmFunction::toBoolean() {
			return true;
		};


	};
};


