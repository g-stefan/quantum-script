//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-variableboolean.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablenull.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableBoolean, "{382D25CC-26F8-45AF-B763-D28B2206CF07}");
		const char *VariableBoolean::strTrue = "true";
		const char *VariableBoolean::strFalse = "false";
		const char *VariableBoolean::strTypeBoolean = "Boolean";

		VariableBoolean::VariableBoolean() {
			XYO_DYNAMIC_TYPE_PUSH(VariableBoolean);
		};

		String VariableBoolean::getVariableType() {
			return strTypeBoolean;
		};

		Variable *VariableBoolean::newVariable(bool value) {
			VariableBoolean *retV;
			retV = TMemory<VariableBoolean>::newMemory();
			retV->value = value;
			return (Variable *)retV;
		};

		Variable *VariableBoolean::instancePrototype() {
			return (Context::getPrototypeBoolean())->prototype;
		};

		Variable *VariableBoolean::clone(SymbolList &inSymbolList) {
			return newVariable(value);
		};

		bool VariableBoolean::toBoolean() {
			return value;
		};

		Number VariableBoolean::toNumber() {
			return (value);
		};

		String VariableBoolean::toString() {
			if (value) {
				return strTrue;
			};
			return strFalse;
		};

	};
};
