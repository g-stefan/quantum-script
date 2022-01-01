//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
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
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableObject);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeObject;
			public:

				TPointerX<Property> value;
				TPointerX<Prototype> prototype;

				QUANTUM_SCRIPT_EXPORT  VariableObject();

				inline void activeConstructor() {
					prototype.newMemory();
					value.newMemory();
				};

				inline void activeDestructor() {
					prototype.deleteMemory();
					value.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyByIndex(size_t index);
				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyByVariable(Variable *index);
				QUANTUM_SCRIPT_EXPORT void setPropertyBySymbol(Symbol symbolId, Variable *value);
				QUANTUM_SCRIPT_EXPORT void setPropertyByIndex(size_t index, Variable *value);
				QUANTUM_SCRIPT_EXPORT void setPropertyByVariable(Variable *index, Variable *value);
				QUANTUM_SCRIPT_EXPORT bool deletePropertyBySymbol(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT bool deletePropertyByIndex(size_t index);
				QUANTUM_SCRIPT_EXPORT bool deletePropertyByVariable(Variable *index);

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();

				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorValue();

				QUANTUM_SCRIPT_EXPORT static void initMemory();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);
				QUANTUM_SCRIPT_EXPORT bool hasPropertyByVariable(Variable *variable);

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

		};

	};
};


#endif


