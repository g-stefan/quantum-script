// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_INSTRUCTION_HPP
#define XYO_QUANTUMSCRIPT_NATIVERUNTIME_INSTRUCTION_HPP

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXCEPTION_HPP
#	include <XYO/QuantumScript.NativeRuntime/Exception.hpp>
#endif

// The instructions of the engine (Instruction.cpp) for compiled code:
// the operand stack is C++ locals, values are given and returned.
// Behavior changes of an instruction go in both.

namespace XYO::QuantumScript::NativeRuntime {

	//
	// Values
	//

	inline Variable *boolean(Module *module, bool value) {
		return value ? module->valueTrue : module->valueFalse;
	};

	inline bool isNumber(Variable *value) {
		return TIsTypeExact<VariableNumber>(value);
	};

	inline Number numberValue(Variable *value) {
		return ((VariableNumber *)value)->value;
	};

	//
	// Bitwise - 64 bit integer semantics, no undefined behavior
	//

	inline UInteger bitwiseToUInteger(Number value) {
		// modulo 2^64, as two's complement
		bool isNegative = false;
		value = trunc(value);
		if (value < 0) {
			isNegative = true;
			value = -value;
		};
		UInteger retV = (UInteger)fmod(value, 18446744073709551616.0);
		if (isNegative) {
			return ((UInteger)0) - retV;
		};
		return retV;
	};

	inline Integer bitwiseToInteger(Number value) {
		return (Integer)bitwiseToUInteger(value);
	};

	inline bool bitwiseIsValid(Number value) {
		return !(isnan(value) || isinf(value));
	};

	inline Number bitwiseOr(Number value, Number x) {
		if (!(bitwiseIsValid(value) && bitwiseIsValid(x))) {
			return NAN;
		};
		return (Number)(bitwiseToInteger(value) | bitwiseToInteger(x));
	};

	inline Number bitwiseAnd(Number value, Number x) {
		if (!(bitwiseIsValid(value) && bitwiseIsValid(x))) {
			return NAN;
		};
		return (Number)(bitwiseToInteger(value) & bitwiseToInteger(x));
	};

	inline Number bitwiseXor(Number value, Number x) {
		if (!(bitwiseIsValid(value) && bitwiseIsValid(x))) {
			return NAN;
		};
		return (Number)(bitwiseToInteger(value) ^ bitwiseToInteger(x));
	};

	inline Number bitwiseLeftShift(Number value, Number x) {
		if (!(bitwiseIsValid(value) && bitwiseIsValid(x)) || (x < 0)) {
			return NAN;
		};
		UInteger shift = bitwiseToUInteger(x);
		if (shift >= 64) {
			return 0;
		};
		return (Number)((Integer)(bitwiseToUInteger(value) << shift));
	};

	inline Number bitwiseRightShift(Number value, Number x) {
		if (!(bitwiseIsValid(value) && bitwiseIsValid(x)) || (x < 0)) {
			return NAN;
		};
		Integer value_ = bitwiseToInteger(value);
		UInteger shift = bitwiseToUInteger(x);
		if (shift >= 64) {
			return (value_ < 0) ? -1 : 0;
		};
		return (Number)(value_ >> shift);
	};

	inline Number bitwiseRightShiftZero(Number value, Number x) {
		if (!(bitwiseIsValid(value) && bitwiseIsValid(x)) || (x < 0)) {
			return NAN;
		};
		UInteger shift = bitwiseToUInteger(x);
		if (shift >= 64) {
			return 0;
		};
		return (Number)(bitwiseToUInteger(value) >> shift);
	};

	inline Number bitwiseNot(Number value) {
		if (isnan(value) || isinf(value)) {
			return NAN;
		};
		return (Number)(~bitwiseToInteger(value));
	};

	//
	// Operators on values, every value is a Variable
	//

	inline TPointer<Variable> operatorPlus(Variable *a, Variable *b) {
		return a->operatorPlus(b);
	};

	inline TPointer<Variable> operatorMinus(Variable *a, Variable *b) {
		return VariableNumber::newVariable(a->toNumber() - b->toNumber());
	};

	inline TPointer<Variable> operatorMul(Variable *a, Variable *b) {
		return VariableNumber::newVariable(a->toNumber() * b->toNumber());
	};

	inline TPointer<Variable> operatorDiv(Variable *a, Variable *b) {
		return VariableNumber::newVariable(a->toNumber() / b->toNumber());
	};

	inline TPointer<Variable> operatorMod(Variable *a, Variable *b) {
		Number value = a->toNumber();
		Number x = b->toNumber();
		return VariableNumber::newVariable(fmod(value, x));
	};

	inline TPointer<Variable> operatorUnaryPlus(Variable *a) {
		return VariableNumber::newVariable(a->toNumber());
	};

	inline TPointer<Variable> operatorBitwiseOr(Variable *a, Variable *b) {
		Number value = a->toNumber();
		Number x = b->toNumber();
		return VariableNumber::newVariable(bitwiseOr(value, x));
	};

