//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEBOOLEAN_HPP
#define QUANTUM_SCRIPT_VARIABLEBOOLEAN_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableBoolean;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableBoolean>:
			public TMemoryPoolActive<Quantum::Script::VariableBoolean> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableBoolean :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableBoolean);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableBoolean);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTrue;
				QUANTUM_SCRIPT_EXPORT static const char *strFalse;
				QUANTUM_SCRIPT_EXPORT static const char *strTypeBoolean;
			public:

				Boolean value;

				QUANTUM_SCRIPT_EXPORT  VariableBoolean();

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(bool value);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT Number toNumber();
				QUANTUM_SCRIPT_EXPORT String toString();

		};

	};
};


#endif
