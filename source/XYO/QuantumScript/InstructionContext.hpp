// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_INSTRUCTIONCONTEXT_HPP
#define XYO_QUANTUMSCRIPT_INSTRUCTIONCONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVECONTEXT_HPP
#	include <XYO/QuantumScript/ExecutiveContext.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_INSTRUCTIONX_HPP
#	include <XYO/QuantumScript/InstructionX.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLEVMFUNCTION_HPP
#	include <XYO/QuantumScript/VariableVmFunction.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLEARRAY_HPP
#	include <XYO/QuantumScript/VariableArray.hpp>
#endif

namespace XYO::QuantumScript {

	class InstructionError {
		public:
			enum {
				None,
				Error,
				Throw
			};
	};

	class InstructionTrace {
		public:
			uint32_t sourceSymbol;
			uint32_t sourceLineNumber;
	};

	class InstructionContext : public Stack {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(InstructionContext);

		public:
			TStack<String> includedFile;

			TPointer<ExecutiveContext> contextStack;

			TPointer<TDoubleEndedQueue<InstructionTrace>> stackTrace;
			TRedBlackTree<String, bool> listIncludeOnce;

			InstructionList *instructionListExecutive;
			ProgramCounter *currentProgramCounter;
			ProgramCounter *nextProgramCounter;

			ExecutiveContextPc *pcContext;
			ExecutiveContextFunction *functionContext;

			TPointer<Variable> returnValue;

			int error;
			String errorInfo;
			int configPrintStackTraceLimit;

			void *executiveSuper;

			String strLength__;
			String strPrototype__;
			String strArguments__;
			String strString__;
			String strArray__;
			String strMessage__;
			String strError__;
			String strStackTrace__;
			String strValue__;

			Symbol symbolLength__;
			Symbol symbolPrototype__;
			Symbol symbolArguments__;
			Symbol symbolString__;
			Symbol symbolArray__;
			Symbol symbolMessage__;
			Symbol symbolError__;
			Symbol symbolStackTrace__;
			Symbol symbolValue__;

			XYO_QUANTUMSCRIPT_EXPORT InstructionContext();
			XYO_QUANTUMSCRIPT_EXPORT bool init();

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> newError(String str);

			inline bool getFunctionX(Symbol &name, TPointer<Variable> &out) {
				TPointer<Variable> out_ = (Context::getGlobalObject())->getPropertyBySymbol(name);
				if (TIsType<VariableVmFunction>(out_)) {
					out = out_;
					return true;
				};
				return false;
			};

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
			inline TPointer<Variable> getFunctionLocalVariablesLevel(int level) {
				FunctionParent *scan;
				--level;
				for (scan = functionContext->functionParent.value(); scan; scan = scan->functionParent.value()) {
					if (level == 0) {
						return scan->variables.value();
					};
					--level;
				};
				return nullptr;
			};
#endif

			inline TPointer<Variable> getLocalVariable(int index) {
				if (index < ((int)((VariableArray *)(functionContext->functionLocalVariables.value()))->value->length())) {
					return (*(((VariableArray *)(functionContext->functionLocalVariables.value()))->value.value()))[index];
				};
				return VariableUndefined::newVariable();
			};

			inline TPointer<Variable> getArgument(int index) {
				if (index < ((int)((VariableArray *)(functionContext->functionArguments.value()))->value->length())) {
					return (*(((VariableArray *)(functionContext->functionArguments.value()))->value.value()))[index];
				};
				return TPointer<Variable>(VariableUndefined::newVariable());
			};

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
			inline TPointer<Variable> getFunctionArgumentsLevel(int level) {
				FunctionParent *scan;
				--level;
				for (scan = functionContext->functionParent.value(); scan; scan = scan->functionParent.value()) {
					if (level == 0) {
						return scan->arguments.value();
					};
					--level;
				};
				return nullptr;
			};
#endif

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();
	};

};

#endif
