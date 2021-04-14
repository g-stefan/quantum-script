//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEOBJECT_HPP
#define QUANTUM_SCRIPT_VARIABLEOBJECT_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PROTOTYPE_HPP
#include "quantum-script-prototype.hpp"
#endif

namespace Quantum {
	namespace Script {


		class VariableObject;

	};
};

namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableObject>:
			public TMemoryPoolActive<Quantum::Script::VariableObject> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableObject :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableObject);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeObject;
				QUANTUM_SCRIPT_EXPORT static const char *typeObjectKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeObject;
			public:

				TPointerX<Property> value;
				TPointerX<Prototype> prototype;

				inline VariableObject() {
					value.pointerLink(this);
					prototype.pointerLink(this);
					variableType = registerType(typeObject, typeObjectKey);
					value.newMemory();
					prototype.newMemory();
				};

				inline ~VariableObject() {
				};

				inline void activeConstructor() {
					prototype.newMemory();
					value.newMemory();
				};

				inline void activeDestructor() {
					prototype.deleteMemory();
					value.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT TPointerX<Variable> &operatorReferenceOwnProperty(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT Variable &operatorReference(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();
				QUANTUM_SCRIPT_EXPORT bool findOwnProperty(Symbol symbolId, Variable *&out);
				QUANTUM_SCRIPT_EXPORT bool operatorDeleteIndex(Variable *variable);
				QUANTUM_SCRIPT_EXPORT bool operatorDeleteOwnProperty(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT Variable &operatorIndex2(Variable *variable);
				QUANTUM_SCRIPT_EXPORT TPointerX<Variable> &operatorReferenceIndex(Variable *variable);

				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorValue();

				QUANTUM_SCRIPT_EXPORT static void initMemory();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);
				QUANTUM_SCRIPT_EXPORT bool hasProperty(Variable *variable);

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableObject(const Variable *value) {
					if(typeObject == nullptr) {
						typeObject = registerType(typeObject, typeObjectKey);
					};
					return (value->variableType == typeObject);
				};

		};

	};
};


#endif


