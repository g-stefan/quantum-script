//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_INSTRUCTIONCONTEXT_HPP
#define QUANTUM_SCRIPT_INSTRUCTIONCONTEXT_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_EXECUTIVECONTEXT_HPP
#include "quantum-script-executivecontext.hpp"
#endif

#ifndef QUANTUM_SCRIPT_INSTRUCTIONX_HPP
#include "quantum-script-instructionx.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEVMFUNCTION_HPP
#include "quantum-script-variablevmfunction.hpp"
#endif

#ifndef QUANTUM_SCRIPT_VARIABLEARRAY_HPP
#include "quantum-script-variablearray.hpp"
#endif

namespace Quantum {
	namespace Script {

		using namespace XYO;

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

		class InstructionContext :
			public Stack {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(InstructionContext);
			public:

				TStack<String> includedFile;

				TPointer<ExecutiveContext> contextStack;

				TPointer<TDoubleEndedQueue<InstructionTrace> > stackTrace;
				TRedBlackTree<String, bool> listIncludeOnce;

				InstructionList *instructionListExecutive;
				ProgramCounter *currentProgramCounter;
				ProgramCounter *nextProgramCounter;

				ExecutiveContextPc    *pcContext;
				ExecutiveContextFunction  *functionContext;

				TPointer<Variable> returnValue;

				int error;
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

				QUANTUM_SCRIPT_EXPORT InstructionContext();
				QUANTUM_SCRIPT_EXPORT bool init();

				QUANTUM_SCRIPT_EXPORT TPointer<Variable> newError(String str);

				inline bool getFunctionX(Symbol &name, TPointer<Variable> &out) {
					TPointer<Variable> out_ = (Context::getGlobalObject())->getPropertyBySymbol(name);
					if (TIsType<VariableVmFunction>(out_)) {
						out = out_;
						return true;
					};
					return false;
				};

#ifndef QUANTUM_SCRIPT_DISABLE_CLOSURE
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
					if (index < ((int) ((VariableArray *) (functionContext->functionLocalVariables.value()))->value->length())) {
						return (*(((VariableArray *) (functionContext->functionLocalVariables.value()))->value.value()))[index];
					};
					return VariableUndefined::newVariable();
				};

				inline TPointer<Variable> getArgument(int index) {
					if (index < ((int) ((VariableArray *) (functionContext->functionArguments.value()))->value->length())) {
						return (*(((VariableArray *) (functionContext->functionArguments.value()))->value.value()))[index];
					};
					return TPointer<Variable>(VariableUndefined::newVariable());
				};

#ifndef QUANTUM_SCRIPT_DISABLE_CLOSURE
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

				QUANTUM_SCRIPT_EXPORT static void initMemory();

		};


	};
};



#endif
