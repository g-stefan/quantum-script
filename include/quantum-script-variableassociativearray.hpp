//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEASSOCIATIVEARRAY_HPP
#define QUANTUM_SCRIPT_VARIABLEASSOCIATIVEARRAY_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace XYO {
	namespace DataStructures {

		template<>
		class TComparator<Quantum::Script::Variable> {
			public:

				inline static bool isLess(const Quantum::Script::Variable &a, const Quantum::Script::Variable &b) {
					return ((const_cast<Quantum::Script::Variable &>(a)).compare(&(const_cast<Quantum::Script::Variable &>(b))) < 0);
				};

				inline static int compare(const Quantum::Script::Variable &a, const Quantum::Script::Variable &b) {
					return (const_cast<Quantum::Script::Variable &>(a)).compare(&(const_cast<Quantum::Script::Variable &>(b)));
				};
		};

	};
};

namespace Quantum {
	namespace Script {

		class VariableAssociativeArray;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableAssociativeArray>:
			public TMemoryPoolActive<Quantum::Script::VariableAssociativeArray> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		typedef TAssociativeArray<TPointerX<Variable>, TPointerX<Variable>, 4, TMemoryPoolActive> AssociativeArray;

		class VariableAssociativeArray :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableAssociativeArray);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeAssociativeArray;
				QUANTUM_SCRIPT_EXPORT static const char *typeAssociativeArrayKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeAssociativeArray;
				TPointer<Variable> vLength;
			public:
				TPointerX<AssociativeArray> value;

				inline VariableAssociativeArray() {
					variableType = registerType(typeAssociativeArray, typeAssociativeArrayKey);
					value.pointerLink(this);
					value.newMemory();
				};

				inline void activeDestructor() {
					value->activeDestructor();
					vLength.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT Variable &operatorReference(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT TPointerX<Variable> &operatorReferenceIndex(Variable *variable);
				QUANTUM_SCRIPT_EXPORT bool operatorDeleteIndex(Variable *variable);
				QUANTUM_SCRIPT_EXPORT Variable &operatorIndex2(Variable *variable);

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorValue();


				QUANTUM_SCRIPT_EXPORT static void initMemory();


				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableAssociativeArray(const Variable *value) {
					if(typeAssociativeArray == nullptr) {
						typeAssociativeArray = registerType(typeAssociativeArray, typeAssociativeArrayKey);
					};
					return (value->variableType == typeAssociativeArray);
				};

		};

	};
};



#endif
