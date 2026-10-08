// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXCEPTION_HPP
#define XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXCEPTION_HPP

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_MODULE_HPP
#	include <XYO/QuantumScript.NativeRuntime/Module.hpp>
#endif

namespace XYO::QuantumScript::NativeRuntime {

	typedef TDoubleEndedQueue<InstructionTrace> StackTrace;

	// A value thrown by script code. It is an Error with the message of the
	// value, the engine sees it as any error of a native function.
	class Exception : public Error {
		public:
			TPointer<Variable> value;
			TPointer<StackTrace> stackTrace;

			inline Exception(){};

			inline Exception(Variable *value_, const String &message_) : Error(message_) {
				value = value_;
			};

			// the code at source / line was running, on the way to the caller
			inline void trace(Symbol source, uint32_t line) {
				if (!source) {
					return;
				};
				if (!stackTrace) {
					stackTrace.newMemory();
				};
				InstructionTrace tracePoint;
				tracePoint.sourceSymbol = source;
				tracePoint.sourceLineNumber = line;
				stackTrace->push(tracePoint);
			};
	};

	// Script.exit, unwinds the compiled code, not an error,
	// script code can not catch it
	class Exit {};

};

#endif
