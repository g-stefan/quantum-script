//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_ARRAYITERATORKEY_HPP
#define QUANTUM_SCRIPT_ARRAYITERATORKEY_HPP

#ifndef QUANTUM_SCRIPT_VARIABLEARRAY_HPP
#include "quantum-script-variablearray.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ITERATOR_HPP
#include "quantum-script-iterator.hpp"
#endif

namespace Quantum {
	namespace Script {
		class ArrayIteratorKey;
	};
};

namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::ArrayIteratorKey>:
			public TMemoryPoolActive<Quantum::Script::ArrayIteratorKey> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class ArrayIteratorKey :
			public Iterator {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(ArrayIteratorKey);
			public:

				Integer index;
				TPointer<VariableArray> sourceArray;

				inline ArrayIteratorKey() {
				};

				QUANTUM_SCRIPT_EXPORT bool next(TPointerX<Variable> &out);

				inline void activeDestructor() {
					sourceArray.deleteMemory();
				};

		};

	};
};

#endif

