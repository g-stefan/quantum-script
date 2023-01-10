// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/VariableResource.hpp>
#include <XYO/QuantumScript/Context.hpp>
#include <XYO/QuantumScript/VariableNull.hpp>
#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>

namespace XYO::QuantumScript {

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
