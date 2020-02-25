//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
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
			protected:
				QUANTUM_SCRIPT_EXPORT static const char *strTypeStackTrace;
				QUANTUM_SCRIPT_EXPORT static const char *typeStackTraceKey;
				QUANTUM_SCRIPT_EXPORT static const void *typeStackTrace;
			public:

				TPointer<TDoubleEndedQueue<InstructionTrace> > stackTrace;
				InstructionContext *context;
				int configPrintStackTraceLimit;

				inline VariableStackTrace() {
					variableType = registerType(typeStackTrace, typeStackTraceKey);
					configPrintStackTraceLimit = QUANTUM_SCRIPT_DEFAULT_STACK_TRACE_LEVEL;
				};

				inline void activeDestructor() {
					stackTrace.deleteMemory();
				};

				QUANTUM_SCRIPT_EXPORT static Variable *newVariable(TDoubleEndedQueue<InstructionTrace> *stackTrace, InstructionContext *context);

				QUANTUM_SCRIPT_EXPORT String getType();

				QUANTUM_SCRIPT_EXPORT String toString();
				QUANTUM_SCRIPT_EXPORT String toString(int level_);


				QUANTUM_SCRIPT_EXPORT bool toBoolean();

				//
				inline static bool isVariableStackTrace(const Variable *value) {
					if(typeStackTrace == nullptr) {
						typeStackTrace = registerType(typeStackTrace, typeStackTraceKey);
					};
					return (value->variableType == typeStackTrace);
				};

		};

	};
};


#endif

