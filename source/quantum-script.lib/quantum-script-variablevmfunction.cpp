//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-objectiteratorkey.hpp"
#include "quantum-script-objectiteratorvalue.hpp"

#include "quantum-script-instructionx.hpp"
#include "quantum-script-instructioncontext.hpp"
#include "quantum-script-variablevmfunction.hpp"
#include "quantum-script-executivex.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablesymbol.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableVmFunction, "{341DBCC2-9D58-4E15-B9CA-9F215B8375AD}");
		const char *VariableVmFunction::strTypeVmFunction = "Function";

		VariableVmFunction::VariableVmFunction() {
			XYO_DYNAMIC_TYPE_PUSH(VariableVmFunction);
			prototype.pointerLink(this);
			coroutineContext.pointerLink(this);
			functionParent.pointerLink(this);
			functionHint = ParserFunctionHint::All;
			coroutineContext.newMemory();
			fnSource = 0;
		};


		Variable *VariableVmFunction::newVariable(ProgramCounter *value) {
			VariableVmFunction *retV;
			retV = TMemory<VariableVmFunction>::newMemory();
			retV->value = value;
			retV->valueEnd = nullptr;
			retV->originalValue = value;
			retV->functionHint = ParserFunctionHint::All;
			retV->fnSource = 0;
			return (Variable *) retV;
		};

		String VariableVmFunction::getVariableType() {
			return strTypeVmFunction;
		};

		TPointer<Variable> VariableVmFunction::getPropertyBySymbol(Symbol symbolId) {
			if(symbolId == Context::getSymbolPrototype()) {
				return prototype->prototype;
			};
			return Variable::getPropertyBySymbol(symbolId);
		};

		void VariableVmFunction::setPropertyBySymbol(Symbol symbolId, Variable *valueToSet) {
			if(symbolId == Context::getSymbolPrototype()) {
				prototype->prototype = valueToSet;
			};
		};

		TPointer<Variable> VariableVmFunction::functionApply(Variable *this_, VariableArray *arguments) {
			return  (ExecutiveX::getExecutive()).callVmFunction(this, this_, arguments);
		};

		Variable *VariableVmFunction::instancePrototype() {
			return (Context::getPrototypeFunction())->prototype;
		};

		bool VariableVmFunction::instanceOfPrototype(Prototype *&out) {
			out = prototype;
			return true;
		};

		String VariableVmFunction::toString() {
			if(fnSource) {
				return Context::getSymbolMirror(fnSource);
			};
			throw Error("toString");
		};

		bool VariableVmFunction::toBoolean() {
			return true;
		};


	};
};


