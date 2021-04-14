//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEREFERENCEOBJECT_HPP
#define QUANTUM_SCRIPT_VARIABLEREFERENCEOBJECT_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableReferenceObject;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableReferenceObject>:
			public TMemoryPoolActive<Quantum::Script::VariableReferenceObject> {};
	};
};


namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableReferenceObject :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableReferenceObject);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeReferenceObject;
				QUANTUM_SCRIPT_EXPORT static const char *typeReferenceObjectKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeReferenceObject;
			public:

				TPointerX<Variable> *value;

				inline VariableReferenceObject() {
					variableType = registerType(typeReferenceObject, typeReferenceObjectKey);
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(TPointerX<Variable> *value);

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableReferenceObject(const Variable *value) {
					if(typeReferenceObject == nullptr) {
						typeReferenceObject = registerType(typeReferenceObject, typeReferenceObjectKey);
					};
					return (value->variableType == typeReferenceObject);
				};

		};

	};
};


#endif
