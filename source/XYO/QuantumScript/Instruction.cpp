// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript/Instruction.hpp>

#include <XYO/QuantumScript/ProgramCounter.hpp>
#include <XYO/QuantumScript/InstructionX.hpp>

#include <XYO/QuantumScript/VariableSymbol.hpp>
#include <XYO/QuantumScript/VariableBoolean.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableString.hpp>
#include <XYO/QuantumScript/VariableNull.hpp>

#include <XYO/QuantumScript/VariableArgumentLevel.hpp>
#include <XYO/QuantumScript/VariableVmProgramCounter.hpp>
#include <XYO/QuantumScript/VariableVmFunction.hpp>
#include <XYO/QuantumScript/VariableOperator21.hpp>
#include <XYO/QuantumScript/VariableOperator22.hpp>
#include <XYO/QuantumScript/VariableOperator23.hpp>
#include <XYO/QuantumScript/VariableOperator31.hpp>
#include <XYO/QuantumScript/VariableReferenceSymbol.hpp>
#include <XYO/QuantumScript/VariableReferenceVariable.hpp>
#include <XYO/QuantumScript/VariableReferenceIndex.hpp>
#include <XYO/QuantumScript/VariableStackTrace.hpp>
#include <XYO/QuantumScript/VariableNativeVmFunction.hpp>

// #define XYO_QUANTUMSCRIPT_DEBUG_RUNTIME

namespace XYO::QuantumScript {

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushUndefined) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    push-undefined\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->push(Context::getValueUndefined());
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushSymbol) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
#	ifdef XYO_OBJECT_COUNTING_DEBUG
		printf(">%p    push-symbol %d\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#	else
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);
		printf(">%p    push-symbol %u : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#	endif
#endif

		context->push((Context::getGlobalObject())->getPropertyBySymbol(((VariableSymbol *)operand)->value));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushBoolean) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    push-boolean %u\n", context->currentProgramCounter, (uint16_t)((VariableBoolean *)operand)->value);
		fflush(stdout);
#endif
		context->push(operand);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushNumber) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    push-number %g\n", context->currentProgramCounter, ((VariableNumber *)operand)->value);
		fflush(stdout);
#endif

		context->push(operand);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushString) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    push-string \"%s\"\n", context->currentProgramCounter, ((VariableString *)operand)->value.value());
		fflush(stdout);
