//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLENATIVEFUNCTION_HPP
#define QUANTUM_SCRIPT_VARIABLENATIVEFUNCTION_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_INSTRUCTIONX_HPP
#include "quantum-script-instructionx.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableNativeVmFunction;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableNativeVmFunction>:
			public TMemoryPoolActive<Quantum::Script::VariableNativeVmFunction> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableNativeVmFunction :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableNativeVmFunction);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableNativeVmFunction);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeFunction;
			public:

				InstructionProcedure procedure;
				TPointer<Variable> operand;

				QUANTUM_SCRIPT_EXPORT  VariableNativeVmFunction();

				inline void activeDestructor() {
					operand.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(InstructionProcedure procedure, Variable *operand);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

		};

	};
};


#endif
