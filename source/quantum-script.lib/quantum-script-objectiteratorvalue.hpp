//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_OBJECTITERATORVALUE_HPP
#define QUANTUM_SCRIPT_OBJECTITERATORVALUE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLEOBJECT_HPP
#include "quantum-script-variableobject.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ITERATOR_HPP
#include "quantum-script-iterator.hpp"
#endif

namespace Quantum {
	namespace Script {

		class ObjectIteratorValue;

	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::ObjectIteratorValue>:
			public TMemoryPoolActive<Quantum::Script::ObjectIteratorValue> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class ObjectIteratorValue :
			public Iterator {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(ObjectIteratorValue);
			public:

				TPointer<Variable> value_;
				PropertyNode *value;
				TRedBlackTree<uint32_t, String, TMemoryPoolActive > *mirrorList;

				inline ObjectIteratorValue() {
				};

				QUANTUM_SCRIPT_EXPORT bool next(Variable *out);

				inline void activeDestructor() {
					value_.deleteMemory();
				};


		};

	};
};


#endif
