//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLESYMBOL_HPP
#define QUANTUM_SCRIPT_VARIABLESYMBOL_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_SYMBOLLIST_HPP
#include "quantum-script-symbollist.hpp"
#endif


namespace Quantum {
	namespace Script {

		class VariableSymbol;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableSymbol>:
			public TMemoryPoolActive<Quantum::Script::VariableSymbol> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableSymbol :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableSymbol);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeSymbol;
				QUANTUM_SCRIPT_EXPORT static const char *typeSymbolKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeSymbol;

				TPointer<Variable> vLength;
			public:

				Symbol value;

				inline VariableSymbol() {
					variableType = registerType(typeSymbol, typeSymbolKey);
				};

				inline void activeDestructor() {
					vLength.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(Symbol value);

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT Variable &operatorReference(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

				QUANTUM_SCRIPT_EXPORT String toString();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT Number toNumber();
				QUANTUM_SCRIPT_EXPORT bool isString();

				//
				inline static bool isVariableSymbol(const Variable *value) {
					if(typeSymbol == nullptr) {
						typeSymbol = registerType(typeSymbol, typeSymbolKey);
					};
					return (value->variableType == typeSymbol);
				};

		};

	};
};


#endif
