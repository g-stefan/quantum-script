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

#include "quantum-script-variableresource.hpp"
#include "quantum-script-context.hpp"
#include "quantum-script-variablenull.hpp"
#include "quantum-script-variablestring.hpp"
#include "quantum-script-variablenumber.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableResource, "{B3910AC4-B98C-4890-94FB-BA5B38F24621}");
		const char *VariableResource::strTypeResource = "Resource";

		VariableResource::VariableResource() {
			XYO_DYNAMIC_TYPE_PUSH(VariableResource);
			resource = nullptr;
			resourceDelete = nullptr;
		};

		String VariableResource::getVariableType() {
			return strTypeResource;
		};

		Variable *VariableResource::newVariable(void *resource, ResourceDelete resourceDelete) {
			VariableResource *retV;
			retV = TMemory<VariableResource>::newMemory();
			retV->resource = resource;
			retV->resourceDelete = resourceDelete;
			return (Variable *)retV;
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
