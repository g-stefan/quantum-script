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

#include "quantum-script-variableassociativearray.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-associativearrayiteratorkey.hpp"
#include "quantum-script-associativearrayiteratorvalue.hpp"


namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableAssociativeArray::typeAssociativeArrayKey = "{35EFC1B3-6F8B-4604-A31E-95CC449B48E1}";
		const void *VariableAssociativeArray::typeAssociativeArray;
		const char *VariableAssociativeArray::strTypeAssociativeArray = "AssociativeArray";

		Variable *VariableAssociativeArray::newVariable() {
			return (Variable *) TMemory<VariableAssociativeArray>::newMemory();
		};

		String VariableAssociativeArray::getType() {
			return strTypeAssociativeArray;
		};

		Variable &VariableAssociativeArray::operatorReference(Symbol symbolId) {
			if(symbolId == Context::getSymbolLength()) {
				if(vLength) {
					((VariableNumber *)vLength.value())->value = (Number)value->length();
				} else {
					vLength=VariableNumber::newVariable((Number)value->length());
				};
				return *vLength;
			};

			return operatorReferenceX(symbolId, (Context::getPrototypeAssociativeArray())->prototype);
		};

		Variable *VariableAssociativeArray::instancePrototype() {
			return (Context::getPrototypeAssociativeArray())->prototype;
		};

		TPointer<Iterator> VariableAssociativeArray::getIteratorKey() {
			AssociativeArrayIteratorKey *iterator_ = TMemory<AssociativeArrayIteratorKey>::newMemory();
			iterator_->sourceArray = this;
			iterator_->index = 0;
			return iterator_;
		};

		TPointer<Iterator> VariableAssociativeArray::getIteratorValue() {
			AssociativeArrayIteratorValue *iterator_ = TMemory<AssociativeArrayIteratorValue>::newMemory();
			iterator_->sourceArray = this;
			iterator_->index = 0;
			return iterator_;
		};

		TPointerX<Variable> &VariableAssociativeArray::operatorReferenceIndex(Variable *variable) {
			AssociativeArray::MapKey::Node *x;
			x = value->mapKey->find(variable);
			if(x) {
				return (*(value->arrayValue))[x->value];
			};
			value->mapKey->set(variable, value->length_);
			(*(value->arrayKey))[value->length_]=variable;
			(*(value->arrayValue))[value->length_]=VariableUndefined::newVariable();
			++value->length_;
			return (*(value->arrayValue))[value->length_ - 1];
		};

		bool VariableAssociativeArray::operatorDeleteIndex(Variable *variable) {
			return value->remove(variable);
		};

		Variable &VariableAssociativeArray::operatorIndex2(Variable *variable) {
			return *(operatorReferenceIndex(variable));
		};

		void VariableAssociativeArray::initMemory() {
			TMemory<Variable>::initMemory();
			TMemory<AssociativeArray>::initMemory();
		};

		Variable *VariableAssociativeArray::clone(SymbolList &inSymbolList) {
			uint32_t k;
			VariableAssociativeArray *out = (VariableAssociativeArray *)newVariable();
			Variable *newKey;
			for(k = 0; k < value->length(); ++k) {
				newKey = ((*(value->arrayKey))[k])->clone(inSymbolList);
				out->value->mapKey->set(newKey, k);
				((*(out->value->arrayKey))[k])=newKey;
				((*(out->value->arrayValue))[k])=((*(value->arrayValue))[k])->clone(inSymbolList);
			};
			return out;
		};

		bool VariableAssociativeArray::toBoolean() {
			return true;
		};

		String VariableAssociativeArray::toString() {
			return strTypeAssociativeArray;
		};

	};
};