#endif

		context->push(operand);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssign) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		Variable *operand2;
		context->swap();
		TPointer<Variable> operand1(context->popTransfer());

		context->peek(operand2);

		operand1->referenceSetA1(operand2);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignPlus) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-plus\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->referenceOperatorPlus(operand2));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignMinus) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-minus\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((operand1)->referenceToNumber()) - ((operand2)->toNumber()))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignMul) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-mul\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((operand1)->referenceToNumber()) * ((operand2)->toNumber()))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignDiv) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-div\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((operand1)->referenceToNumber()) / ((operand2)->toNumber()))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignMod) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-mod\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->referenceToNumber();
		Number x = operand2->toNumber();

		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {
			context->push(operand1->referenceSet(VariableNumber::newVariable(
			    NAN)));
			return;
		};

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((Integer)value) % ((Integer)x))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignBitwiseOr) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-bitwise-or\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->referenceToNumber();
		Number x = operand2->toNumber();

		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {

			context->push(operand1->referenceSet(VariableNumber::newVariable(
			    NAN)));
			return;
		};

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((Integer)value) | ((Integer)x))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignBitwiseAnd) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-bitwise-and\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->referenceToNumber();
		Number x = operand2->toNumber();

		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {

			context->push(operand1->referenceSet(VariableNumber::newVariable(
			    NAN)));
			return;
		};

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((Integer)value) & ((Integer)x))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignBitwiseLeftShift) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-bitwise-left-shift\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->referenceToNumber();
		Number x = operand2->toNumber();

		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {

			context->push(operand1->referenceSet(VariableNumber::newVariable(
			    NAN)));
			return;
		};

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((Integer)value) << ((Integer)x))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignBitwiseRightShift) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-bitwise-right-shift\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->referenceToNumber();
		Number x = operand2->toNumber();

		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {

			context->push(operand1->referenceSet(VariableNumber::newVariable(
			    NAN)));
			return;
		};

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((Integer)value) >> ((Integer)x))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignBitwiseRightShiftZero) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-bitwise-right-shift-zero\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->referenceToNumber();
		Number x = operand2->toNumber();

		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {

			context->push(operand1->referenceSet(VariableNumber::newVariable(
			    NAN)));
			return;
		};

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((UInteger)value) >> ((Integer)x))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignBitwiseXor) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-bitwise-xor\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->referenceToNumber();
		Number x = operand2->toNumber();

		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {

			context->push(operand1->referenceSet(VariableNumber::newVariable(
			    NAN)));
			return;
		};

		context->push(operand1->referenceSet(VariableNumber::newVariable(
		    ((Integer)value) ^ ((Integer)x))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorEqual) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-equal\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(operand1->isEqual(operand2)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorEqualStrict) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-equal-strict\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(operand1->isEqualStrict(operand2)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorPlus) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-plus\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->operatorPlus(operand2));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorUnaryPlus) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-unary-plus\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableNumber::newVariable(operand1->toNumber()));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorMinus) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-minus\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableNumber::newVariable((operand1->toNumber()) - (operand2->toNumber())));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorMul) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-mul\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableNumber::newVariable((operand1->toNumber()) * (operand2->toNumber())));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorDiv) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-div\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableNumber::newVariable((operand1->toNumber()) / (operand2->toNumber())));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorMod) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-mod\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->toNumber();
		Number x = operand2->toNumber();
		context->push(VariableNumber::newVariable(fmod(value, x)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorNotEqual) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-not-equal\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(!(operand1->isEqual(operand2))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorNotEqualStrict) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-not-equal-strict\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(!(operand1->isEqualStrict(operand2))));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorNot) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-not\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(!operand1->toBoolean()));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorLessOrEqual) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-less-or-equal\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(operand1->isLessOrEqual(operand2)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorLess) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-less\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(operand1->isLess(operand2)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorGreaterOrEqual) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-greater-or-equal\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(operand1->isGreaterOrEqual(operand2)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorGreater) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-greater\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(operand1->isGreater(operand2)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorLogicalOr) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-logical-or\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		if (operand1->toBoolean()) {
			context->push(VariableBoolean::newVariable(true));
			return;
		};
		if (operand2->toBoolean()) {
			context->push(VariableBoolean::newVariable(true));
			return;
		};
		context->push(VariableBoolean::newVariable(false));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorBitwiseOr) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-bitwise-or\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->toNumber();
		Number x = operand2->toNumber();
		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {
			context->push(VariableNumber::newVariable(NAN));
			return;
		};
		context->push(VariableNumber::newVariable(((Integer)value) | ((Integer)x)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorLogicalAnd) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-logical-and\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		if (operand1->toBoolean()) {
			if (operand2->toBoolean()) {
				context->push(VariableBoolean::newVariable(true));
				return;
			};
		};
		context->push(VariableBoolean::newVariable(false));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorBitwiseAnd) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-bitwise-and\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->toNumber();
		Number x = operand2->toNumber();
		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {
			context->push(VariableNumber::newVariable(NAN));
			return;
		};
		context->push(VariableNumber::newVariable(((Integer)value) & ((Integer)x)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorBitwiseNot) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-bitwise-not\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->toNumber();
		if (isnan(value) || isinf(value)) {
			context->push(VariableNumber::newVariable(NAN));
			return;
		};
		context->push(VariableNumber::newVariable(~((Integer)value)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorBitwiseLeftShift) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-bitwise-left-shift\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->toNumber();
		Number x = operand2->toNumber();
		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {
			context->push(VariableNumber::newVariable(NAN));
			return;
		};
		context->push(VariableNumber::newVariable(((Integer)value) << ((Integer)x)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorBitwiseRightShift) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-bitwise-right-shift\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->toNumber();
		Number x = operand2->toNumber();
		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {
			context->push(VariableNumber::newVariable(NAN));
			return;
		};
		context->push(VariableNumber::newVariable(((Integer)value) >> ((Integer)x)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorBitwiseRightShiftZero) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-bitwise-right-shift-zero\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->toNumber();
		Number x = operand2->toNumber();
		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {
			context->push(VariableNumber::newVariable(NAN));
			return;
		};
		context->push(VariableNumber::newVariable(((UInteger)value) >> ((Integer)x)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorBitwiseXor) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-bitwise-xor\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		Number value = operand1->toNumber();
		Number x = operand2->toNumber();
		if (isnan(value) || isinf(value) || isnan(x) || isinf(x) || signbit(x)) {
			context->push(VariableNumber::newVariable(NAN));
			return;
		};
		context->push(VariableNumber::newVariable(((Integer)value) ^ ((Integer)x)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorIn) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-in\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableBoolean::newVariable(operand2->hasPropertyByVariable(operand1)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmNop) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    nop\n", context->currentProgramCounter);
		fflush(stdout);
#endif
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmMark) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    mark\n", context->currentProgramCounter);
		fflush(stdout);
#endif
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmReference) { //  ... x.y
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
#	ifdef XYO_OBJECT_COUNTING_DEBUG
		printf(">%p    reference %d\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#	else
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);

		printf(">%p    reference %u : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#	endif
#endif
		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->getPropertyBySymbol(((VariableSymbol *)operand)->value));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmEnterContext) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    enter-context\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->contextStack->enter(context->pcContext);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmLeaveContext) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    leave-context\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->contextStack->leave(context->pcContext, context->functionContext);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmBreak) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    break\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		ProgramCounter *break_;
		ProgramCounter *finally_;
		ProgramCounter *tryBreak_;

		do {
			if (context->pcContext == nullptr) {
				break;
			};
			if (context->pcContext == static_cast<ExecutiveContextPc *>(context->functionContext)) {
				break;
			};

			break_ = context->pcContext->break_;
			finally_ = context->pcContext->finally_;
			tryBreak_ = context->pcContext->tryBreak_;
			context->contextStack->leave(context->pcContext, context->functionContext);
			if (finally_ != nullptr) {
				if (tryBreak_ != nullptr) {

					context->push(VariableVmProgramCounter::newVariable(tryBreak_));
					context->nextProgramCounter = finally_;
					return;
				};
			};
			if (break_ != nullptr) {
				context->nextProgramCounter = break_;
				return;
			};
		} while (break_ == nullptr);

		context->error = InstructionError::Error;
		context->errorInfo = "break without loop/switch";
		context->nextProgramCounter = nullptr;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContinue) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    continue\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		ProgramCounter *continue_;
		ProgramCounter *finally_;
		ProgramCounter *tryContinue_;
		TPointer<Variable> pc;

		do {
			if (context->pcContext == nullptr) {
				break;
			};
			if (context->pcContext == static_cast<ExecutiveContextPc *>(context->functionContext)) {
				break;
			};

			continue_ = context->pcContext->continue_;
			finally_ = context->pcContext->finally_;
			tryContinue_ = context->pcContext->tryContinue_;
			context->contextStack->leave(context->pcContext, context->functionContext);
			if (finally_ != nullptr) {
				if (tryContinue_ != nullptr) {
					context->push(VariableVmProgramCounter::newVariable(tryContinue_));
					context->nextProgramCounter = finally_;
					return;
				};
			};
			if (continue_ != nullptr) {
				context->nextProgramCounter = continue_;
				return;
			};
		} while (continue_ == nullptr);

		context->error = InstructionError::Error;
		context->errorInfo = "continue without loop";
		context->nextProgramCounter = nullptr;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorEqual1) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-equal-1\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		Variable *operand1;

		TPointer<Variable> operand2(context->popTransfer());

		context->peek(operand1);

		context->push(VariableBoolean::newVariable(operand1->isEqual(operand2)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPop1) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    pop1\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->pop();
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushNull) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    push-null\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		context->push(VariableNull::newVariable());
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmReturn) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    return\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		ProgramCounter *return_;
		ProgramCounter *finally_;
		ProgramCounter *tryReturn_;

		// coroutine - reset this function
		if (context->functionContext->thisFunction_) {
			((VariableVmFunction *)(context->functionContext->thisFunction_.value()))->coroutineContext->empty();
			((VariableVmFunction *)(context->functionContext->thisFunction_.value()))->value = ((VariableVmFunction *)(context->functionContext->thisFunction_.value()))->originalValue;
		};

		do {
			if (context->pcContext == nullptr) {
				break;
			};

			return_ = context->pcContext->pc_;
			finally_ = context->pcContext->finally_;
			tryReturn_ = context->pcContext->tryReturn_;
			context->contextStack->leave(context->pcContext, context->functionContext);
			if (finally_ != nullptr) {
				if (tryReturn_ != nullptr) {
					context->contextStack->enter(context->pcContext);
					context->pcContext->pc_ = tryReturn_;
					context->nextProgramCounter = finally_;
					return;
				};
			};
			if (return_ != nullptr) {
				context->nextProgramCounter = return_;
				return;
			};
		} while (return_ == nullptr);
		context->error = InstructionError::Error;
		context->errorInfo = "return without call";
		context->nextProgramCounter = nullptr;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmThrow) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    throw\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		ProgramCounter *catch_;
		ProgramCounter *finally_;
		ProgramCounter *tryThrow_;

		TPointer<Variable> operand1(context->popTransfer());

		if (!TIsType<VariableObject>(operand1)) {
			operand1 = context->newError(operand1->toString());
		};
		
		if(!context->stackTrace) {
			context->stackTrace.newMemory();
		};
		InstructionTrace tracePoint;
		tracePoint.sourceSymbol = (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(context->currentProgramCounter))->value.sourceSymbol;
		tracePoint.sourceLineNumber = (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(context->currentProgramCounter))->value.sourceLineNumber;
		context->stackTrace->push(tracePoint);

		do {
			if (context->pcContext == nullptr) {
				break;
			};
			if (context->pcContext->pc_) {
				tracePoint.sourceSymbol = (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(context->pcContext->pc_))->value.sourceSymbol;
				tracePoint.sourceLineNumber = (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(context->pcContext->pc_))->value.sourceLineNumber;
				context->stackTrace->push(tracePoint);				
			};
			catch_ = context->pcContext->catch_;
			finally_ = context->pcContext->finally_;
			tryThrow_ = context->pcContext->tryThrow_;

			// reset context stack if set ...
			if (context->pcContext->stackLink_) {
				while (context->pcContext->stackLink_ != context->head) {
					if (context->isEmpty()) {
						break;
					};
					context->pop();
				};
				context->pcContext->stackLink_ = nullptr;
			};
			context->contextStack->leave(context->pcContext, context->functionContext);

			if (catch_ != nullptr) {
				context->push(operand1);
				context->nextProgramCounter = catch_;
				return;
			};
			if (finally_ != nullptr) {
				if (tryThrow_ != nullptr) {
					context->push(operand1);
					context->contextStack->enter(context->pcContext);
					context->pcContext->pc_ = tryThrow_;
					context->nextProgramCounter = finally_;
					return;
				};
			};
		} while (catch_ == nullptr);
		
		context->push(operand1);
		context->error = InstructionError::Throw;
		context->errorInfo = "throw";
		context->nextProgramCounter = nullptr;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetThis) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-this\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->pop(context->functionContext->this_);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextPushThis) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-push-this\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->push(context->functionContext->this_);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushNewObject) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    push-new-object\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->push(VariableObject::newVariable());
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmIfFalseGoto) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    if-false-goto %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		if (!operand1->toBoolean()) {
			context->nextProgramCounter = ((VariableVmProgramCounter *)operand)->value;
		};
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmGoto) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    goto %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->nextProgramCounter = ((VariableVmProgramCounter *)operand)->value;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetBreak) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-break %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->pcContext->break_ = (((VariableVmProgramCounter *)operand)->value);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetContinue) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-continue %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->pcContext->continue_ = (((VariableVmProgramCounter *)operand)->value);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmIfTrueGoto) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    if-true-goto %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		bool retV;

		if (operand1->toBoolean()) {
			context->nextProgramCounter = ((VariableVmProgramCounter *)operand)->value;
		};
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetCatch) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-catch %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->pcContext->catch_ = (((VariableVmProgramCounter *)operand)->value);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetFinally) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-finally %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->pcContext->finally_ = (((VariableVmProgramCounter *)operand)->value);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetTryBreak) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-try-break %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->pcContext->tryBreak_ = (((VariableVmProgramCounter *)operand)->value);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetTryContinue) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-try-continue %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->pcContext->tryContinue_ = (((VariableVmProgramCounter *)operand)->value);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetTryReturn) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-try-return %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->pcContext->tryReturn_ = (((VariableVmProgramCounter *)operand)->value);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetTryThrow) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-try-throw %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->pcContext->tryThrow_ = (((VariableVmProgramCounter *)operand)->value);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmEnterFirstContext) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    enter-first-context\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->contextStack->enterMaster(context->functionContext);
		context->pcContext = context->functionContext;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmAssign) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    assign\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		operand1->referenceSetA1(operand2);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmCallNative) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    call-native %p\n", context->currentProgramCounter, ((VariableNativeVmFunction *)operand)->procedure);
		fflush(stdout);
