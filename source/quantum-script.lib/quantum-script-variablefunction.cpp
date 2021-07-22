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

#include "quantum-script-variablefunction.hpp"
#include "quantum-script-variablearray.hpp"
#include "quantum-script-variableobject.hpp"
#include "quantum-script-variablesymbol.hpp"

namespace Quantum {
	namespace Script {

		using namespace XYO;

		XYO_DYNAMIC_TYPE_IMPLEMENT(VariableFunction, "{84EAB78C-BA85-4E5E-AC2F-06E7DF2E9E8B}");
		const char *VariableFunction::strTypeFunction = "Function";

		VariableFunction::VariableFunction() {
			XYO_DYNAMIC_TYPE_PUSH(VariableFunction);

			super.pointerLink(this);
			prototype.pointerLink(this);
			functionParent.pointerLink(this);

			prototype.newMemory();
			prototype->prototype=VariableObject::newVariable();

			functionProcedure = nullptr;
			valueSuper = nullptr;
		};

		Variable *VariableFunction::newVariable(FunctionParent *functionParent, VariableArray *parentVariables, VariableArray *parentArguments, FunctionProcedure functionProcedure, Object *super, void *valueSuper) {
			VariableFunction *retV;
			retV = TMemory<VariableFunction>::newMemory();
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

		String VariableFunction::getVariableType() {
			return strTypeFunction;
		};

		TPointer<Variable> VariableFunction::getPropertyBySymbol(Symbol symbolId) {
			if(symbolId == Context::getSymbolPrototype()) {
				return prototype->prototype;
			};
			return Variable::getPropertyBySymbol(symbolId);
		};

		void VariableFunction::setPropertyBySymbol(Symbol symbolId, Variable *valueToSet) {
			if(symbolId == Context::getSymbolPrototype()) {
				prototype->prototype = valueToSet;
			};
		};

		TPointer<Variable> VariableFunction::functionApply(Variable *this_, VariableArray *arguments) {
			return (*functionProcedure)(this, this_, arguments);
		};

		Variable *VariableFunction::instancePrototype() {
			return (Context::getPrototypeFunction())->prototype;
		};

		bool VariableFunction::instanceOfPrototype(Prototype *&out) {
			out = prototype;
			return true;
		};

		void VariableFunction::initMemory() {
			Variable::initMemory();
			TMemory<Prototype>::initMemory();
			TMemory<FunctionParent>::initMemory();
		};

		bool VariableFunction::toBoolean() {
			return true;
		};

		String VariableFunction::toString() {
			return strTypeFunction;
		};

	};
};


