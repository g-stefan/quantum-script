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
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeArgumentLevel;
				QUANTUM_SCRIPT_EXPORT static const char *typeArgumentLevelKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeArgumentLevel;
			public:

				int value;
				int level;

				inline VariableArgumentLevel() {
					variableType = registerType(typeArgumentLevel, typeArgumentLevelKey);
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(int value, int level);

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();
				//
				inline static bool isVariableArgumentLevel(const Variable *value) {
					if(typeArgumentLevel == nullptr) {
						typeArgumentLevel = registerType(typeArgumentLevel, typeArgumentLevelKey);
					};
					return (value->variableType == typeArgumentLevel);
				};

		};

	};
};


#endif
