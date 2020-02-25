//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEOPERATOR31_HPP
#define QUANTUM_SCRIPT_VARIABLEOPERATOR31_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PROGRAMCOUNTER_HPP
#include "quantum-script-programcounter.hpp"
#endif


namespace Quantum {
	namespace Script {

		class VariableOperator31;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableOperator31>:
			public TMemoryPoolActive<Quantum::Script::VariableOperator31> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableOperator31 :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableOperator31);
				QUANTUM_SCRIPT_EXPORT static const char *typeOperator31Key;
				QUANTUM_SCRIPT_EXPORT static const void *typeOperator31;
			public:

				Symbol symbol;
				Number value;
				ProgramCounter *pc;

				inline VariableOperator31() {
					variableType = registerType(typeOperator31, typeOperator31Key);
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableOperator31(const Variable *value) {
					if(typeOperator31 == nullptr) {
						typeOperator31 = registerType(typeOperator31, typeOperator31Key);
					};
					return (value->variableType == typeOperator31);
				};

		};

	};
};


#endif

