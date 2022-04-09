//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEOPERATOR21_HPP
#define QUANTUM_SCRIPT_VARIABLEOPERATOR21_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#	include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableOperator21;
	};
};

namespace XYO {
	namespace ManagedMemory {
		template <>
		class TMemory<Quantum::Script::VariableOperator21> : public TMemoryPoolActive<Quantum::Script::VariableOperator21> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableOperator21 : public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableOperator21);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableOperator21);

			public:
				Symbol symbol;
				Number value;

				QUANTUM_SCRIPT_EXPORT VariableOperator21();

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();
		};

	};
};

#endif
