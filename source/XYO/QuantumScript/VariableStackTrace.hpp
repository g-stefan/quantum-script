// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLESTACKTRACE_HPP
#define XYO_QUANTUMSCRIPT_VARIABLESTACKTRACE_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_INSTRUCTIONCONTEXT_HPP
#	include <XYO/QuantumScript/InstructionContext.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableStackTrace;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableStackTrace> : public TMemoryPoolActive<XYO::QuantumScript::VariableStackTrace> {};
};

namespace XYO::QuantumScript {

	class InstructionContext;

	class VariableStackTrace : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableStackTrace);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableStackTrace);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeStackTrace;

		public:
			TPointer<TDoubleEndedQueue<InstructionTrace>> stackTrace;
			InstructionContext *context;
			int configPrintStackTraceLimit;

			XYO_QUANTUMSCRIPT_EXPORT VariableStackTrace();

			inline void activeDestructor() {
				stackTrace.deleteMemory();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(TDoubleEndedQueue<InstructionTrace> *stackTrace, InstructionContext *context);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT String toString();
			XYO_QUANTUMSCRIPT_EXPORT String toString(int level_);

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
	};

};

#endif
