//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_PROTOTYPE_HPP
#define QUANTUM_SCRIPT_PROTOTYPE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

namespace Quantum {
	namespace Script {

		class Prototype;

	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::Prototype>:
			public TMemoryPoolActive<Quantum::Script::Prototype> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class Prototype :
			public Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(Prototype);
			public:

				TPointerX<Prototype> parent;
				TPointerX<Variable> prototype;

				QUANTUM_SCRIPT_EXPORT Prototype();

				inline void activeDestructor() {
					parent.deleteMemory();
					prototype.deleteMemory();
				};

				inline static void initMemory() {
					TMemory<Variable>::initMemory();
				};
		};


	};
};


#endif
