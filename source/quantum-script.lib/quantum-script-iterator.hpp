//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_ITERATOR_HPP
#define QUANTUM_SCRIPT_ITERATOR_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class Iterator;

	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Iterator>:
			public TMemoryPoolActive<Quantum::Script::Iterator> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class Iterator :
			public virtual Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(Iterator);
			public:

				inline Iterator() {
				};

				QUANTUM_SCRIPT_EXPORT virtual bool next(Variable *out);
		};
	};
};



#endif
