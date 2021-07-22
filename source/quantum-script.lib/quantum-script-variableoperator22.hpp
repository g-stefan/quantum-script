//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
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
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableOperator22);
			public:

				Symbol symbol1;
				Symbol symbol2;

				QUANTUM_SCRIPT_EXPORT  VariableOperator22();

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

		};

	};
};


#endif