#endif
		(*((VariableNativeVmFunction *)operand)->procedure)(
		    context,
		    ((VariableNativeVmFunction *)operand)->operand);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorReference) { // ... = x[y]
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-reference\n", context->currentProgramCounter); // x[y]
		fflush(stdout);
#endif
		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->getPropertyByVariable(operand2));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushNewArray) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    push-new-array\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->push(VariableArray::newVariable());
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmArrayPush) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    array-push\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		Variable *operand1;
		TPointer<Variable> operand2(context->popTransfer());

		context->peek(operand1);

		(*((VariableArray *)operand1)->value)[((VariableArray *)operand1)->value->length()] = operand2;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorArrayPush) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-array-push\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		(*((VariableArray *)operand1.value())->value)[((VariableArray *)operand1.value())->value->length()] = operand2;
		context->push(operand2);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushObjectReference) { // x= ... (global.x= ...)
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
#	ifdef XYO_OBJECT_COUNTING_DEBUG
		printf(">%p    push-object-reference %d\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#	else
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);

		printf(">%p    push-object-reference %u : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#	endif
#endif
		context->push(VariableReferenceSymbol::newVariable(Context::getGlobalObject(), ((VariableSymbol *)operand)->value));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmReferenceObjectReference) { // x.y= ...
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
#	ifdef XYO_OBJECT_COUNTING_DEBUG
		printf(">%p    reference-object-reference %d\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#	else
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);

		printf(">%p    reference-object-reference %u : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#	endif
#endif
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableReferenceSymbol::newVariable(operand1, ((VariableSymbol *)operand)->value));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorInstanceOf) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-instance-of\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand1(context->popTransfer());
		TPointer<Variable> operand2(context->popTransfer());

		context->push(VariableBoolean::newVariable(operand2->instanceOf(operand1)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorTypeOf) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-type-of\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableString::newVariable(operand1->getVariableType()));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorReferenceReference) { // x[y]= ...
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-reference-reference\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		context->push(VariableReferenceVariable::newVariable(operand1, operand2));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorPlusPlusLeft) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-plus-plus-left\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->referenceSet(VariableNumber::newVariable(operand1->referenceToNumber() + 1)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorPlusPlusRight) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-plus-plus-right\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->referenceGet());
		operand1->referenceSet(VariableNumber::newVariable(operand1->referenceToNumber() + 1));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorMinusMinusLeft) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-minus-minus-left\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->referenceSet(VariableNumber::newVariable(operand1->referenceToNumber() - 1)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorMinusMinusRight) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-minus-minus-right\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand1(context->popTransfer());

		context->push(operand1->referenceGet());
		operand1->referenceSet(VariableNumber::newVariable(operand1->referenceToNumber() - 1));
	};

	//
	// 1. EnterContext
	// 2. ContextSetArguments
	// 3. ContextSetPc
	// 5. FunctionSetContext
	// 6. ContextSetLocalVariables
	// 7. Call
	//
	// ([function])()

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXCall) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-call\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		TPointer<Variable> operand1(context->popTransfer());

		if (!TIsType<VariableVmFunction>(operand1)) {
			context->push(operand1->functionApply(Context::getValueUndefined(), functionArguments));
			return;
		};

		if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
			TPointer<ExecutiveContextPc> contextTemp;
			((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
			context->contextStack->push(contextTemp);
			(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
			(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;
			while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);
			};
			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

			context->pcContext = (context->contextStack->head)->value;
			context->functionContext = context->pcContext->functionContext;
			return;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    - function-hint %04X\n", context->currentProgramCounter, ((VariableVmFunction *)operand1.value())->functionHint);
		fflush(stdout);
#endif
		context->contextStack->enterMaster(context->functionContext);
		context->pcContext = context->functionContext;
		context->pcContext->pc_ = context->nextProgramCounter;

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::This) {
			context->functionContext->this_ = VariableUndefined::newVariable();
		};
		if ((((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Yield) ||
		    (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Self)) {
			context->functionContext->thisFunction_ = operand1;
		};
		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Arguments) {
			context->functionContext->functionArguments = functionArguments;
		};
		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::LocalVariables) {
			context->functionContext->functionLocalVariables = VariableArray::newArray();
		};

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif
		context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
	};

	//
	// 1. EnterContext
	// 2. ContextSetArguments
	// 3. ContextSetPc
	// 5. FunctionSetContext
	// 6. ContextSetLocalVariables
	// 7. CallSymbol
	//
	// [function]()

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXCallSymbol) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);

		printf(">%p    x-call-symbol %u : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#endif
		TPointer<Variable> operand1;

		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		operand1 = (Context::getGlobalObject())->getPropertyBySymbol(((VariableSymbol *)operand)->value);

		if (!TIsType<VariableVmFunction>(operand1)) {
			if (operand1->isFunction()) {
				context->push(operand1->functionApply(Context::getValueUndefined(), functionArguments));
				return;
			};

			String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);
			String error;
			error << "\"" << symbolName << "\" is not a function";
			context->push(context->newError(error));
			InstructionVmThrow(context, nullptr);
			return;
		};

		if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
			TPointer<ExecutiveContextPc> contextTemp;
			((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
			context->contextStack->push(contextTemp);
			(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
			(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;
			while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);
			};
			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

			context->pcContext = (context->contextStack->head)->value;
			context->functionContext = context->pcContext->functionContext;
			return;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    - function-hint %04X\n", context->currentProgramCounter, ((VariableVmFunction *)operand1.value())->functionHint);
		fflush(stdout);
#endif
		context->contextStack->enterMaster(context->functionContext);
		context->pcContext = context->functionContext;
		context->pcContext->pc_ = context->nextProgramCounter;

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::This) {
			context->functionContext->this_ = VariableUndefined::newVariable();
		};
		if ((((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Yield) ||
		    (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Self)) {
			context->functionContext->thisFunction_ = operand1;
		};
		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Arguments) {
			context->functionContext->functionArguments = functionArguments;
		};
		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::LocalVariables) {
			context->functionContext->functionLocalVariables = VariableArray::newArray();
		};
#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif
		context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
	};

	//
	// 1. EnterFirstContext
	// 2. ContextSetArguments
	// 3. ContextSetPc
	// 4. ContextSetThis
	// 5. FunctionSetContext
	// 6. ContextSetLocalVariables
	// 7. Call
	//
	// new [function]()

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXCallThis) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-call-this\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		TPointer<Variable> operand1(context->popTransfer());
		TPointer<Variable> operand2(context->popTransfer());

		if (!TIsType<VariableVmFunction>(operand1)) {
			context->push(operand1->functionApply(operand2, functionArguments));
			return;
		};

		if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
			TPointer<ExecutiveContextPc> contextTemp;

			((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
			context->contextStack->push(contextTemp);

			context->functionContext->this_ = operand2;

			(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
			(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;
			while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);
			};

			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

			context->pcContext = (context->contextStack->head)->value;
			context->functionContext = context->pcContext->functionContext;

			return;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    - function-hint %04X\n", context->currentProgramCounter, ((VariableVmFunction *)operand1.value())->functionHint);
		fflush(stdout);
#endif

		context->contextStack->enterMaster(context->functionContext);
		context->pcContext = context->functionContext;

		context->functionContext->this_ = operand2;

		context->pcContext->pc_ = context->nextProgramCounter;

		if ((((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Yield) ||
		    (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Self)) {
			context->functionContext->thisFunction_ = operand1;
		};

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Arguments) {
			context->functionContext->functionArguments = functionArguments;
		};

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::LocalVariables) {
			context->functionContext->functionLocalVariables = VariableArray::newArray();
		};

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif

		context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
	};

	//
	// 1. EnterFirstContext
	// 2. ContextSetArguments
	// 3. ContextSetThis
	// 4. ContextSetPc
	// 5. FunctionSetContext
	// 6. ContextSetLocalVariables
	// 7. Call
	//
	// [function].call(...)

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXCallThisModeCall) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-call-this-mode-call\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		if (!TIsType<VariableVmFunction>(operand1)) {
			context->push(operand1->functionApply(operand2, functionArguments));
		};

		if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
			TPointer<ExecutiveContextPc> contextTemp;

			((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
			context->contextStack->push(contextTemp);

			context->functionContext->this_ = operand2;
			(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
			(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;

			while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);
			};

			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

			context->pcContext = (context->contextStack->head)->value;
			context->functionContext = context->pcContext->functionContext;
			return;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    - function-hint %04X\n", context->currentProgramCounter, ((VariableVmFunction *)operand1.value())->functionHint);
		fflush(stdout);
#endif

		context->contextStack->enterMaster(context->functionContext);
		context->pcContext = context->functionContext;
		context->functionContext->this_ = operand2;
		context->pcContext->pc_ = context->nextProgramCounter;

		if ((((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Yield) ||
		    (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Self)) {
			context->functionContext->thisFunction_ = operand1;
		};

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Arguments) {
			context->functionContext->functionArguments = functionArguments;
		};

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::LocalVariables) {
			context->functionContext->functionLocalVariables = VariableArray::newArray();
		};

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif
		context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
	};

	// [function].apply(...)
	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXCallThisModeApply) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-call-this-mode-apply\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		ProgramCounter *return_;

		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		TPointerX<Variable> &operandX = (*(((VariableArray *)(functionArguments.value()))->value))[0];
		if (TIsType<VariableArray>(operandX)) {

			if (!TIsType<VariableVmFunction>(operand1)) {
				context->push(operand1->functionApply(operand2, static_cast<VariableArray *>(operandX.value())));
				return;
			};

			if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				TPointer<ExecutiveContextPc> contextTemp;

				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);

				context->functionContext->this_ = operand2;
				(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
				(context->contextStack->head)->value->functionContext->functionArguments = static_cast<VariableArray *>(operandX.value());

				while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
					((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
					context->contextStack->push(contextTemp);
				};

				context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

				// functionArguments->decReferenceCount();

				context->pcContext = (context->contextStack->head)->value;
				context->functionContext = context->pcContext->functionContext;
				return;
			};

			context->contextStack->enterMaster(context->functionContext);
			context->pcContext = context->functionContext;

			context->functionContext->this_ = operand2;
			context->pcContext->pc_ = context->nextProgramCounter;
			context->functionContext->thisFunction_ = operand1;

			context->functionContext->functionArguments = static_cast<VariableArray *>(operandX.value());

			context->functionContext->functionLocalVariables = VariableArray::newArray();
#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
			context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif
			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
			return;
		};

		context->push(context->newError("parameter not an Array"));
		InstructionVmThrow(context, nullptr);
		return;
	};

	//
	// 1. EnterFirstContext
	// 2. ContextSetArguments
	// 3. ContextSetThis
	// 4. ContextPushThis
	// 5. Reference, symbol
	// 6. ContextSetPc
	// 5. FunctionSetContext
	// 6. ContextSetLocalVariables
	// 7. Call
	//
	// [object].[function]()

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXCallWithThisReference) { //  x.fn()
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);

		printf(">%p    x-call-with-this-reference %u : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#endif
		TPointer<Variable> operand1;
		TPointer<Variable> result;
		TPointer<Variable> prototype;

		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		context->pop(operand1);
		result = operand1->getPropertyBySymbol(((VariableSymbol *)operand)->value);

		if (result) {

			if (!TIsType<VariableVmFunction>(result)) {
				if (result->isFunction()) {
					context->push(result->functionApply(operand1, functionArguments));
					return;
				};

				String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);
				String error;
				error << "\"" << symbolName << "\" is not a function";
				context->push(context->newError(error));
				InstructionVmThrow(context, nullptr);
				return;
			};

			if (!(((VariableVmFunction *)result.value())->coroutineContext->isEmpty())) {
				TPointer<ExecutiveContextPc> contextTemp;

				((VariableVmFunction *)result.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);

				context->functionContext->this_ = operand1;
				(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
				(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;

				while (!(((VariableVmFunction *)result.value())->coroutineContext->isEmpty())) {
					((VariableVmFunction *)result.value())->coroutineContext->pop(contextTemp);
					context->contextStack->push(contextTemp);
				};

				context->nextProgramCounter = ((VariableVmFunction *)result.value())->value;

				context->pcContext = (context->contextStack->head)->value;
				context->functionContext = context->pcContext->functionContext;
				return;
			};

			context->contextStack->enterMaster(context->functionContext);
			context->pcContext = context->functionContext;

			context->functionContext->this_ = operand1;
			context->pcContext->pc_ = context->nextProgramCounter;
			context->functionContext->thisFunction_ = result;
			context->functionContext->functionArguments = functionArguments;

			context->functionContext->functionLocalVariables = VariableArray::newArray();
#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
			context->functionContext->functionParent = ((VariableVmFunction *)result.value())->functionParent;
#endif
			context->nextProgramCounter = ((VariableVmFunction *)result.value())->value;
			return;
		};

		context->push(context->newError("call to a member function on a non-object"));
		InstructionVmThrow(context, nullptr);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXArrayPushWithTransfer) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-array-push-with-transfer\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		Variable *operand1;
		TTransfer<Variable> *operand2 = context->popTransfer();

		context->peek(operand1);

		((((VariableArray *)operand1)->value)->push()).setExecutive(operand2);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmAssignReverse) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    assign-reverse\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		operand2->referenceSetA1(operand1);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmDuplicate) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    duplicate\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->duplicate();
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorAssignXPrototype) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-assign-x-prototype\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		Variable *operand1;
		TPointer<Variable> operand2(context->popTransfer()); // function

		context->peek(operand1); // new object
		Prototype *prototype = nullptr;

		if (operand2->instanceOfPrototype(prototype)) {
			if (TIsType<VariableObject>(operand1)) {
				((VariableObject *)operand1)->prototype = prototype;
				context->push(operand2);
				return;
			};
		};

		context->push(context->newError("assign prototype failed"));
		InstructionVmThrow(context, nullptr);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmArgumentsPushObjectReference) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    arguments-object-reference %lu\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#endif

		context->push(VariableReferenceIndex::newVariable(context->functionContext->functionArguments, ((VariableSymbol *)operand)->value));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmArgumentsPushSymbol) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    arguments-push-symbol %lu\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#endif

		context->push(((VariableArray *)(context->functionContext->functionArguments.value()))->index(((VariableSymbol *)operand)->value));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmAssignNewObject) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    new-assign\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		if (!TIsTypeExact<VariableUndefined>(operand1)) {
			TPointer<Variable> operand2(context->popTransfer());
			context->push(operand1);
			return;
		};
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmCatch) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    catch\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		Variable *operand1;
		context->peek(operand1);
		if (TIsType<VariableObject>(operand1)) {
			Variable *retV = VariableStackTrace::newVariable(context->stackTrace, context);
			context->stackTrace.deleteMemory();
			((VariableStackTrace *)retV)->configPrintStackTraceLimit = context->configPrintStackTraceLimit;
			((VariableObject *)operand1)->value->set(context->symbolStackTrace__, retV);
			return;
		};
	};

	// -> VmXPushFunctionActive, to permit compiled code garbage collect
	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXPushFunction) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-push-function %p\n", context->currentProgramCounter, ((VariableVmFunction *)operand)->value);
		fflush(stdout);
