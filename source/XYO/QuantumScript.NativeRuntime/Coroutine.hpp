// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_COROUTINE_HPP
#define XYO_QUANTUMSCRIPT_NATIVERUNTIME_COROUTINE_HPP

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_INSTRUCTION_HPP
#	include <XYO/QuantumScript.NativeRuntime/Instruction.hpp>
#endif

namespace XYO::QuantumScript::NativeRuntime {

	// The state of a function with yield between calls: the C++ locals of the
	// function are saved on yield and restored on the next call.
	// VariableFunctionWithYield::yieldStep is the point to resume, 0 to start.
	class CoroutineFrame : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(CoroutineFrame);

		public:
			TPointer<Module> module;
			TPointerX<VariableArray> variable;
			TPointer<Iterator> *iterator;
			int *value;

			XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT CoroutineFrame();
			XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT ~CoroutineFrame();

			XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT void setSize(size_t iteratorCount, size_t valueCount);
			// the function starts again
			XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT void reset();

		protected:
			size_t iteratorCount;
			size_t valueCount;
	};

	// a function with yield of the module, the state is in the frame of the function
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> newFunctionWithYield(Module *module, FunctionParent *parent, VariableArray *localVariables, VariableArray *arguments, FunctionProcedureWithYield procedure, size_t iteratorCount, size_t valueCount);

	inline CoroutineFrame *getCoroutineFrame(VariableFunctionWithYield *function) {
		return (CoroutineFrame *)function->super.value();
	};

	// the function returned, the next call starts it again
	inline void coroutineReset(VariableFunctionWithYield *function) {
		function->yieldStep = 0;
		getCoroutineFrame(function)->reset();
	};

};

#endif
