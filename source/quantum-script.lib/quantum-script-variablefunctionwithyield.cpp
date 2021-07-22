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

#include "quantum-script-variablefunctionwithyield.hpp"
#include "quantum-script-variablearray.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablesymbol.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableFunctionWithYield, "{483FB52F-EEAB-46C5-89B3-850A8F65304F}");
		const char *VariableFunctionWithYield::strTypeFunction = "Function";

		VariableFunctionWithYield::VariableFunctionWithYield() {
			XYO_DYNAMIC_TYPE_PUSH(VariableFunctionWithYield);

			super.pointerLink(this);
			prototype.pointerLink(this);

			prototype.newMemory();
			prototype->prototype=VariableObject::newVariable();

			functionParent.pointerLink(this);
			yieldVariables.pointerLink(this);
			functionProcedure = nullptr;
			yieldStep = 0;
			yieldVariables=VariableArray::newVariable();
			valueSuper = nullptr;

		};

		Variable *VariableFunctionWithYield::newVariable(FunctionParent *functionParent, VariableArray *parentVariables, VariableArray *parentArguments, FunctionProcedureWithYield functionProcedure, Object *super, void *valueSuper) {
			VariableFunctionWithYield *retV;
			retV = TMemory<VariableFunctionWithYield>::newMemory();
			if(functionParent || parentVariables || parentArguments) {
				retV->functionParent.newMemory();
				retV->functionParent->functionParent = functionParent;
				retV->functionParent->variables = parentVariables;
				retV->functionParent->arguments = parentArguments;
			};
			retV->functionProcedure = functionProcedure;
			retV->super = super;
			retV->valueSuper = valueSuper;
			return (Variable *) retV;
		};

		String VariableFunctionWithYield::getVariableType() {
			return strTypeFunction;
		};

		TPointer<Variable> VariableFunctionWithYield::getPropertyBySymbol(Symbol symbolId) {
			if(symbolId == Context::getSymbolPrototype()) {
				return prototype->prototype;
			};
			return Variable::getPropertyBySymbol(symbolId);
		};

		void VariableFunctionWithYield::setPropertyBySymbol(Symbol symbolId, Variable *valueToSet) {
			if(symbolId == Context::getSymbolPrototype()) {
				prototype->prototype = valueToSet;
			};
		};

		TPointer<Variable> VariableFunctionWithYield::functionApply(Variable *this_, VariableArray *arguments) {
			if(yieldStep) {
				return (*functionProcedure)(this, this_, arguments);
			};
			yieldVariables=VariableArray::newVariable();
			TPointer<Variable> retV = (*functionProcedure)(this, this_, arguments);
			if(yieldStep) {
				return retV;
			};
			yieldVariables=VariableArray::newVariable();
			return retV;
		};

		Variable *VariableFunctionWithYield::instancePrototype() {
			return (Context::getPrototypeFunction())->prototype;
		};

		bool VariableFunctionWithYield::instanceOfPrototype(Prototype *&out) {
			out = prototype;
			return true;
		};

		void VariableFunctionWithYield::initMemory() {
			Variable::initMemory();
			TMemory<Variable>::initMemory();
			TMemory<Prototype>::initMemory();
			TMemory<FunctionParent>::initMemory();
		};

		bool VariableFunctionWithYield::toBoolean() {
			return true;
		};

		String VariableFunctionWithYield::toString() {
			return strTypeFunction;
		};


	};
};


