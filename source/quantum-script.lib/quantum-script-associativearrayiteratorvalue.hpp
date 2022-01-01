//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_ASSOCIATIVEARRAYITERATORVALUE_HPP
#define QUANTUM_SCRIPT_ASSOCIATIVEARRAYITERATORVALUE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLEASSOCIATIVEARRAY_HPP
#include "quantum-script-variableassociativearray.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ITERATOR_HPP
#include "quantum-script-iterator.hpp"
#endif

namespace Quantum {
	namespace Script {

		class AssociativeArrayIteratorValue;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::AssociativeArrayIteratorValue>:
			public TMemoryPoolActive<Quantum::Script::AssociativeArrayIteratorValue> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class AssociativeArrayIteratorValue :
			public Iterator {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(AssociativeArrayIteratorValue);
			public:

				Integer index;
				TPointer<VariableAssociativeArray> sourceArray;

				inline AssociativeArrayIteratorValue() {
				};

				QUANTUM_SCRIPT_EXPORT bool next(Variable *out);

				inline void activeDestructor() {
					sourceArray.deleteMemory();
				};

		};

	};
};



#endif
