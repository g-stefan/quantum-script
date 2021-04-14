//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_EXECUTIVECONTEXTPC_HPP
#define QUANTUM_SCRIPT_EXECUTIVECONTEXTPC_HPP

#ifndef QUANTUM_SCRIPT__DEPENDENCY_HPP
#include "quantum-script--dependency.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PROGRAMCOUNTER_HPP
#include "quantum-script-programcounter.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_ITERATOR_HPP
#include "quantum-script-iterator.hpp"
#endif

namespace Quantum {
	namespace Script {

		class ExecutiveContextPc;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::ExecutiveContextPc>:
			public TMemoryPoolActive<Quantum::Script::ExecutiveContextPc> {};
	};
};


namespace Quantum {
	namespace Script {

		using namespace XYO;

		class ExecutiveContextFunction;

		class ExecutiveContextPc :
			public Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(ExecutiveContextPc);
			public:
				ProgramCounter *catch_;
				ProgramCounter *finally_;
				ProgramCounter *break_;
				ProgramCounter *continue_;
				ProgramCounter *tryBreak_;
				ProgramCounter *tryContinue_;
				ProgramCounter *tryReturn_;
				ProgramCounter *tryThrow_;
				ProgramCounter *pc_;

				Stack::Node *stackLink_;

				ExecutiveContextFunction *functionContext;

				TPointerX<Variable>  registerValue;
				TPointerX<Iterator>  iteratorValue;
				TPointerX<Variable> *referenceValue;

				inline ExecutiveContextPc() {
					registerValue.pointerLink(this);
					iteratorValue.pointerLink(this);

					functionContext = nullptr;

					catch_ = nullptr;
					finally_ = nullptr;
					break_ = nullptr;
					continue_ = nullptr;
					tryBreak_ = nullptr;
					tryContinue_ = nullptr;
					tryReturn_ = nullptr;
					tryThrow_ = nullptr;
					pc_ = nullptr;
					stackLink_ = nullptr;
					//
					referenceValue = nullptr;
				};

				inline void activeConstructor() {
					catch_ = nullptr;
					finally_ = nullptr;
					break_ = nullptr;
					continue_ = nullptr;
					tryBreak_ = nullptr;
					tryContinue_ = nullptr;
					tryReturn_ = nullptr;
					tryThrow_ = nullptr;
					pc_ = nullptr;
					stackLink_ = nullptr;
					//
					referenceValue = nullptr;
				};

				inline void activeDestructor() {
					registerValue.deleteMemory();
					iteratorValue.deleteMemory();
				};

				inline static void initMemory() {
					TMemory<Variable>::initMemory();
					TMemory<Iterator>::initMemory();
				};
		};

	};
};


#endif
