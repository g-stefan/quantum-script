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

#include "quantum-script-variablereferenceobject.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableReferenceObject::typeReferenceObjectKey = "{64BF83CC-B7E8-4E07-A180-C0E78FFA0D3F}";
		const void *VariableReferenceObject::typeReferenceObject;
		const char *VariableReferenceObject::strTypeReferenceObject = "ReferenceObject";

		String VariableReferenceObject::getType() {
			return strTypeReferenceObject;
		};

		Variable *VariableReferenceObject::newVariable(TPointerX<Variable> *value) {
			VariableReferenceObject *retV;
			retV = TMemory<VariableReferenceObject>::newMemory();
			retV->value = value;
			return (Variable *) retV;
		};

		bool VariableReferenceObject::toBoolean() {
			return true;
		};


		String VariableReferenceObject::toString() {
			return strTypeReferenceObject;
		};

	};
};