#endif
		// extract,relink,modify
		TDoubleEndedQueue<InstructionX>::Node *pcBegin = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(((VariableVmFunction *)operand)->value);
		TDoubleEndedQueue<InstructionX>::Node *pcEnd = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(((VariableVmFunction *)operand)->valueEnd);
		context->instructionListExecutive->extractList(pcBegin, pcEnd);
		((VariableVmFunction *)operand)->instructionList.newMemory();
		((VariableVmFunction *)operand)->instructionList->setList(pcBegin, pcEnd);
		(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(context->currentProgramCounter))->value.procedure = InstructionVmXPushFunctionActive;
		((VariableVmFunction *)operand)->fnSource = (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(context->currentProgramCounter))->value.sourceSymbol;
		//

		Variable *pc = VariableVmFunction::newVariable(((VariableVmFunction *)operand)->value);
		((VariableVmFunction *)pc)->instructionList = ((VariableVmFunction *)operand)->instructionList;
		((VariableVmFunction *)pc)->valueEnd = ((VariableVmFunction *)operand)->valueEnd;
		((VariableVmFunction *)pc)->functionHint = ((VariableVmFunction *)operand)->functionHint;
		((VariableVmFunction *)pc)->fnSource = ((VariableVmFunction *)operand)->fnSource;
