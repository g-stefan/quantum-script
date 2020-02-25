//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEOPERATOR22_HPP
#define QUANTUM_SCRIPT_VARIABLEOPERATOR22_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableOperator22;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableOperator22>:
			public TMemoryPoolActive<Quantum::Script::VariableOperator22> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableOperator22 :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableOperator22);
				QUANTUM_SCRIPT_EXPORT static const char *typeOperator22Key;
				QUANTUM_SCRIPT_EXPORT static const void *typeOperator22;
			public:

				Symbol symbol1;
				Symbol symbol2;

				inline VariableOperator22() {
					variableType = registerType(typeOperator22, typeOperator22Key);
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableOperator22(const Variable *value) {
					if(typeOperator22 == nullptr) {
						typeOperator22 = registerType(typeOperator22, typeOperator22Key);
					};
					return (value->variableType == typeOperator22);
				};
		};

	};
};


#endif

