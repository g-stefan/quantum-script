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
#include <math.h>

#include "quantum-script-variablearray.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-arrayiteratorkey.hpp"
#include "quantum-script-arrayiteratorvalue.hpp"
#include "quantum-script-variablesymbol.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableArray::typeArrayKey = "{BCC18B65-1BA1-4943-859C-82F791ED62AD}";
		const void *VariableArray::typeArray;
		const char *VariableArray::strTypeArray = "Array";

		Variable *VariableArray::newVariable() {
			return (Variable *) TMemory<VariableArray>::newMemory();
		};

		String VariableArray::getType() {
			return strTypeArray;
		};

		TPointerX<Variable> &VariableArray::operatorIndex(uint32_t index) {
			TPointerX<Variable> &retV = (*value)[index];
			if(!retV) {
				retV=VariableUndefined::newVariable();
			};
			return retV;
		};

		Variable &VariableArray::operatorReference(Symbol symbolId) {
			if(symbolId == Context::getSymbolLength()) {
				if(vLength) {
					if(vLength->hasOneReference()) {
						((VariableNumber *)vLength.value())->value = (Number)value->length();
					} else {
						vLength=VariableNumber::newVariable((Number)value->length());
					};
				} else {
					vLength=VariableNumber::newVariable((Number)value->length());
				};
				return *vLength;
			};

			return operatorReferenceX(symbolId, (Context::getPrototypeArray())->prototype);
		};


		Variable *VariableArray::instancePrototype() {
			return (Context::getPrototypeArray())->prototype;
		};

		bool VariableArray::operatorDeleteIndex(Variable *variable) {
			Number index = variable->toNumber();
			if(isnan(index) || isinf(index) || signbit(index)) {
				return false;
			};
			value->remove((Integer)(index));
			return true;
		};

		Variable &VariableArray::operatorIndex2(Variable *variable) {
			Number index = variable->toNumber();
			if(isnan(index) || isinf(index) || signbit(index)) {
				throw(Error("invalid index"));
			};
			TPointerX<Variable> &retV = (*value)[(Integer)(index)];
			if(!retV) {
				retV=VariableUndefined::newVariable();
			};
			return *retV;
		};

		TPointerX<Variable> &VariableArray::operatorReferenceIndex(Variable *variable) {
			Number index = variable->toNumber();
			if(isnan(index) || isinf(index) || signbit(index)) {
				throw(Error("invalid index"));
			};
			TPointerX<Variable> &retV = (*value)[(Integer)variable->toNumber()];
			if(!retV) {
				retV=VariableUndefined::newVariable();
			};
			return retV;
		};

		TPointer<Iterator> VariableArray::getIteratorKey() {
			TPointer<Iterator> retV;
			ArrayIteratorKey *iterator_ = TMemory<ArrayIteratorKey>::newMemory();
			iterator_->sourceArray = this;
			iterator_->index = 0;
			retV=iterator_;
			return retV;
		};

		TPointer<Iterator> VariableArray::getIteratorValue() {
			ArrayIteratorValue *iterator_ = TMemory<ArrayIteratorValue>::newMemory();
			iterator_->sourceArray = this;
			iterator_->index = 0;
			return iterator_;
		};

		void VariableArray::initMemory() {
			TMemory<Variable>::initMemory();
			TMemory<Array>::initMemory();
		};

		Variable *VariableArray::clone(SymbolList &inSymbolList) {
			int k;
			VariableArray *out = (VariableArray *)newVariable();
			for(k = 0; k < value->length(); ++k) {
				((*(out->value))[k])=((*value)[k])->clone(inSymbolList);
			};
			return out;
		};

		bool VariableArray::hasProperty(Variable *variable) {
			if(VariableNumber::isVariableNumber(variable)) {
				if((static_cast<VariableNumber *>(variable))->value < value->length()) {
					return true;
				};
			};
			if(VariableSymbol::isVariableSymbol(variable)) {
				if((static_cast<VariableSymbol *>(variable))->value == Context::getSymbolLength()) {
					return true;
				};
			};
			return (Context::getPrototypeArray())->prototype->hasProperty(variable);
		};

		bool VariableArray::toBoolean() {
			return true;
		};

		String VariableArray::toString() {
			uint32_t scan;
			String retV;
			retV = "";
			for (scan = 0; scan < value->length(); ++scan) {
				if (scan > 0) {
					retV << ",";
				};
				retV << (((*value)[scan])->toString());
			};
			return retV;
		};

		String VariableArray::join(String with_) {
			uint32_t scan;
			String retV;
			retV = "";
			for (scan = 0; scan < value->length(); ++scan) {
				if (scan > 0) {
					retV << with_;
				};
				retV << (((*value)[scan])->toString());
			};
			return retV;
		};


	};
};



