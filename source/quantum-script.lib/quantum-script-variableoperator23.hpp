﻿//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEOPERATOR23_HPP
#define QUANTUM_SCRIPT_VARIABLEOPERATOR23_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PROGRAMCOUNTER_HPP
#include "quantum-script-programcounter.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableOperator23;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableOperator23>:
			public TMemoryPoolActive<Quantum::Script::VariableOperator23> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableOperator23 :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableOperator23);
				QUANTUM_SCRIPT_EXPORT static const char *typeOperator23Key;
				QUANTUM_SCRIPT_EXPORT static const void *typeOperator23;
			public:

				ProgramCounter *linkBegin;
				ProgramCounter *pc;

				inline VariableOperator23() {
					variableType = registerType(typeOperator23, typeOperator23Key);
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableOperator23(const Variable *value) {
					if(typeOperator23 == nullptr) {
						typeOperator23 = registerType(typeOperator23, typeOperator23Key);
					};
					return (value->variableType == typeOperator23);
				};

		};

	};
};


#endif

