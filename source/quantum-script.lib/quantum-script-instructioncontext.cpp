//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-instructioncontext.hpp"
#include "quantum-script-variablestring.hpp"

//#define QUANTUM_SCRIPT_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {

		using namespace XYO;

		InstructionContext::InstructionContext() {
			error = InstructionError::None;
		};

		bool InstructionContext::init() {

			configPrintStackTraceLimit = QUANTUM_SCRIPT_DEFAULT_STACK_TRACE_LEVEL;

			strLength__ = "length";
			strPrototype__ = "prototype";
			strArguments__ = "arguments";
			strString__ = "String";
			strArray__ = "Array";
			strMessage__ = "message";
			strError__ = "Error";
			strStackTrace__ = "stackTrace";
			strValue__ = "value";

			symbolLength__ = Context::getSymbol(strLength__);
			symbolPrototype__ = Context::getSymbol(strPrototype__);
			symbolArguments__ = Context::getSymbol(strArguments__);
			symbolString__ = Context::getSymbol(strString__);
			symbolArray__ = Context::getSymbol(strArray__);
			symbolMessage__ = Context::getSymbol(strMessage__);
			symbolError__ = Context::getSymbol(strError__);
			symbolStackTrace__ = Context::getSymbol(strStackTrace__);
			symbolValue__ = Context::getSymbol(strValue__);

			returnValue = VariableUndefined::newVariable();

			return true;
		};

		TPointer<Variable> InstructionContext::newError(String str) {
			TPointer<Variable> constructor;
			VariableObject *retV_;
			retV_ = (VariableObject *)VariableObject::newVariable();
			retV_->value->set(symbolMessage__, VariableString::newVariable(str));
			if (getFunctionX(symbolError__, constructor)) {
				retV_->prototype = ((VariableVmFunction *)constructor.value())->prototype;
			};
			return (Variable *)retV_;
		};

		void InstructionContext::initMemory() {
			TMemory<TStack<String>>::initMemory();
			TMemory<TRedBlackTree<String, bool>>::initMemory();
			TMemory<ExecutiveContext>::initMemory();
			TMemory<ExecutiveContextPc>::initMemory();
			TMemory<ExecutiveContextFunction>::initMemory();
			TMemory<TDoubleEndedQueue<InstructionTrace>>::initMemory();
		};

	};
};
