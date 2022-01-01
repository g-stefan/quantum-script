//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLESTRING_HPP
#define QUANTUM_SCRIPT_VARIABLESTRING_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableString;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableString>:
			public TMemoryPoolActive<Quantum::Script::VariableString> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableString :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableString);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableString);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeString;
			public:
				String value;

				QUANTUM_SCRIPT_EXPORT VariableString();

				inline void activeDestructor() {
					value.activeDestructor();
				};

				QUANTUM_SCRIPT_EXPORT  static Variable *newVariable(String value);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();

				QUANTUM_SCRIPT_EXPORT static void initMemory();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

				QUANTUM_SCRIPT_EXPORT bool hasPropertyByVariable(Variable *variable);

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT Number toNumber();
				QUANTUM_SCRIPT_EXPORT String toString();
				QUANTUM_SCRIPT_EXPORT bool isString();

		};

	};
};


#endif
