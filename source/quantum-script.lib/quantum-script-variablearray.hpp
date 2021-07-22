//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLEARRAY_HPP
#define QUANTUM_SCRIPT_VARIABLEARRAY_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableArray;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableArray>:
			public TMemoryPoolActive<Quantum::Script::VariableArray> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableArray :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableArray);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableArray);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeArray;
			public:
				TPointerX<Array> value;

				QUANTUM_SCRIPT_EXPORT VariableArray();

				inline void activeDestructor() {
					value->activeDestructor();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				inline static VariableArray *newArray() {
					return TMemory<VariableArray>::newMemory();
				};

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);
				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyByIndex(size_t index);
				QUANTUM_SCRIPT_EXPORT TPointer<Variable> getPropertyByVariable(Variable *index);
				QUANTUM_SCRIPT_EXPORT void setPropertyByIndex(size_t index, Variable *value);
				QUANTUM_SCRIPT_EXPORT void setPropertyByVariable(Variable *index, Variable *value);
				QUANTUM_SCRIPT_EXPORT bool deletePropertyByIndex(size_t index);
				QUANTUM_SCRIPT_EXPORT bool deletePropertyByVariable(Variable *index);

				inline TPointerX<Variable> &index(uint32_t index_) {
					TPointerX<Variable> &retV = (*value)[index_];
					if(!retV) {
						retV=VariableUndefined::newVariable();
					};
					return retV;
				};

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorValue();
				QUANTUM_SCRIPT_EXPORT bool hasPropertyByVariable(Variable *variable);

				QUANTUM_SCRIPT_EXPORT static void initMemory();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				QUANTUM_SCRIPT_EXPORT String join(String with_);

		};

	};
};



#endif
