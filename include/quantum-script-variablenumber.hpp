//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLENUMBER_HPP
#define QUANTUM_SCRIPT_VARIABLENUMBER_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableNumber;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableNumber>:
			public TMemoryPoolActive<Quantum::Script::VariableNumber> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableNumber :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableNumber);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeNumber;
				QUANTUM_SCRIPT_EXPORT static const char *typeNumberKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeNumber;
			public:

				Number value;

				inline VariableNumber() {
					variableType = registerType(typeNumber, typeNumberKey);
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(Number value);

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT Variable &operatorReference(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);


				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT Number toNumber();
				QUANTUM_SCRIPT_EXPORT String toString();

				QUANTUM_SCRIPT_EXPORT static String toStringX(Number value_);

				//
				inline static bool isVariableNumber(const Variable *value) {
					if(typeNumber == nullptr) {
						typeNumber = registerType(typeNumber, typeNumberKey);
					};
					return (value->variableType == typeNumber);
				};

		};
	};
};


#endif

