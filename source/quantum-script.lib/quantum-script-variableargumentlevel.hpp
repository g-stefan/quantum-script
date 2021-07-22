//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEARGUMENTLEVEL_HPP
#define QUANTUM_SCRIPT_VARIABLEARGUMENTLEVEL_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableArgumentLevel;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableArgumentLevel>:
			public TMemoryPoolActive<Quantum::Script::VariableArgumentLevel> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableArgumentLevel :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableArgumentLevel);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableArgumentLevel);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeArgumentLevel;
			public:

				int value;
				int level;

				QUANTUM_SCRIPT_EXPORT  VariableArgumentLevel();

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(int value, int level);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

		};

	};
};


#endif
