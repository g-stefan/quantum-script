//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXECUTIVECONTEXTFUNCTION_HPP
#define QUANTUM_SCRIPT_EXECUTIVECONTEXTFUNCTION_HPP

#ifndef QUANTUM_SCRIPT__DEPENDENCY_HPP
#	include "quantum-script--dependency.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXECUTIVECONTEXTPC_HPP
#	include "quantum-script-executivecontextpc.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEFUNCTION_HPP
#	include "quantum-script-variablefunction.hpp"
#endif

namespace Quantum {
	namespace Script {

		class ExecutiveContextFunction;
	};
};

namespace XYO {
	namespace ManagedMemory {
		template <>
		class TMemory<Quantum::Script::ExecutiveContextFunction> : public TMemoryPoolActive<Quantum::Script::ExecutiveContextFunction> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class ExecutiveContextFunction : public ExecutiveContextPc {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(ExecutiveContextFunction);

			public:
				TPointerX<Variable> thisFunction_;
				TPointerX<Variable> this_;

				TPointerX<VariableArray> functionArguments;
				TPointerX<VariableArray> functionLocalVariables;

#ifndef QUANTUM_SCRIPT_DISABLE_CLOSURE
				TPointerX<FunctionParent> functionParent;
#endif

				inline ExecutiveContextFunction() {

					functionContext = this;

					thisFunction_.pointerLink(this);
					this_.pointerLink(this);
					functionArguments.pointerLink(this);
					functionLocalVariables.pointerLink(this);

#ifndef QUANTUM_SCRIPT_DISABLE_CLOSURE
					functionParent.pointerLink(this);
#endif
				};

				inline void activeConstructor() {
					ExecutiveContextPc::activeConstructor();
				};

				inline void activeDestructor() {
					ExecutiveContextPc::activeDestructor();
					//
					thisFunction_.deleteMemory();
					this_.deleteMemory();
					functionArguments.deleteMemory();
					functionLocalVariables.deleteMemory();

#ifndef QUANTUM_SCRIPT_DISABLE_CLOSURE
					functionParent.deleteMemory();
#endif
				};

				inline static void initMemory() {
					TMemory<ExecutiveContextPc>::initMemory();
				};
		};

	};
};

#endif
