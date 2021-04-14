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
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeArray;
				QUANTUM_SCRIPT_EXPORT static const char *typeArrayKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeArray;
				TPointer<Variable> vLength;
			public:
				TPointerX<Array> value;

				inline VariableArray() {
					variableType = registerType(typeArray, typeArrayKey);
					value.pointerLink(this);
					value.newMemory();
				};

				inline void activeDestructor() {
					value->activeDestructor();
					vLength.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable();

				inline static VariableArray *newArray() {
					return TMemory<VariableArray>::newMemory();
				};

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT TPointerX<Variable> &operatorIndex(uint32_t index);
				QUANTUM_SCRIPT_EXPORT Variable &operatorReference(Symbol symbolId);

				inline TPointerX<Variable> &index(uint32_t index_) {
					TPointerX<Variable> &retV = (*value)[index_];
					if(!retV) {
						retV=VariableUndefined::newVariable();
					};
					return retV;
				};

				QUANTUM_SCRIPT_EXPORT Variable *instancePrototype();
				QUANTUM_SCRIPT_EXPORT bool operatorDeleteIndex(Variable *variable);
				QUANTUM_SCRIPT_EXPORT Variable &operatorIndex2(Variable *variable);
				QUANTUM_SCRIPT_EXPORT TPointerX<Variable> &operatorReferenceIndex(Variable *variable);
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorKey();
				QUANTUM_SCRIPT_EXPORT TPointer<Iterator> getIteratorValue();
				QUANTUM_SCRIPT_EXPORT bool hasProperty(Variable *variable);

				QUANTUM_SCRIPT_EXPORT static void initMemory();

				QUANTUM_SCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

				QUANTUM_SCRIPT_EXPORT bool toBoolean();
				QUANTUM_SCRIPT_EXPORT String toString();

				QUANTUM_SCRIPT_EXPORT String join(String with_);

				//
				inline static bool isVariableArray(const Variable *value) {
					if(typeArray == nullptr) {
						typeArray = registerType(typeArray, typeArrayKey);
					};
					return (value->variableType == typeArray);
				};

		};

	};
};



#endif
