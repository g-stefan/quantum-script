//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_FUNCTIONPARENT_HPP
#define QUANTUM_SCRIPT_FUNCTIONPARENT_HPP

#ifndef QUANTUM_SCRIPT_VARIABLEARRAY_HPP
#include "quantum-script-variablearray.hpp"
#endif

namespace Quantum {
	namespace Script {

		class FunctionParent;

	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::FunctionParent>:
			public TMemoryPoolActive<Quantum::Script::FunctionParent> {};
	};
};


namespace Quantum {
	namespace Script {

		using namespace XYO;

		class FunctionParent:
			public Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(FunctionParent);
			public:

				TPointerX<FunctionParent> functionParent;
				TPointerX<VariableArray> variables;
				TPointerX<VariableArray> arguments;

				inline FunctionParent() {
					functionParent.pointerLink(this);
					variables.pointerLink(this);
					arguments.pointerLink(this);
				};

				inline void activeDestructor() {
					functionParent.deleteMemory();
					variables.deleteMemory();
					arguments.deleteMemory();
				};

		};

	};
};


#endif
