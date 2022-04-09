//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEVMPROGRAMCOUNTER_HPP
#define QUANTUM_SCRIPT_VARIABLEVMPROGRAMCOUNTER_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#	include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PROGRAMCOUNTER_HPP
#	include "quantum-script-programcounter.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableVmProgramCounter;
	};
};

namespace XYO {
	namespace ManagedMemory {
		template <>
		class TMemory<Quantum::Script::VariableVmProgramCounter> : public TMemoryPoolActive<Quantum::Script::VariableVmProgramCounter> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableVmProgramCounter : public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableVmProgramCounter);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableVmProgramCounter);

			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeVmProgramCounter;

			public:
				ProgramCounter *value;

				QUANTUM_SCRIPT_EXPORT VariableVmProgramCounter();

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(ProgramCounter *value);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();
		};

	};
};

#endif
