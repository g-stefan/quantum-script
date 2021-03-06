﻿//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLERESOURCE_HPP
#define QUANTUM_SCRIPT_VARIABLERESOURCE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableResource;

	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableResource>:
			public TMemoryPoolActive<Quantum::Script::VariableResource> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableResource :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableResource);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeResource;
				QUANTUM_SCRIPT_EXPORT static const char *typeResourceKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeResource;
			public:

				typedef void (*ResourceDelete)(void *);

				const void *resourceType;
				void *resource;
				ResourceDelete resourceDelete;

				inline VariableResource() {
					variableType = registerType(typeResource, typeResourceKey);
					resourceType = typeResource;
					resource = nullptr;
					resourceDelete = nullptr;
				};

				inline void drop() {
					resourceType = typeResource;
					resource = nullptr;
					resourceDelete = nullptr;
				};

				inline void close() {
					if(resource != nullptr) {
						if(resourceDelete != nullptr) {
							(*resourceDelete)(resource);
							resourceDelete = nullptr;
						};
						resource = nullptr;
					};
					variableType = typeResource;
				};

				inline void activeDestructor() {
					close();
				};


				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(void *resource, ResourceDelete resourceDelete);
				QUANTUM_SCRIPT_EXPORT static Variable *newVariable2(void *resource, ResourceDelete resourceDelete, const void *resourceType);

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableResource(const Variable *value) {
					if(typeResource == nullptr) {
						typeResource = registerType(typeResource, typeResourceKey);
					};
					return (value->variableType == typeResource);
				};

		};

	};
};


#endif
