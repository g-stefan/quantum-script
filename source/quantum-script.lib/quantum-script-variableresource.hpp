//
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
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableResource);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeResource;
			public:

				typedef void (*ResourceDelete)(void *);

				void *resource;
				ResourceDelete resourceDelete;

				QUANTUM_SCRIPT_EXPORT VariableResource();

				inline void drop() {
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
				};

				inline void activeDestructor() {
					close();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(void *resource, ResourceDelete resourceDelete);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

		};

	};
};


#endif