	inline TPointer<Variable> operatorBitwiseAnd(Variable *a, Variable *b) {
		Number value = a->toNumber();
		Number x = b->toNumber();
		return VariableNumber::newVariable(bitwiseAnd(value, x));
	};

	inline TPointer<Variable> operatorBitwiseXor(Variable *a, Variable *b) {
		Number value = a->toNumber();
		Number x = b->toNumber();
		return VariableNumber::newVariable(bitwiseXor(value, x));
	};

	inline TPointer<Variable> operatorBitwiseLeftShift(Variable *a, Variable *b) {
		Number value = a->toNumber();
		Number x = b->toNumber();
		return VariableNumber::newVariable(bitwiseLeftShift(value, x));
	};

	inline TPointer<Variable> operatorBitwiseRightShift(Variable *a, Variable *b) {
		Number value = a->toNumber();
		Number x = b->toNumber();
		return VariableNumber::newVariable(bitwiseRightShift(value, x));
	};

	inline TPointer<Variable> operatorBitwiseRightShiftZero(Variable *a, Variable *b) {
		Number value = a->toNumber();
		Number x = b->toNumber();
		return VariableNumber::newVariable(bitwiseRightShiftZero(value, x));
	};

	inline TPointer<Variable> operatorBitwiseNot(Variable *a) {
		return VariableNumber::newVariable(bitwiseNot(a->toNumber()));
	};

	inline Variable *operatorNot(Module *module, Variable *a) {
		return boolean(module, !a->toBoolean());
	};

	inline Variable *operatorLogicalOr(Module *module, Variable *a, Variable *b) {
		if (a->toBoolean()) {
			return module->valueTrue;
		};
		return boolean(module, b->toBoolean());
	};

	inline Variable *operatorLogicalAnd(Module *module, Variable *a, Variable *b) {
		if (a->toBoolean()) {
			return boolean(module, b->toBoolean());
		};
		return module->valueFalse;
	};

	inline Variable *operatorEqual(Module *module, Variable *a, Variable *b) {
		return boolean(module, a->isEqual(b));
	};

	inline Variable *operatorNotEqual(Module *module, Variable *a, Variable *b) {
		return boolean(module, !a->isEqual(b));
	};

	inline Variable *operatorEqualStrict(Module *module, Variable *a, Variable *b) {
		return boolean(module, a->isEqualStrict(b));
	};

	inline Variable *operatorNotEqualStrict(Module *module, Variable *a, Variable *b) {
		return boolean(module, !a->isEqualStrict(b));
	};

	inline Variable *operatorLess(Module *module, Variable *a, Variable *b) {
		return boolean(module, a->isLess(b));
	};

	inline Variable *operatorLessOrEqual(Module *module, Variable *a, Variable *b) {
		return boolean(module, a->isLessOrEqual(b));
	};

	inline Variable *operatorGreater(Module *module, Variable *a, Variable *b) {
		return boolean(module, a->isGreater(b));
	};

	inline Variable *operatorGreaterOrEqual(Module *module, Variable *a, Variable *b) {
		return boolean(module, a->isGreaterOrEqual(b));
	};

	// y in x
	inline Variable *operatorIn(Module *module, Variable *a, Variable *b) {
		return boolean(module, b->hasPropertyByVariable(a));
	};

	// x instanceof y
	inline Variable *operatorInstanceOf(Module *module, Variable *a, Variable *b) {
		return boolean(module, a->instanceOf(b));
	};

	inline TPointer<Variable> operatorTypeOf(Variable *a) {
		return VariableString::newVariable(a->getVariableType());
	};

	//
	// Assign operators, reference is a VariableReference*
	//

	inline TPointer<Variable> assignPlus(Variable *reference, Variable *b) {
		return reference->referenceOperatorPlus(b);
	};

	inline TPointer<Variable> assignMinus(Variable *reference, Variable *b) {
		return reference->referenceSet(VariableNumber::newVariable(reference->referenceToNumber() - b->toNumber()));
	};

	inline TPointer<Variable> assignMul(Variable *reference, Variable *b) {
		return reference->referenceSet(VariableNumber::newVariable(reference->referenceToNumber() * b->toNumber()));
	};

	inline TPointer<Variable> assignDiv(Variable *reference, Variable *b) {
		return reference->referenceSet(VariableNumber::newVariable(reference->referenceToNumber() / b->toNumber()));
	};

	inline TPointer<Variable> assignMod(Variable *reference, Variable *b) {
		Number value = reference->referenceToNumber();
		Number x = b->toNumber();
		return reference->referenceSet(VariableNumber::newVariable(fmod(value, x)));
	};

	inline TPointer<Variable> assignBitwiseOr(Variable *reference, Variable *b) {
		Number value = reference->referenceToNumber();
		Number x = b->toNumber();
		return reference->referenceSet(VariableNumber::newVariable(bitwiseOr(value, x)));
	};

	inline TPointer<Variable> assignBitwiseAnd(Variable *reference, Variable *b) {
		Number value = reference->referenceToNumber();
		Number x = b->toNumber();
		return reference->referenceSet(VariableNumber::newVariable(bitwiseAnd(value, x)));
	};

