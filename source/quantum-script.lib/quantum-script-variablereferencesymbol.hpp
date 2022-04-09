//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEREFERENCESYMBOL_HPP
#define QUANTUM_SCRIPT_VARIABLEREFERENCESYMBOL_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#	include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_SYMBOLLIST_HPP
#	include "quantum-script-symbollist.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableReferenceSymbol;
	};
};

namespace XYO {
	namespace ManagedMemory {
		template <>
		class TMemory<Quantum::Script::VariableReferenceSymbol> : public TMemoryPoolActive<Quantum::Script::VariableReferenceSymbol> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableReferenceSymbol : public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableReferenceSymbol);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableReferenceSymbol);

			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeReferenceObjectSymbol;

			public:
				TPointer<Variable> object;
				Symbol symbol;

				QUANTUM_SCRIPT_EXPORT VariableReferenceSymbol();

				inline void activeDestructor() {
					object.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(Variable *object, Symbol symbol);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> referenceSet(Variable *value);
				QUANTUM_SCRIPT_EXPORT TPointer<Variable> referenceGet();
				QUANTUM_SCRIPT_EXPORT Number referenceToNumber();
				QUANTUM_SCRIPT_EXPORT String referenceToString();
				QUANTUM_SCRIPT_EXPORT TPointer<Variable> referenceOperatorPlus(Variable *b);
				QUANTUM_SCRIPT_EXPORT void referenceSetA1(Variable *value);
		};

	};
};

#endif
