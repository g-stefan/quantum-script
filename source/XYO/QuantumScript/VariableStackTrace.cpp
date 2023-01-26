// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/InstructionContext.hpp>
#include <XYO/QuantumScript/VariableStackTrace.hpp>

namespace XYO::QuantumScript {

	XYO_DYNAMIC_TYPE_IMPLEMENT(VariableStackTrace, "{D2C5840D-48B9-4424-8BCC-39147444BC1A}");
	const char *VariableStackTrace::strTypeStackTrace = "StackTrace";

	VariableStackTrace::VariableStackTrace() {
		XYO_DYNAMIC_TYPE_PUSH(VariableStackTrace);
		configPrintStackTraceLimit = XYO_QUANTUMSCRIPT_DEFAULT_STACK_TRACE_LEVEL;
	};

	Variable *VariableStackTrace::newVariable(TDoubleEndedQueue<InstructionTrace> *stackTrace, InstructionContext *context) {
		VariableStackTrace *retV;
		retV = TMemory<VariableStackTrace>::newMemory();
		retV->stackTrace = stackTrace;
		retV->context = context;
		return (Variable *)retV;
	};

	String VariableStackTrace::getVariableType() {
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
				if (scan->value.sourceSymbol) {					
					String symbol = Context::getSymbolMirror(scan->value.sourceSymbol);
					if (symbol[0] == '#') {
						out << "- file " << String::replace(symbol.index(1), Shell::getCwd() << Shell::pathSeparator, "") << " ";
						sprintf(buffer, "line %u\n", scan->value.sourceLineNumber);
						out << buffer;
					};
					if (symbol[0] == '@') {
#ifdef XYO_QUANTUMSCRIPT_STACK_TRACE_SHOW_MEMORY_SOURCE
						out << "- memory " << symbol.index(1) << " ";
#else
						out << "- memory ";
#endif
						sprintf(buffer, "line %u\n", scan->value.sourceLineNumber);
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