#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		((VariableVmFunction *)pc)->functionParent.newMemory();
		((VariableVmFunction *)pc)->functionParent->functionParent = context->functionContext->functionParent;
#endif

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE

		if (context->functionContext->functionArguments) {
			((VariableVmFunction *)pc)->functionParent->arguments = context->functionContext->functionArguments;
		};

		if (context->functionContext->functionLocalVariables) {
			((VariableVmFunction *)pc)->functionParent->variables = context->functionContext->functionLocalVariables;
		};

#endif
		((VariableVmFunction *)pc)->prototype.newMemory();
		((VariableVmFunction *)pc)->prototype->prototype = VariableObject::newVariable();
		context->push(pc);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmArgumentsLevelPushObjectReference) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    arguments-level-object-reference %d:%d\n", context->currentProgramCounter, ((VariableArgumentLevel *)operand)->value, ((VariableArgumentLevel *)operand)->level);
		fflush(stdout);
#endif
		TPointer<Variable> functionArguments;

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		functionArguments = context->getFunctionArgumentsLevel(((VariableArgumentLevel *)operand)->level);
		if (functionArguments) {
			context->push(VariableReferenceIndex::newVariable(context->functionContext->functionArguments, ((VariableArgumentLevel *)operand)->value));
			return;
		};
#endif

		context->push(context->newError("arguments reference"));
		InstructionVmThrow(context, nullptr);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmArgumentsLevelPushSymbol) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    arguments-level-push-symbol %d:%d\n", context->currentProgramCounter, ((VariableArgumentLevel *)operand)->value, ((VariableArgumentLevel *)operand)->level);
		fflush(stdout);
#endif

		TPointer<Variable> functionArguments;

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		functionArguments = context->getFunctionArgumentsLevel(((VariableArgumentLevel *)operand)->level);
		if (functionArguments) {
			context->push(((VariableArray *)(functionArguments.value()))->value->getValue(((VariableArgumentLevel *)operand)->value, Context::getValueUndefined()));
			return;
		};
#endif

		context->push(context->newError("arguments symbol"));
		InstructionVmThrow(context, nullptr);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmLocalVariablesPushObjectReference) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    local-variables-object-reference %lu\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#endif
		context->push(VariableReferenceIndex::newVariable(context->functionContext->functionLocalVariables, ((VariableSymbol *)operand)->value));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmLocalVariablesPushSymbol) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    local-variables-push-symbol %lu\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#endif
		context->push(((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableSymbol *)operand)->value));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmLocalVariablesLevelPushObjectReference) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    local-variables-level-object-reference %d:%d\n", context->currentProgramCounter, ((VariableArgumentLevel *)operand)->value, ((VariableArgumentLevel *)operand)->level);
		fflush(stdout);
#endif
		TPointer<Variable> functionLocalVariables;

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		functionLocalVariables = context->getFunctionLocalVariablesLevel(((VariableArgumentLevel *)operand)->level);
		if (functionLocalVariables) {
			context->push(VariableReferenceIndex::newVariable(functionLocalVariables, ((VariableArgumentLevel *)operand)->value));
			return;
		};
#endif

		context->push(context->newError("variables reference"));
		InstructionVmThrow(context, nullptr);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmLocalVariablesLevelPushSymbol) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    local-variables-level-push-symbol %d:%d\n", context->currentProgramCounter, ((VariableArgumentLevel *)operand)->value, ((VariableArgumentLevel *)operand)->level);
		fflush(stdout);
