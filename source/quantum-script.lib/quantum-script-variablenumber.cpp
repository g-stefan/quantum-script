﻿//
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

#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablenull.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableNumber::typeNumberKey = "{14F9D487-6647-4D2E-9C5B-23F8A486F8BD}";
		const void *VariableNumber::typeNumber;
		const char *VariableNumber::strTypeNumber = "Number";

		String VariableNumber::getType() {
			return strTypeNumber;
		};

		Variable *VariableNumber::newVariable(Number value) {
			VariableNumber *retV;
			retV = TMemory<VariableNumber>::newMemory();
			retV->value = value;
			return (Variable *) retV;
		};

		Variable &VariableNumber::operatorReference(Symbol symbolId) {
			return operatorReferenceX(symbolId, (Context::getPrototypeNumber())->prototype);
		};

		Variable *VariableNumber::instancePrototype() {
			return (Context::getPrototypeNumber())->prototype;
		};

		Variable *VariableNumber::clone(SymbolList &inSymbolList) {
			return newVariable(value);
		};

		bool VariableNumber::toBoolean() {
			if(isnan(value)) {
				return false;
			};
			if(value) {
				return true;
			};
			return false;
		};

		Number VariableNumber::toNumber() {
			return value;
		};

		String VariableNumber::toString() {
			return toStringX(value);
		};

		String VariableNumber::toStringX(Number value_) {
			char buf[128];
			if(isnan(value_)) {
				return "NaN";
			};
			if(isinf(value_)) {
				if(signbit(value_)) {
					return "-Infinity";
				} else {
					return "Infinity";
				};
			};
			if((value_ > 0 && value_ < 1e+11) || (value_ < 0 && value_ > -1e+11)) {
				Number fractpart, intpart;
				fractpart = modf(value_, &intpart);
				if(fractpart == 0.0) {
					sprintf(buf, QUANTUM_SCRIPT_FORMAT_NUMBER_INTEGER, value_);
				} else {
					sprintf(buf, QUANTUM_SCRIPT_FORMAT_NUMBER, value_);
				};
			} else {
				sprintf(buf, QUANTUM_SCRIPT_FORMAT_NUMBER, value_);
			};
			return buf;
		};

	};
};


