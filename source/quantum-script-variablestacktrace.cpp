//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-instructioncontext.hpp"
#include "quantum-script-variablestacktrace.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		const char *VariableStackTrace::typeStackTraceKey = "{D2C5840D-48B9-4424-8BCC-39147444BC1A}";
		const void *VariableStackTrace::typeStackTrace;
		const char *VariableStackTrace::strTypeStackTrace = "StackTrace";

		Variable *VariableStackTrace::newVariable(TDoubleEndedQueue<InstructionTrace> *stackTrace, InstructionContext *context) {
			VariableStackTrace *retV;
			retV = TMemory<VariableStackTrace>::newMemory();
			retV->stackTrace = stackTrace;
			retV->context = context;
			return (Variable *) retV;
		};

		String VariableStackTrace::getType() {
			return strTypeStackTrace;
		};

		String VariableStackTrace::toString() {
			return toString(configPrintStackTraceLimit);
		};

		String VariableStackTrace::toString(int level_) {
			String out = "";
			char buffer[1024];
			if (stackTrace) {
				uint32_t level = 0;
				TDoubleEndedQueue<InstructionTrace>::Node *scan;
				for (scan = stackTrace->tail; scan && (level < level_); scan = scan->back, ++level) {
					if(scan->value.sourceSymbol) {
						String symbol = Context::getSymbolMirror(scan->value.sourceSymbol);
						if (symbol[0] == '#') {
							out << "- file " << symbol.index(1) << " ";
							sprintf(buffer, "line %d\n", scan->value.sourceLineNumber);
							out << buffer;
						};
						if (symbol[0] == '@') {
#ifdef QUANTUM_SCRIPT_STACK_TRACE_SHOW_MEMORY_SOURCE
							out << "- memory " << symbol.index(1) << " ";
#else
							out << "- memory ";
#endif
							sprintf(buffer, "line %d\n", scan->value.sourceLineNumber);
							out << buffer;
						};
					};
				};
			};
			return out;
		};


		bool VariableStackTrace::toBoolean() {
			return true;
		};


	};
};