	inline TPointer<Variable> assignBitwiseXor(Variable *reference, Variable *b) {
		Number value = reference->referenceToNumber();
		Number x = b->toNumber();
		return reference->referenceSet(VariableNumber::newVariable(bitwiseXor(value, x)));
	};

	inline TPointer<Variable> assignBitwiseLeftShift(Variable *reference, Variable *b) {
		Number value = reference->referenceToNumber();
		Number x = b->toNumber();
		return reference->referenceSet(VariableNumber::newVariable(bitwiseLeftShift(value, x)));
	};

	inline TPointer<Variable> assignBitwiseRightShift(Variable *reference, Variable *b) {
		Number value = reference->referenceToNumber();
		Number x = b->toNumber();
		return reference->referenceSet(VariableNumber::newVariable(bitwiseRightShift(value, x)));
	};

	inline TPointer<Variable> assignBitwiseRightShiftZero(Variable *reference, Variable *b) {
		Number value = reference->referenceToNumber();
		Number x = b->toNumber();
		return reference->referenceSet(VariableNumber::newVariable(bitwiseRightShiftZero(value, x)));
	};

	// ++x
	inline TPointer<Variable> plusPlusLeft(Variable *reference) {
		return reference->referenceSet(VariableNumber::newVariable(reference->referenceToNumber() + 1));
	};

	// --x
	inline TPointer<Variable> minusMinusLeft(Variable *reference) {
		return reference->referenceSet(VariableNumber::newVariable(reference->referenceToNumber() - 1));
	};

	// x++
	inline TPointer<Variable> plusPlusRight(Variable *reference) {
		TPointer<Variable> retV(reference->referenceGet());
		reference->referenceSet(VariableNumber::newVariable(reference->referenceToNumber() + 1));
		return retV;
	};

	// x--
	inline TPointer<Variable> minusMinusRight(Variable *reference) {
		TPointer<Variable> retV(reference->referenceGet());
		reference->referenceSet(VariableNumber::newVariable(reference->referenceToNumber() - 1));
		return retV;
	};

	//
	// Errors and exceptions
	//

	// an Error object, as the Error constructor of the standard library
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> newError(Module *module, const String &message);
	// a thrown value, a value that is not an object becomes an Error object
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT Exception newException(Module *module, Variable *value);
	// an error of a native function as a thrown value
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT Exception newException(Module *module, Error &error);
	// catch: the stack trace of the exception is set on the thrown object
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT void setStackTrace(Module *module, Variable *value, StackTrace *stackTrace);
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> newStackTrace(Module *module, StackTrace *stackTrace);

	//
	// Calls
	//

	// after a call of code that is not compiled: Script.exit stops the compiled code
	inline void checkExit(Module *module) {
		if (module->executive->isExit) {
			throw Exit();
		};
	};

	// ([function])()
	inline TPointer<Variable> call(Module *module, Variable *function, Variable *this_, VariableArray *arguments) {
		TPointer<Variable> retV(function->functionApply(this_, arguments));
		checkExit(module);
		return retV;
	};

	// [function]()
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> callSymbol(Module *module, Symbol symbol, VariableArray *arguments);
	// [object].[function]()
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> callWithThisReference(Module *module, Variable *object, Symbol symbol, VariableArray *arguments);
	// [function].apply(this, arguments)
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> callThisModeApply(Module *module, Variable *function, Variable *this_, VariableArray *arguments);

	//
	// new [function]()
	//

	// the new object gets the prototype of the function
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT void assignXPrototype(Variable *object, Variable *function);

	// the result of new, the object or the value returned by the constructor
	inline Variable *assignNewObject(Variable *object, Variable *result) {
		if (TIsTypeExact<VariableUndefined>(result)) {
			return object;
		};
		return result;
	};

	//
	// Functions, closures
	//

	// a function of the module, the parent function variables and arguments are captured
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> newFunction(Module *module, FunctionParent *parent, VariableArray *localVariables, VariableArray *arguments, FunctionProcedure procedure);

	// variables and arguments of a parent function, level 1 is the parent
	inline VariableArray *localVariablesLevel(FunctionParent *parent, int level) {
		FunctionParent *scan;
		--level;
		for (scan = parent; scan; scan = scan->functionParent.value()) {
			if (level == 0) {
				return scan->variables.value();
			};
			--level;
		};
		return nullptr;
	};

	inline VariableArray *argumentsLevel(FunctionParent *parent, int level) {
		FunctionParent *scan;
		--level;
		for (scan = parent; scan; scan = scan->functionParent.value()) {
			if (level == 0) {
				return scan->arguments.value();
			};
			--level;
		};
		return nullptr;
	};

	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT VariableArray *localVariablesLevelReference(FunctionParent *parent, int level);
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> localVariablesLevelGet(FunctionParent *parent, int level, uint32_t index);
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT VariableArray *argumentsLevelReference(FunctionParent *parent, int level);
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> argumentsLevelGet(FunctionParent *parent, int level, uint32_t index);

};

#endif