#endif

		TPointer<Variable> functionLocalVariables;

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		functionLocalVariables = context->getFunctionLocalVariablesLevel(((VariableArgumentLevel *)operand)->level);
		if (functionLocalVariables) {
			context->push(((VariableArray *)(functionLocalVariables.value()))->value->getValue(((VariableArgumentLevel *)operand)->value, Context::getValueUndefined()));
			return;
		};
#endif

		context->push(context->newError("variables symbol"));
		InstructionVmThrow(context, nullptr);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmEndExecution) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    end-execution\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->nextProgramCounter = nullptr;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmIfArgumentsSymbolNotEqualNumberGoto) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    if-arguments-symbol-not-equal-number-goto %lu %g %p\n", context->currentProgramCounter,
		       ((VariableOperator31 *)operand)->symbol, ((VariableOperator31 *)operand)->value, ((VariableOperator31 *)operand)->pc);
		fflush(stdout);
#endif
		Variable *symbolValue;
		bool retV;

		symbolValue = (((VariableArray *)(context->functionContext->functionArguments.value()))->index(((VariableOperator31 *)operand)->symbol)).value();
		if (symbolValue->toNumber() != ((VariableOperator31 *)operand)->value) {
			context->nextProgramCounter = ((VariableOperator31 *)operand)->pc;
		};
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorMinusArgumentsSymbolXNumber) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-minus-arguments-symbol-x-number %lu %g\n", context->currentProgramCounter,
		       ((VariableOperator21 *)operand)->symbol, ((VariableOperator21 *)operand)->value);
		fflush(stdout);
#endif
		context->push(VariableNumber::newVariable(
		    ((((VariableArray *)(context->functionContext->functionArguments.value()))->index(((VariableOperator21 *)operand)->symbol))->toNumber()) - (((VariableOperator21 *)operand)->value)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmSymbolPlusPlus) { // ++x  or x++ no operator
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
#	ifdef XYO_OBJECT_COUNTING_DEBUG
		printf(">%p    symbol-plus-plus %lu\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#	else
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);

		printf(">%p    symbol-plus-plus %lu : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#	endif
#endif
		TPointer<Variable> value_ = (Context::getGlobalObject())->getPropertyBySymbol(((VariableSymbol *)operand)->value);
		(Context::getGlobalObject())->setPropertyBySymbol(((VariableSymbol *)operand)->value, VariableNumber::newVariable((value_->toNumber()) + 1));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmIfSymbolNotLessNumberGoto) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    if-symbol-not-less-number-goto %lu " XYO_QUANTUMSCRIPT_FORMAT_NUMBER " %p\n", context->currentProgramCounter,
		       ((VariableOperator31 *)operand)->symbol, ((VariableOperator31 *)operand)->value, ((VariableOperator31 *)operand)->pc);
		fflush(stdout);
#endif
		TPointer<Variable> symbolValue;
		int retV;
		symbolValue = (Context::getGlobalObject())->getPropertyBySymbol(((VariableOperator31 *)operand)->symbol);
		if (!((symbolValue->toNumber()) < ((VariableOperator31 *)operand)->value)) {
			context->nextProgramCounter = ((VariableOperator31 *)operand)->pc;
		};
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmIfArgumentsSymbolNotLessNumberGoto) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    if-arguments-symbol-not-less-number-goto %lu " XYO_QUANTUMSCRIPT_FORMAT_NUMBER " %p\n", context->currentProgramCounter,
		       ((VariableOperator31 *)operand)->symbol, ((VariableOperator31 *)operand)->value, ((VariableOperator31 *)operand)->pc);
		fflush(stdout);
#endif
		Variable *symbolValue;
		int retV;

		symbolValue = (((VariableArray *)(context->functionContext->functionArguments.value()))->index(((VariableOperator31 *)operand)->symbol)).value();

		if (!((symbolValue->toNumber()) < ((VariableOperator31 *)operand)->value)) {
			context->nextProgramCounter = ((VariableOperator31 *)operand)->pc;
		};
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmYield) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    yield\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		ProgramCounter *return_;
		ProgramCounter *finally_;
		ProgramCounter *tryReturn_;

		// coroutine - yield this function
		if (context->functionContext->thisFunction_) {
			TPointer<ExecutiveContextPc> contextTemp;
			TPointer<Variable> thisFunction;

			thisFunction = context->functionContext->thisFunction_;
			((VariableVmFunction *)(thisFunction.value()))->coroutineContext->empty();

			((VariableVmFunction *)(thisFunction.value()))->value = context->nextProgramCounter;

			while ((context->contextStack->head)->value->pc_ == nullptr) {
				context->contextStack->pop(contextTemp);
				((VariableVmFunction *)(thisFunction.value()))->coroutineContext->push(contextTemp);
			};
			//
			context->contextStack->pop(contextTemp);
			((VariableVmFunction *)(thisFunction.value()))->coroutineContext->push(contextTemp);

			context->nextProgramCounter = contextTemp->pc_;
			context->pcContext = (context->contextStack->head)->value;
			context->functionContext = context->pcContext->functionContext;
			return;
		};

		context->push(context->newError("yield"));
		InstructionVmThrow(context, nullptr);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetReference) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-reference\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand1(context->popTransfer());

		context->pcContext->referenceValue = operand1;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorSetReferenceIndexKey) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-set-reference-index-key\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		context->pcContext->iteratorValue = operand1->getIteratorKey();
		context->push(VariableBoolean::newVariable(
		    context->pcContext->iteratorValue->next(context->pcContext->referenceValue)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorNextReferenceIndex) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-next-reference-index\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		context->push(VariableBoolean::newVariable(
		    context->pcContext->iteratorValue->next(context->pcContext->referenceValue)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorSetReferenceIndexValue) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-set-reference-index-value\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());

		context->pcContext->iteratorValue = operand1->getIteratorValue();
		context->push(VariableBoolean::newVariable(
		    context->pcContext->iteratorValue->next(context->pcContext->referenceValue)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetRegisterValue) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-register-value\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand1(context->popTransfer());
		context->pcContext->registerValue = operand1;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextPushRegisterValue) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-push-register-value\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->push(context->pcContext->registerValue);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorMulLocalVariablesSymbol2) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-mul-local-variables-symbol-2 %lu %lu\n", context->currentProgramCounter,
		       ((VariableOperator22 *)operand)->symbol1, ((VariableOperator22 *)operand)->symbol2);
		fflush(stdout);
#endif

		Variable *operand1;
		Variable *operand2;

		operand2 = (((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableOperator22 *)operand)->symbol1)).value();
		operand1 = (((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableOperator22 *)operand)->symbol2)).value();

		context->push(VariableNumber::newVariable((operand1->toNumber()) * (operand2->toNumber())));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorMinusLocalVariablesSymbol2) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-minus-local-variables-symbol-2 %lu %lu\n", context->currentProgramCounter,
		       ((VariableOperator22 *)operand)->symbol1, ((VariableOperator22 *)operand)->symbol2);
		fflush(stdout);
#endif

		Variable *operand1;
		Variable *operand2;

		operand2 = (((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableOperator22 *)operand)->symbol1)).value();
		operand1 = (((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableOperator22 *)operand)->symbol2)).value();

		context->push(VariableNumber::newVariable((operand1->toNumber()) - (operand2->toNumber())));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorPlusLocalVariablesSymbol2) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-plus-local-variables-symbol-2 %lu %lu\n", context->currentProgramCounter,
		       ((VariableOperator22 *)operand)->symbol1, ((VariableOperator22 *)operand)->symbol2);
		fflush(stdout);
