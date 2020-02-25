//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLENULL_HPP
#define QUANTUM_SCRIPT_VARIABLENULL_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableNull;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableNull>:
			public TMemoryPoolActive<Quantum::Script::VariableNull> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class VariableNull :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableNull);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeNull;
				QUANTUM_SCRIPT_EXPORT static const char *typeNullKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeNull;
			public:

				inline VariableNull() {
					variableType = registerType(typeNull, typeNullKey);
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT Number toNumber();
				QUANTUM_SCRIPT_EXPORT String toString();

				//
				inline static bool isVariableNull(const Variable *value) {
					if(typeNull == nullptr) {
						typeNull = registerType(typeNull, typeNullKey);
					};
					return (value->variableType == typeNull);
				};

		};


	};
};



#endif

