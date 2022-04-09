//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEREFERENCEINDEX_HPP
#define QUANTUM_SCRIPT_VARIABLEREFERENCEINDEX_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#	include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_SYMBOLLIST_HPP
#	include "quantum-script-symbollist.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableReferenceIndex;
	};
};

namespace XYO {
	namespace ManagedMemory {
		template <>
		class TMemory<Quantum::Script::VariableReferenceIndex> : public TMemoryPoolActive<Quantum::Script::VariableReferenceIndex> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableReferenceIndex : public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableReferenceIndex);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableReferenceIndex);

			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeReferenceObjectIndex;

			public:
				TPointer<Variable> object;
				size_t index;

				QUANTUM_SCRIPT_EXPORT VariableReferenceIndex();

				inline void activeDestructor() {
					object.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(Variable *object, size_t index);

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
