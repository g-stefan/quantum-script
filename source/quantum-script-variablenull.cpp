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

#include "quantum-script-variablenull.hpp"
#include "quantum-script-context.hpp"


namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableNull::typeNullKey = "{BD1A147D-2E59-4583-9E4B-C815A3EF5543}";
		const void *VariableNull::typeNull;
		const char *VariableNull::strTypeNull = "null";

		String VariableNull::getType() {
			return strTypeNull;
		};

		Variable *VariableNull::newVariable() {
			return (Variable *) TMemory<VariableNull>::newMemory();
		};

		Variable *VariableNull::clone(SymbolList &inSymbolList) {
			return newVariable();
		};

		bool VariableNull::toBoolean() {
			return false;
		};

		Number VariableNull::toNumber() {
			return 0;
		};

		String VariableNull::toString() {
			return strTypeNull;
		};


	};
};


