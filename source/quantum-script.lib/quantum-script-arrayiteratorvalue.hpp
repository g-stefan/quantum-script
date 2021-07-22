//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_ARRAYITERATORVALUE_HPP
#define QUANTUM_SCRIPT_ARRAYITERATORVALUE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLEARRAY_HPP
#include "quantum-script-variablearray.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ITERATOR_HPP
#include "quantum-script-iterator.hpp"
#endif

namespace Quantum {
	namespace Script {
		class ArrayIteratorValue;
	};
};

namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::ArrayIteratorValue>:
			public TMemoryPoolActive<Quantum::Script::ArrayIteratorValue> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class ArrayIteratorValue :
			public Iterator {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(ArrayIteratorValue);
			public:

				Integer index;
				TPointer<VariableArray> sourceArray;

				inline ArrayIteratorValue() {
				};

				QUANTUM_SCRIPT_EXPORT bool next(Variable *out);

				inline void activeDestructor() {
					sourceArray.deleteMemory();
				};
		};

	};
};


#endif
