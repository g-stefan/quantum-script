//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEOPERATOR21_HPP
#define QUANTUM_SCRIPT_VARIABLEOPERATOR21_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableOperator21;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableOperator21>:
			public TMemoryPoolActive<Quantum::Script::VariableOperator21> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableOperator21 :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableOperator21);
				QUANTUM_SCRIPT_EXPORT static const char *typeOperator21Key;
				QUANTUM_SCRIPT_EXPORT static const void *typeOperator21;
			public:

				Symbol symbol;
				Number value;

				inline VariableOperator21() {
					variableType = registerType(typeOperator21, typeOperator21Key);
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableOperator21(const Variable *value) {
					if(typeOperator21 == nullptr) {
						typeOperator21 = registerType(typeOperator21, typeOperator21Key);
					};
					return (value->variableType == typeOperator21);
				};

		};

	};
};


#endif

