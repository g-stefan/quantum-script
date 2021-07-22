//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_ASSOCIATIVEARRAYITERATORKEY_HPP
#define QUANTUM_SCRIPT_ASSOCIATIVEARRAYITERATORKEY_HPP

#ifndef QUANTUM_SCRIPT_VARIABLEASSOCIATIVEARRAY_HPP
#include "quantum-script-variableassociativearray.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ITERATOR_HPP
#include "quantum-script-iterator.hpp"
#endif

namespace Quantum {
	namespace Script {

		class AssociativeArrayIteratorKey;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::AssociativeArrayIteratorKey>:
			public TMemoryPoolActive<Quantum::Script::AssociativeArrayIteratorKey> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class AssociativeArrayIteratorKey :
			public Iterator {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(AssociativeArrayIteratorKey);
			public:

				Integer index;
				TPointer<VariableAssociativeArray> sourceArray;

				inline AssociativeArrayIteratorKey() {
				};

				QUANTUM_SCRIPT_EXPORT bool next(Variable *out);

				inline void activeDestructor() {
					sourceArray.deleteMemory();
				};

		};

	};
};


#endif

