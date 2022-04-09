//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLESYMBOL_HPP
#define QUANTUM_SCRIPT_VARIABLESYMBOL_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#	include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_SYMBOLLIST_HPP
#	include "quantum-script-symbollist.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableSymbol;
	};
};

namespace XYO {
	namespace ManagedMemory {
		template <>
		class TMemory<Quantum::Script::VariableSymbol> : public TMemoryPoolActive<Quantum::Script::VariableSymbol> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableSymbol : public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableSymbol);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableSymbol);

			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeSymbol;

			public:
				Symbol value;

				QUANTUM_SCRIPT_EXPORT VariableSymbol();

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(Symbol value);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

				QUANTUM_SCRIPT_EXPORT String toString();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT Number toNumber();
				QUANTUM_SCRIPT_EXPORT bool isString();
		};

	};
};

#endif
