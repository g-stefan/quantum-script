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

#include "quantum-script-variableresource.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variablenull.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-variablenumber.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableResource::typeResourceKey = "{B3910AC4-B98C-4890-94FB-BA5B38F24621}";
		const void *VariableResource::typeResource;
		const char *VariableResource::strTypeResource = "Resource";

		String VariableResource::getType() {
			return strTypeResource;
		};

		Variable *VariableResource::newVariable(void *resource, ResourceDelete resourceDelete) {
			VariableResource *retV;
			retV = TMemory<VariableResource>::newMemory();
			retV->resource = resource;
			retV->resourceDelete = resourceDelete;
			return (Variable *) retV;
		};

		Variable *VariableResource::newVariable2(void *resource, ResourceDelete resourceDelete, const void *resourceType) {
			VariableResource *retV;
			retV = TMemory<VariableResource>::newMemory();
			retV->resource = resource;
			retV->resourceDelete = resourceDelete;
			retV->resourceType = resourceType;
			return (Variable *) retV;
		};

		Variable *VariableResource::instancePrototype() {
			return (Context::getPrototypeResource())->prototype;
		};

		bool VariableResource::toBoolean() {
			return true;
		};


		String VariableResource::toString() {
			return strTypeResource;
		};

	};
};



