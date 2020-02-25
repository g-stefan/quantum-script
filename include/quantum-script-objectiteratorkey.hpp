//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_OBJECTITERATORKEY_HPP
#define QUANTUM_SCRIPT_OBJECTITERATORKEY_HPP

#ifndef QUANTUM_SCRIPT_VARIABLEOBJECT_HPP
#include "quantum-script-variableobject.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ITERATOR_HPP
#include "quantum-script-iterator.hpp"
#endif

namespace Quantum {
	namespace Script {

		class ObjectIteratorKey;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::ObjectIteratorKey>:
			public TMemoryPoolActive<Quantum::Script::ObjectIteratorKey> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class ObjectIteratorKey :
			public Iterator {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(ObjectIteratorKey);
			public:

				TPointer<Variable> value_;
				PropertyNode *value;

				inline ObjectIteratorKey() {
				};

				QUANTUM_SCRIPT_EXPORT bool next(TPointerX<Variable> &out);

				inline void activeDestructor() {
					value_.deleteMemory();
				};

		};

	};
};


#endif
