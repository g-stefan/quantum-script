//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_VARIABLESTACKTRACE_HPP
#define QUANTUM_SCRIPT_VARIABLESTACKTRACE_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_INSTRUCTIONCONTEXT_HPP
#include "quantum-script-instructioncontext.hpp"
#endif

namespace Quantum {
	namespace Script {

		class VariableStackTrace;
	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::VariableStackTrace>:
			public TMemoryPoolActive<Quantum::Script::VariableStackTrace> {};
	};
};

namespace Quantum {
	namespace Script {

		using namespace XYO;

		class InstructionContext;

		class VariableStackTrace :
			public Variable {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableStackTrace);
				XYO_DYNAMIC_TYPE_DEFINE(QUANTUM_SCRIPT_EXPORT, VariableStackTrace);
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeStackTrace;
			public:

				TPointer<TDoubleEndedQueue<InstructionTrace> > stackTrace;
				InstructionContext *context;
				int configPrintStackTraceLimit;

				QUANTUM_SCRIPT_EXPORT VariableStackTrace();

				inline void activeDestructor() {
					stackTrace.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(TDoubleEndedQueue<InstructionTrace> *stackTrace, InstructionContext *context);

				QUANTUM_SCRIPT_EXPORT String getVariableType();

				QUANTUM_SCRIPT_EXPORT String toString();
				QUANTUM_SCRIPT_EXPORT String toString(int level_);


				QUANTUM_SCRIPT_EXPORT bool toBoolean();

		};

	};
};


#endif

