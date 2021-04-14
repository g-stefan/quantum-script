//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
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
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeFunction;
				QUANTUM_SCRIPT_EXPORT static const char *typeNativeVmFunctionKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeNativeVmFunction;
			public:

				InstructionProcedure procedure;
				TPointer<Variable> operand;

				inline VariableNativeVmFunction() {
					variableType = registerType(typeNativeVmFunction, typeNativeVmFunctionKey);
				};

				inline void activeDestructor() {
					operand.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(InstructionProcedure procedure, Variable *operand);

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableNativeVmFunction(const Variable *value) {
					if(typeNativeVmFunction == nullptr) {
						typeNativeVmFunction = registerType(typeNativeVmFunction, typeNativeVmFunctionKey);
					};
					return (value->variableType == typeNativeVmFunction);
				};

		};

	};
};


#endif
