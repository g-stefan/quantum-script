// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.NativeRuntime/Instruction.hpp>
#include <XYO/QuantumScript/VariableVmFunction.hpp>

namespace XYO::QuantumScript::NativeRuntime {

	//
	// Errors and exceptions
	//

	TPointer<Variable> newError(Module *module, const String &message) {
		VariableObject *retV = (VariableObject *)VariableObject::newVariable();
		TPointer<Variable> retV_(retV);
		retV->value->set(module->symbolMessage, VariableString::newVariable(message));
		TPointer<Variable> constructor(module->global->getPropertyBySymbol(module->symbolError));
		if (TIsType<VariableVmFunction>(constructor)) {
			retV->prototype = ((VariableVmFunction *)constructor.value())->prototype;
		};
		return retV_;
	};

	Exception newException(Module *module, Variable *value) {
		if (!TIsType<VariableObject>(value)) {
			TPointer<Variable> error(newError(module, value->toString()));
			return Exception(error, error->getPropertyBySymbol(module->symbolMessage)->toString());
		};
		return Exception(value, value->getPropertyBySymbol(module->symbolMessage)->toString());
	};

	Exception newException(Module *module, Error &error) {
		return newException(module, newError(module, error.getMessage()));
	};

	TPointer<Variable> newStackTrace(Module *module, StackTrace *stackTrace) {
		TPointer<Variable> retV(VariableStackTrace::newVariable(stackTrace, nullptr));
		((VariableStackTrace *)retV.value())->configPrintStackTraceLimit = module->executive->configPrintStackTraceLimit;
		return retV;
	};

	void setStackTrace(Module *module, Variable *value, StackTrace *stackTrace) {
		if (TIsType<VariableObject>(value)) {
			((VariableObject *)value)->value->set(module->symbolStackTrace, newStackTrace(module, stackTrace));
		};
	};

	//
	// Calls
	//

	TPointer<Variable> callSymbol(Module *module, Symbol symbol, VariableArray *arguments) {
		TPointer<Variable> function(module->global->getPropertyBySymbol(symbol));
		if (!function->isFunction()) {
			String error;
			error << "\"" << Context::getSymbolMirror(symbol) << "\" is not a function";
			throw Error(error);
		};
		return call(module, function, module->undefined, arguments);
	};

	TPointer<Variable> callWithThisReference(Module *module, Variable *object, Symbol symbol, VariableArray *arguments) {
		TPointer<Variable> function(object->getPropertyBySymbol(symbol));
		if (!function) {
			throw Error("call to a member function on a non-object");
		};
		if (!function->isFunction()) {
			String error;
			error << "\"" << Context::getSymbolMirror(symbol) << "\" is not a function";
			throw Error(error);
		};
		return call(module, function, object, arguments);
	};

	TPointer<Variable> callThisModeApply(Module *module, Variable *function, Variable *this_, VariableArray *arguments) {
		Variable *functionArguments = arguments->index(0).value();
		if (!TIsType<VariableArray>(functionArguments)) {
			throw Error("parameter not an Array");
		};
		// keep the arguments of apply while the function runs
		TPointer<Variable> functionArguments_(functionArguments);
		return call(module, function, this_, (VariableArray *)functionArguments);
	};

	//
	// new
	//

	void assignXPrototype(Variable *object, Variable *function) {
		Prototype *prototype = nullptr;
		if (function->instanceOfPrototype(prototype)) {
			if (TIsType<VariableObject>(object)) {
				((VariableObject *)object)->prototype = prototype;
				return;
			};
		};
		throw Error("assign prototype failed");
	};

	//
	// Functions
	//

	TPointer<Variable> newFunction(Module *module, FunctionParent *parent, VariableArray *localVariables, VariableArray *arguments, FunctionProcedure procedure) {
		return VariableFunction::newVariable(parent, localVariables, arguments, procedure, module, module);
	};

	VariableArray *localVariablesLevelReference(FunctionParent *parent, int level) {
		VariableArray *retV = localVariablesLevel(parent, level);
		if (!retV) {
			throw Error("variables reference");
		};
		return retV;
	};

	TPointer<Variable> localVariablesLevelGet(FunctionParent *parent, int level, uint32_t index) {
		VariableArray *array = localVariablesLevel(parent, level);
		if (!array) {
			throw Error("variables symbol");
		};
		return array->value->getValue(index, Context::getValueUndefined());
	};

	VariableArray *argumentsLevelReference(FunctionParent *parent, int level) {
		VariableArray *retV = argumentsLevel(parent, level);
		if (!retV) {
			throw Error("arguments reference");
		};
		return retV;
	};

	TPointer<Variable> argumentsLevelGet(FunctionParent *parent, int level, uint32_t index) {
		VariableArray *array = argumentsLevel(parent, level);
		if (!array) {
			throw Error("arguments symbol");
		};
		return array->value->getValue(index, Context::getValueUndefined());
	};

};
