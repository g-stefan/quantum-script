//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXECUTIVECONTEXT_HPP
#define QUANTUM_SCRIPT_EXECUTIVECONTEXT_HPP

#ifndef QUANTUM_SCRIPT_EXECUTIVECONTEXTPC_HPP
#include "quantum-script-executivecontextpc.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXECUTIVECONTEXTFUNCTION_HPP
#include "quantum-script-executivecontextfunction.hpp"
#endif

namespace Quantum {
	namespace Script {

		class ExecutiveContext;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::ExecutiveContext>:
			public TMemoryPoolActive<Quantum::Script::ExecutiveContext> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class ExecutiveContext :
			public TStackPointerUnsafe<ExecutiveContextPc, TMemoryPoolActive> {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(ExecutiveContext);
			public:

				inline ExecutiveContext() {};

				inline void enter(ExecutiveContextPc *&out) {
					out = TMemory<ExecutiveContextPc>::newMemory();
					out->functionContext = head->value->functionContext;
					push(out);
				};

				inline void enterMaster(ExecutiveContextFunction *&out) {
					out = TMemory<ExecutiveContextFunction>::newMemory();
					push(out);
				};

				inline void leave(ExecutiveContextPc *&out, ExecutiveContextFunction *&outFunction) {
					pop();
					if (head) {
						out = head->value;
						outFunction = out->functionContext;
					} else {
						out = nullptr;
						outFunction = nullptr;
					};
				};

				inline static void initMemory() {
					TMemory<TStackPointerUnsafe<ExecutiveContextPc, TMemoryPoolActive> >::initMemory();
					TMemory<ExecutiveContextFunction>::initMemory();
				};

				inline void activeDestructor() {
					TStackPointerUnsafe<ExecutiveContextPc, TMemoryPoolActive>::activeDestructor();
				};
		};

	};
};


#endif