#endif

		Variable *operand1;
		Variable *operand2;
		Variable *result;

		operand2 = (((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableOperator22 *)operand)->symbol1)).value();
		operand1 = (((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableOperator22 *)operand)->symbol2)).value();
		context->push(operand1->operatorPlus(operand2));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXPushNewArray) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-push-new-array\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->push(VariableArray::newVariable());
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextPushSelf) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-push-self\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->push(context->functionContext->thisFunction_);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmCurrentContextExit) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    current-context-exit\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> result(context->popTransfer());
		context->returnValue = result;
		context->nextProgramCounter = nullptr;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushArguments) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    push-arguments\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->push(context->functionContext->functionArguments.value());
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmSetFunction) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    set-function\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		TPointer<Variable> operand1(context->popTransfer());
		operand1->referenceSet(operand);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmIfNotGreaterGoto) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    if-not-greater-goto %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		if (!operand1->isGreater(operand2)) {
			context->nextProgramCounter = ((VariableVmProgramCounter *)operand)->value;
		};
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXPushFunctionActive) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-push-function-active %p\n", context->currentProgramCounter, ((VariableVmFunction *)operand)->value);
		fflush(stdout);
#endif
		Variable *pc = VariableVmFunction::newVariable(((VariableVmFunction *)operand)->value);
		((VariableVmFunction *)pc)->instructionList = ((VariableVmFunction *)operand)->instructionList;
		((VariableVmFunction *)pc)->valueEnd = ((VariableVmFunction *)operand)->valueEnd;
		((VariableVmFunction *)pc)->functionHint = ((VariableVmFunction *)operand)->functionHint;
		((VariableVmFunction *)pc)->fnSource = ((VariableVmFunction *)operand)->fnSource;
#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		((VariableVmFunction *)pc)->functionParent.newMemory();
		((VariableVmFunction *)pc)->functionParent->functionParent = context->functionContext->functionParent;
#endif

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE

		if (context->functionContext->functionArguments) {
			((VariableVmFunction *)pc)->functionParent->arguments = context->functionContext->functionArguments;
		};

		if (context->functionContext->functionLocalVariables) {
			((VariableVmFunction *)pc)->functionParent->variables = context->functionContext->functionLocalVariables;
		};

#endif
		((VariableVmFunction *)pc)->prototype.newMemory();
		((VariableVmFunction *)pc)->prototype->prototype = VariableObject::newVariable();
		context->push(pc);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmInstructionListExtractAndDelete) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    instruction-list-extract-and-delete %p %p and goto %p\n", context->currentProgramCounter, ((VariableOperator23 *)operand)->linkBegin, context->currentProgramCounter, ((VariableOperator23 *)operand)->pc);
		fflush(stdout);
#endif
		InstructionList instructionList;
		TDoubleEndedQueue<InstructionX>::Node *pcBegin = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(((VariableOperator23 *)operand)->linkBegin);
		TDoubleEndedQueue<InstructionX>::Node *pcEnd = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(context->currentProgramCounter);
		context->nextProgramCounter = ((VariableOperator23 *)operand)->pc;
		context->instructionListExecutive->extractList(pcBegin, pcEnd);
		instructionList.setList(pcBegin, pcEnd);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorReferenceDeleteIndex) { // delete x[y]
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-reference-delete-index\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<Variable> operand2;
		context->pop(operand2);
		TPointer<Variable> operand1;
		context->pop(operand1);

		context->push(VariableBoolean::newVariable(operand1->deletePropertyByVariable(operand2)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorReferenceDeleteReference) { // delete x.y
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
#	ifdef XYO_OBJECT_COUNTING_DEBUG
		printf(">%p    operator-reference-delete-reference %d\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#	else
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);

		printf(">%p    operator-reference-delete-reference %u : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#	endif
#endif
		TPointer<Variable> operand1;
		context->pop(operand1);
		context->push(VariableBoolean::newVariable(operand1->deletePropertyBySymbol(((VariableSymbol *)operand)->value)));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetStack) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-stack\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->pcContext->stackLink_ = context->head;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmClearIncludedFile) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    clear-included-file\n", context->currentProgramCounter);
		fflush(stdout);
#endif

		context->includedFile.pop();
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmOperatorPow2LocalVariablesSymbol) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    operator-pow2-local-variables-symbol %lu\n", context->currentProgramCounter,
		       ((VariableSymbol *)operand)->value);
		fflush(stdout);
#endif

		Variable *operandX;

		operandX = (((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableSymbol *)operand)->value)).value();

		context->push(VariableNumber::newVariable((operandX->toNumber()) * (operandX->toNumber())));
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXTailCall) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-tail-call\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		TPointer<Variable> operand1(context->popTransfer());

		if (!TIsType<VariableVmFunction>(operand1)) {
			context->push(operand1->functionApply(Context::getValueUndefined(), functionArguments));
			InstructionVmReturn(context, nullptr);
			return;
		};

		if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
			TPointer<ExecutiveContextPc> contextTemp;
			((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
			context->contextStack->push(contextTemp);
			(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
			(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;
			while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);
			};
			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

			context->pcContext = (context->contextStack->head)->value;
			context->functionContext = context->pcContext->functionContext;
			return;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    - function-hint %04X\n", context->currentProgramCounter, ((VariableVmFunction *)operand1.value())->functionHint);
		fflush(stdout);
#endif

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::This) {
			context->functionContext->this_ = VariableUndefined::newVariable();
		};
		if ((((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Yield) ||
		    (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Self)) {
			context->functionContext->thisFunction_ = operand1;
		};
		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Arguments) {
			context->functionContext->functionArguments = functionArguments;
		};
		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::LocalVariables) {
			context->functionContext->functionLocalVariables = VariableArray::newArray();
		};

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif
		context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXTailCallSymbol) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);

		printf(">%p    x-tail-call-symbol %u : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#endif
		TPointer<Variable> operand1;

		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		operand1 = (Context::getGlobalObject())->getPropertyBySymbol(((VariableSymbol *)operand)->value);

		if (!TIsType<VariableVmFunction>(operand1)) {
			context->push(operand1->functionApply(Context::getValueUndefined(), functionArguments));
			InstructionVmReturn(context, nullptr);
			return;
		};

		if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
			TPointer<ExecutiveContextPc> contextTemp;
			((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
			context->contextStack->push(contextTemp);
			(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
			(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;
			while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);
			};
			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

			context->pcContext = (context->contextStack->head)->value;
			context->functionContext = context->pcContext->functionContext;
			return;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    - function-hint %04X\n", context->currentProgramCounter, ((VariableVmFunction *)operand1.value())->functionHint);
		fflush(stdout);
#endif

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::This) {
			context->functionContext->this_ = VariableUndefined::newVariable();
		};
		if ((((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Yield) ||
		    (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Self)) {
			context->functionContext->thisFunction_ = operand1;
		};
		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Arguments) {
			context->functionContext->functionArguments = functionArguments;
		};
		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::LocalVariables) {
			context->functionContext->functionLocalVariables = VariableArray::newArray();
		};
#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif
		context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXTailCallThis) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-tail-call-this\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		TPointer<Variable> operand1(context->popTransfer());
		TPointer<Variable> operand2(context->popTransfer());

		if (!TIsType<VariableVmFunction>(operand1)) {
			context->push(operand1->functionApply(operand2, functionArguments));
			InstructionVmReturn(context, nullptr);
			return;
		};

		if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
			TPointer<ExecutiveContextPc> contextTemp;

			((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
			context->contextStack->push(contextTemp);

			context->functionContext->this_ = operand2;

			(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
			(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;
			while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);
			};

			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

			context->pcContext = (context->contextStack->head)->value;
			context->functionContext = context->pcContext->functionContext;

			return;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    - function-hint %04X\n", context->currentProgramCounter, ((VariableVmFunction *)operand1.value())->functionHint);
		fflush(stdout);
#endif

		context->functionContext->this_ = operand2;

		if ((((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Yield) ||
		    (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Self)) {
			context->functionContext->thisFunction_ = operand1;
		};

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Arguments) {
			context->functionContext->functionArguments = functionArguments;
		};

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::LocalVariables) {
			context->functionContext->functionLocalVariables = VariableArray::newArray();
		};

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif

		context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXTailCallThisModeCall) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-tail-call-this-mode-call\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		if (!TIsType<VariableVmFunction>(operand1)) {
			context->push(operand1->functionApply(operand2, functionArguments));
			InstructionVmReturn(context, nullptr);
			return;
		};

		if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
			TPointer<ExecutiveContextPc> contextTemp;

			((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
			context->contextStack->push(contextTemp);

			context->functionContext->this_ = operand2;
			(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
			(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;

			while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);
			};

			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

			context->pcContext = (context->contextStack->head)->value;
			context->functionContext = context->pcContext->functionContext;
			return;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    - function-hint %04X\n", context->currentProgramCounter, ((VariableVmFunction *)operand1.value())->functionHint);
		fflush(stdout);
#endif

		context->functionContext->this_ = operand2;

		if ((((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Yield) ||
		    (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Self)) {
			context->functionContext->thisFunction_ = operand1;
		};

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::Arguments) {
			context->functionContext->functionArguments = functionArguments;
		};

		if (((VariableVmFunction *)operand1.value())->functionHint & ParserFunctionHint::LocalVariables) {
			context->functionContext->functionLocalVariables = VariableArray::newArray();
		};

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
		context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif
		context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXTailCallThisModeApply) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    x-tail-call-this-mode-apply\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		ProgramCounter *return_;

		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		TPointer<Variable> operand2(context->popTransfer());
		TPointer<Variable> operand1(context->popTransfer());

		TPointerX<Variable> &operandX = (*(((VariableArray *)(functionArguments.value()))->value))[0];
		if (TIsType<VariableArray>(operandX)) {

			if (!TIsType<VariableVmFunction>(operand1)) {
				context->push(operand1->functionApply(operand2, static_cast<VariableArray *>(operandX.value())));
				InstructionVmReturn(context, nullptr);
				return;
			};

			if (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
				TPointer<ExecutiveContextPc> contextTemp;

				((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);

				context->functionContext->this_ = operand2;
				(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
				(context->contextStack->head)->value->functionContext->functionArguments = static_cast<VariableArray *>(operandX.value());

				while (!(((VariableVmFunction *)operand1.value())->coroutineContext->isEmpty())) {
					((VariableVmFunction *)operand1.value())->coroutineContext->pop(contextTemp);
					context->contextStack->push(contextTemp);
				};

				context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;

				// functionArguments->decReferenceCount();

				context->pcContext = (context->contextStack->head)->value;
				context->functionContext = context->pcContext->functionContext;
				return;
			};

			context->functionContext->this_ = operand2;
			context->functionContext->thisFunction_ = operand1;

			context->functionContext->functionArguments = static_cast<VariableArray *>(operandX.value());

			context->functionContext->functionLocalVariables = VariableArray::newArray();
#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
			context->functionContext->functionParent = ((VariableVmFunction *)operand1.value())->functionParent;
#endif
			context->nextProgramCounter = ((VariableVmFunction *)operand1.value())->value;
			return;
		};

		context->push(context->newError("parameter not an Array"));
		InstructionVmThrow(context, nullptr);
		return;
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmXTailCallWithThisReference) { //  x.fn()
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		String symbolName = Context::getSymbolMirror(((VariableSymbol *)operand)->value);

		printf(">%p    x-tail-call-with-this-reference %u : %s\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value, symbolName.value());
		fflush(stdout);
#endif
		TPointer<Variable> operand1;
		TPointer<Variable> result;
		TPointer<Variable> prototype;

		TPointer<VariableArray> functionArguments(TTransfer<VariableArray, Variable>::cast(context->popTransfer()));

		context->pop(operand1);
		result = operand1->getPropertyBySymbol(((VariableSymbol *)operand)->value);

		if (result) {

			if (!TIsType<VariableVmFunction>(result)) {
				context->push(result->functionApply(operand1, functionArguments));
				InstructionVmReturn(context, nullptr);
				return;
			};

			if (!(((VariableVmFunction *)result.value())->coroutineContext->isEmpty())) {
				TPointer<ExecutiveContextPc> contextTemp;

				((VariableVmFunction *)result.value())->coroutineContext->pop(contextTemp);
				context->contextStack->push(contextTemp);

				context->functionContext->this_ = operand1;
				(context->contextStack->head)->value->pc_ = context->nextProgramCounter;
				(context->contextStack->head)->value->functionContext->functionArguments = functionArguments;

				while (!(((VariableVmFunction *)result.value())->coroutineContext->isEmpty())) {
					((VariableVmFunction *)result.value())->coroutineContext->pop(contextTemp);
					context->contextStack->push(contextTemp);
				};

				context->nextProgramCounter = ((VariableVmFunction *)result.value())->value;

				context->pcContext = (context->contextStack->head)->value;
				context->functionContext = context->pcContext->functionContext;
				return;
			};

			context->functionContext->this_ = operand1;
			context->functionContext->thisFunction_ = result;
			context->functionContext->functionArguments = functionArguments;

			context->functionContext->functionLocalVariables = VariableArray::newArray();
#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
			context->functionContext->functionParent = ((VariableVmFunction *)result.value())->functionParent;
#endif
			context->nextProgramCounter = ((VariableVmFunction *)result.value())->value;
			return;
		};

		context->push(context->newError("call to a member function on a non-object"));
		InstructionVmThrow(context, nullptr);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmAssignLocalVariablesObjectReferencePow2LocalVariablesSymbol) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    assign-local-variables-object-reference-pow2-local-variables-symbol %lu %lu\n", context->currentProgramCounter,
		       ((VariableOperator22 *)operand)->symbol1,
		       ((VariableOperator22 *)operand)->symbol2);
		fflush(stdout);
#endif

		Number value = (((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableOperator22 *)operand)->symbol2))->toNumber();
		(((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableOperator22 *)operand)->symbol1)) = VariableNumber::newVariable(value * value);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmLocalVariablesPlusPlus) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    local-variables-plus-plus %lu\n", context->currentProgramCounter, ((VariableSymbol *)operand)->value);
		fflush(stdout);
#endif
		TPointerX<Variable> &refObject = (((VariableArray *)(context->functionContext->functionLocalVariables.value()))->index(((VariableSymbol *)operand)->value));
		refObject = VariableNumber::newVariable((refObject->toNumber()) + 1);
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmPushGlobal) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    push-global\n", context->currentProgramCounter);
		fflush(stdout);
#endif
		context->push(Context::getGlobalObject());
	};

	XYO_QUANTUMSCRIPT_INSTRUCTION_IMPLEMENT(VmContextSetPC) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_RUNTIME
		printf(">%p    context-set-pc %p\n", context->currentProgramCounter, ((VariableVmProgramCounter *)operand)->value);
		fflush(stdout);
#endif
		context->pcContext->pc_ = (((VariableVmProgramCounter *)operand)->value);
	};

};
