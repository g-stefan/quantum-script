// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript/Asm.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>
#include <XYO/QuantumScript/ProgramCounter.hpp>
#include <XYO/QuantumScript/InstructionX.hpp>

#include <XYO/QuantumScript/VariableSymbol.hpp>
#include <XYO/QuantumScript/VariableBoolean.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableString.hpp>

#include <XYO/QuantumScript/VariableArgumentLevel.hpp>
#include <XYO/QuantumScript/VariableVmProgramCounter.hpp>
#include <XYO/QuantumScript/VariableVmFunction.hpp>
#include <XYO/QuantumScript/VariableOperator21.hpp>
#include <XYO/QuantumScript/VariableOperator22.hpp>
#include <XYO/QuantumScript/VariableOperator23.hpp>
#include <XYO/QuantumScript/VariableOperator31.hpp>

#define XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Type, Operand)                                                      \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.asmType = ParserAsm::Type;       \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.procedure = InstructionVm##Type; \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.operand = Operand;               \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.sourceSymbol = sourceSymbol;     \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.sourceLineNumber = sourceLineNumber;

#define XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(Type, Operand)                                                   \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.asmType = ParserAsm::Type;       \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.procedure = InstructionVm##Type; \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.operand = Operand;               \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.sourceSymbol = sourceSymbol;     \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.sourceLineNumber = sourceLineNumber;

namespace XYO::ManagedMemory {

	template <>
	class TComparator<XYO::QuantumScript::ProgramCounter *> {
		public:
			typedef XYO::QuantumScript::ProgramCounter *T;

			inline static bool isLess(const T &a, const T &b) {
				return (compare(a, b) < 0);
			};

			inline static int compare(const T &a, const T &b) {
				return ((uint8_t *)(const_cast<T>(a))) - ((uint8_t *)const_cast<T>(b));
			};
	};

};

namespace XYO::QuantumScript {

	Asm::Asm() {
		lastIsMark_ = false;
		asmLink_.empty();
	};

	void Asm::resetLinks() {
		asmLink_.empty();
	};

	void Asm::endProcessing() {
		if (instructionList) {
			instructionList->empty();
		};
		asmLink_.empty();
	};

	ProgramCounter *Asm::assembleDirect(InstructionProcedure procedure, Variable *operand) {
		TDoubleEndedQueue<InstructionX>::Node *pc_;
		ProgramCounter *pc;
		if (lastIsMark_) {
			lastIsMark_ = false;
		} else {
			instructionList->pushToTail();
		};
		pc_ = instructionList->tail;
		pc = reinterpret_cast<ProgramCounter *>(pc_);

		reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.procedure = procedure;
		reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.operand = operand;
		return optimizeCode(pc);
	};

	ProgramCounter *Asm::assemble(int type_, const char *name_, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
		TDoubleEndedQueue<InstructionX>::Node *pc_;
		ProgramCounter *pc;
		if (lastIsMark_) {
			lastIsMark_ = false;
		} else {
			instructionList->pushToTail();
		};

		pc_ = instructionList->tail;
		pc = reinterpret_cast<ProgramCounter *>(pc_);

		switch (type_) {
		case ParserAsm::PushUndefined: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushUndefined, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-undefined\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushSymbol: {
			Symbol symbolId = Context::getSymbol(name_);
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushSymbol, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-symbol %s : %u\n", pc, name_, symbolId);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushBoolean: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushBoolean, VariableBoolean::newVariable(StringCore::isEqual(name_, "true")));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-boolean %s\n", pc, name_);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushNumber: {
			Number valueNumber;
			if (sscanf(name_, XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT, &valueNumber) != 1) {
				valueNumber = 0;
			};
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushNumber, VariableNumber::newVariable(valueNumber));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-number %s\n", pc, name_);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushNaN: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushNumber, VariableNumber::newVariable(NAN));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-nan\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushInfinity: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushNumber, VariableNumber::newVariable(INFINITY));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-infinity\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssign: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssign, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignPlus: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignPlus, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-plus\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignMinus: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignMinus, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-minus\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignMul: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignMul, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-mul\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignDiv: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignDiv, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-div\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignMod: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignMod, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-mod\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignBitwiseOr: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseOr, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-bitwise-or\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignBitwiseAnd: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseAnd, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-bitwise-and\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignBitwiseLeftShift: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseLeftShift, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-bitwise-left-shift\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignBitwiseRightShift: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseRightShift, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-bitwise-right-shift\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignBitwiseRightShiftZero: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseRightShiftZero, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-bitwise-right-shift-zero\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorAssignBitwiseXor: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseXor, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-bitwise-xor\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorEqual: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorEqual, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-equal\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorEqualStrict: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorEqualStrict, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-equal-strict\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorPlus: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorPlus, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-plus\n", pc);
#endif
			return optimizeCode(pc);
		};
		    break;
		case ParserAsm::OperatorMinus: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorMinus, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-minus\n", pc);
#endif
			return optimizeCode(pc);
		};
		    break;
		case ParserAsm::OperatorUnaryPlus: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorUnaryPlus, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-unary-plus\n", pc);
#endif
			return optimizeCode(pc);
		};
		    break;
		case ParserAsm::OperatorMul: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorMul, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-mul\n", pc);
#endif
			return optimizeCode(pc);
		};
		    break;
		case ParserAsm::OperatorDiv: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorDiv, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-div\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorMod: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorMod, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-mod\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorNotEqual: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorNotEqual, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-not-equal\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorNotEqualStrict: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorNotEqualStrict, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-not-equal-strict\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorNot: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorNot, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-not\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorLessOrEqual: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorLessOrEqual, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-less-or-equal\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorLess: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorLess, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-less\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorGreaterOrEqual: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorGreaterOrEqual, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-greater-or-equal\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorGreater: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorGreater, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-greater\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorLogicalOr: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorLogicalOr, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-logical-or\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorBitwiseOr: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorBitwiseOr, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-bitwise-or\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorLogicalAnd: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorLogicalAnd, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-logical-and\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorBitwiseAnd: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorBitwiseAnd, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-bitwise-and\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorBitwiseNot: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorBitwiseNot, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-bitwise-not\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorBitwiseLeftShift: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorBitwiseLeftShift, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-bitwise-left-shift\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorBitwiseRightShift: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorBitwiseRightShift, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-bitwise-right-shift\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorBitwiseRightShiftZero: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorBitwiseRightShiftZero, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-bitwise-right-shift-zero\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorBitwiseXor: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorBitwiseXor, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-bitwise-xor\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorIn: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorIn, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-in\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::Nop: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Nop, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    nop\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::Reference: {
			Symbol symbolId = Context::getSymbol(name_);
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Reference, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    reference %s : %u\n", pc, name_, symbolId);
#endif
			return pc;
		};
		    break;
		case ParserAsm::EnterContext: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(EnterContext, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    enter-context\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::LeaveContext: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(LeaveContext, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    leave-context\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::Mark: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Mark, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    mark\n", pc);
#endif
			lastIsMark_ = true;
			return pc;
		};
		    break;
		case ParserAsm::Break: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Break, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    break\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::Continue: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Continue, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    continue\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorEqual1: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorEqual1, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-equal1\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::Pop1: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Pop1, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    pop1\n", pc);
#endif

			return optimizeCode(pc);
		};
		    break;
		case ParserAsm::PushNull: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushNull, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-null\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::Return: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Return, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    return\n", pc);
#endif
			return optimizeCode(pc);
		};
		    break;
		case ParserAsm::Throw: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Throw, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    throw\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::ContextSetThis: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ContextSetThis, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-this\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::ContextPushThis: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ContextPushThis, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-push-this\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushNewObject: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushNewObject, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-new-object\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushString: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushString, VariableString::newVariable(name_));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-string %s\n", pc, name_);
#endif
			return pc;
		};
		    break;
		case ParserAsm::EnterFirstContext: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(EnterFirstContext, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    enter-first-context\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::Assign: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Assign, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    assign\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorReference: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorReference, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-reference\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushNewArray: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushNewArray, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-new-array\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::ArrayPush: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ArrayPush, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    array-push\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorArrayPush: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorArrayPush, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-array-push\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushObjectReference: {
			Symbol symbolId = Context::getSymbol(name_);
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushObjectReference, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-object-reference %s : %u\n", pc, name_, symbolId);
#endif
			return pc;
		};
		    break;
		case ParserAsm::ReferenceObjectReference: {
			Symbol symbolId = Context::getSymbol(name_);
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ReferenceObjectReference, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    reference-object-reference %s : %u\n", pc, name_, symbolId);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorInstanceOf: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorInstanceOf, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-instance-of\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::OperatorTypeOf: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorTypeOf, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-type-of\n", pc);
#endif
			return pc;
		};

		case ParserAsm::OperatorReferenceReference: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorReferenceReference, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-reference-reference\n", pc);
#endif
			return pc;
		};

		case ParserAsm::OperatorPlusPlusLeft: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorPlusPlusLeft, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-plus-plus-left\n", pc);
#endif
			return pc;
		};
		case ParserAsm::OperatorPlusPlusRight: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorPlusPlusRight, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-plus-plus-right\n", pc);
#endif
			return pc;
		};
		case ParserAsm::OperatorMinusMinusLeft: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorMinusMinusLeft, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-minus-minus-left\n", pc);
#endif
			return pc;
		};
		case ParserAsm::OperatorMinusMinusRight: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorMinusMinusRight, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-minus-minus-right\n", pc);
#endif
			return pc;
		};

		case ParserAsm::XCall: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(XCall, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    x-call\n", pc);
#endif
			return pc;
		};
		case ParserAsm::XCallThis: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(XCallThis, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    x-call-this\n", pc);
#endif
			return pc;
		};
		case ParserAsm::XCallThisModeCall: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(XCallThisModeCall, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    x-call-this-mode-call\n", pc);
#endif
			return pc;
		};
		case ParserAsm::XCallThisModeApply: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(XCallThisModeApply, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    x-call-this-mode-apply\n", pc);
#endif
			return pc;
		};
		case ParserAsm::XCallWithThisReference: {
			Symbol symbolId = Context::getSymbol(name_);
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(XCallWithThisReference, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    x-call-with-this-reference %s : %u\n", pc, name_, symbolId);
#endif
			return pc;
		};
		case ParserAsm::XCallSymbol: {
			Symbol symbolId = Context::getSymbol(name_);
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(XCallSymbol, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    x-call-symbol %s : %u\n", pc, name_, symbolId);
#endif
			return pc;
		};
		case ParserAsm::XArrayPushWithTransfer: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(XArrayPushWithTransfer, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    x-array-push-with-transfer\n", pc);
#endif
			return pc;
		};
		case ParserAsm::AssignReverse: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(AssignReverse, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    assign-reverse\n", pc);
#endif
			return pc;
		};
		case ParserAsm::Duplicate: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Duplicate, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    duplicate\n", pc);
#endif
			return pc;
		};
		case ParserAsm::OperatorAssignXPrototype: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorAssignXPrototype, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-assign-x-prototype\n", pc);
#endif
			return pc;
		};

		case ParserAsm::ArgumentsPushObjectReference: {
			uint32_t symbolId;
			if (sscanf(name_, XYO_QUANTUMSCRIPT_FORMAT_DWORD, &symbolId) != 1) {
				symbolId = 0;
			};
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ArgumentsPushObjectReference, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    arguments-push-object-reference %s\n", pc, name_);
#endif
			return pc;
		};

		case ParserAsm::ArgumentsPushSymbol: {
			uint32_t symbolId;
			if (sscanf(name_, XYO_QUANTUMSCRIPT_FORMAT_DWORD, &symbolId) != 1) {
				symbolId = 0;
			};
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ArgumentsPushSymbol, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    arguments-push-symbol %s\n", pc, name_);
#endif
			return pc;
		};

		case ParserAsm::AssignNewObject: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(AssignNewObject, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    assign-new-object\n", pc);
#endif
			return pc;
		};

		case ParserAsm::Catch: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Catch, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    catch\n", pc);
#endif
			return pc;
		};

		case ParserAsm::LocalVariablesPushObjectReference: {
			uint32_t symbolId;
			if (sscanf(name_, XYO_QUANTUMSCRIPT_FORMAT_DWORD, &symbolId) != 1) {
				symbolId = 0;
			};
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(LocalVariablesPushObjectReference, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    local-variables-push-object-reference %s\n", pc, name_);
#endif
			return pc;
		};

		case ParserAsm::LocalVariablesPushSymbol: {
			uint32_t symbolId;
			if (sscanf(name_, XYO_QUANTUMSCRIPT_FORMAT_DWORD, &symbolId) != 1) {
				symbolId = 0;
			};
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(LocalVariablesPushSymbol, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    local-variables-push-symbol %s\n", pc, name_);
#endif
			return pc;
		};

		case ParserAsm::EndExecution: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(EndExecution, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    end-execution\n", pc);
#endif
			return pc;
		};

		case ParserAsm::Yield: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(Yield, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    yield\n", pc);
#endif
			return pc;
		};

		case ParserAsm::ContextSetReference: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ContextSetReference, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-reference\n", pc);
#endif
			return pc;
		};

		case ParserAsm::OperatorSetReferenceIndexKey: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorSetReferenceIndexKey, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-set-reference-index-key\n", pc);
#endif
			return pc;
		};
		case ParserAsm::OperatorNextReferenceIndex: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorNextReferenceIndex, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-next-reference-index\n", pc);
#endif
			return pc;
		};

		case ParserAsm::OperatorSetReferenceIndexValue: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorSetReferenceIndexValue, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-set-reference-index-value\n", pc);
#endif
			return pc;
		};

		case ParserAsm::ContextSetRegisterValue: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ContextSetRegisterValue, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-register-value\n", pc);
#endif
			return pc;
		};

		case ParserAsm::ContextPushRegisterValue: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ContextPushRegisterValue, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-push-register-value\n", pc);
#endif
			return pc;
		};

		case ParserAsm::XPushNewArray: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(XPushNewArray, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    x-push-new-array\n", pc);
#endif
			return pc;
		};

		case ParserAsm::ContextPushSelf: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ContextPushSelf, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-push-self\n", pc);
#endif
			return pc;
		};
		    break;

		case ParserAsm::CurrentContextExit: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(CurrentContextExit, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    current-context-exit\n", pc);
#endif
			return pc;
		};
		    break;

		case ParserAsm::PushArguments: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushArguments, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-arguments\n", pc);
#endif
			return pc;
		};
		    break;

		case ParserAsm::OperatorReferenceDeleteIndex: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorReferenceDeleteIndex, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-reference-delete-index\n", pc);
#endif
			return pc;
		};
		    break;

		case ParserAsm::OperatorReferenceDeleteReference: {
			Symbol symbolId = Context::getSymbol(name_);
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(OperatorReferenceDeleteReference, VariableSymbol::newVariable(symbolId));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    operator-reference-delete-reference %s : %u\n", pc, name_, symbolId);
#endif
			return pc;
		};
		    break;

		case ParserAsm::ContextSetStack: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ContextSetStack, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-stack\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::ClearIncludedFile: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ClearIncludedFile, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    clear-included-file\n", pc);
#endif
			return pc;
		};
		    break;
		case ParserAsm::PushGlobal: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(PushGlobal, nullptr);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    push-global\n", pc);
#endif
			return pc;
		};
		    break;

		default:
			break;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("%p **  asm-unknown %04X:%s\n", pc, type_, name_);
#endif

		lastIsMark_ = true;
		return nullptr;
	};

	ProgramCounter *Asm::assembleX(int type_, const char *name_, const char *nameX_, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
		TDoubleEndedQueue<InstructionX>::Node *pc_;
		ProgramCounter *pc;
		if (lastIsMark_) {
			lastIsMark_ = false;
		} else {
			instructionList->pushToTail();
		};

		pc_ = instructionList->tail;
		pc = reinterpret_cast<ProgramCounter *>(pc_);

		switch (type_) {

		case ParserAsm::ArgumentsLevelPushObjectReference: {
			int value;
			int level;
			if (sscanf(name_, "%d", &value) != 1) {
				value = 0;
			};
			if (sscanf(nameX_, "%d", &level) != 1) {
				level = 0;
			};

			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ArgumentsLevelPushObjectReference, VariableArgumentLevel::newVariable(value, level));

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    arguments-level-push-object-reference %s:%s\n", pc, name_, nameX_);
#endif
			return pc;
		};
		case ParserAsm::ArgumentsLevelPushSymbol: {
			int value;
			int level;
			if (sscanf(name_, "%d", &value) != 1) {
				value = 0;
			};
			if (sscanf(nameX_, "%d", &level) != 1) {
				level = 0;
			};

			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(ArgumentsLevelPushSymbol, VariableArgumentLevel::newVariable(value, level));

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    arguments-level-push-symbol %s:%s\n", pc, name_, nameX_);
#endif
			return pc;
		};

		case ParserAsm::LocalVariablesLevelPushObjectReference: {
			int value;
			int level;
			if (sscanf(name_, "%d", &value) != 1) {
				value = 0;
			};
			if (sscanf(nameX_, "%d", &level) != 1) {
				level = 0;
			};

			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(LocalVariablesLevelPushObjectReference, VariableArgumentLevel::newVariable(value, level));

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    local-variables-level-push-object-reference %s:%s\n", pc, name_, nameX_);
#endif
			return pc;
		};
		case ParserAsm::LocalVariablesLevelPushSymbol: {
			int value;
			int level;
			if (sscanf(name_, "%d", &value) != 1) {
				value = 0;
			};
			if (sscanf(nameX_, "%d", &level) != 1) {
				level = 0;
			};

			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION(LocalVariablesLevelPushSymbol, VariableArgumentLevel::newVariable(value, level));

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    local-variables-level-push-symbol %s:%s\n", pc, name_, nameX_);
#endif
			return pc;
		};

		case ParserAsm::FunctionHint: {
			ProgramCounter *fn_;
			int hint;
			if (sscanf(name_, "%p", &fn_) != 1) {
				return nullptr;
			};
			if (sscanf(nameX_, "%d", &hint) != 1) {
				hint = 0;
			};

			((VariableVmFunction *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(fn_))->value.operand.value()))->functionHint = hint;
			lastIsMark_ = true;

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    function-hint %p:%04X\n", pc, fn_, hint);
#endif
			return pc;
		};

		default:
			break;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("%p **  asm-unknown-x %04X:%s:%s\n", pc, type_, name_, nameX_);
#endif

		lastIsMark_ = true;
		return nullptr;
	};

	ProgramCounter *Asm::assembleProgramCounter(int type_, ProgramCounter *value, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
		TDoubleEndedQueue<InstructionX>::Node *pc_;
		ProgramCounter *pc;
		if (lastIsMark_) {
			lastIsMark_ = false;
		} else {
			instructionList->pushToTail();
		};

		pc_ = instructionList->tail;
		pc = reinterpret_cast<ProgramCounter *>(pc_);

		switch (type_) {
		case ParserAsm::IfFalseGoto: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(IfFalseGoto, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    if-false-goto %p\n", pc, value);
#endif
			return optimizeCode(pc);
		};
		case ParserAsm::Goto: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(Goto, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    goto %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::ContextSetBreak: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(ContextSetBreak, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-break %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::ContextSetContinue: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(ContextSetContinue, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-continue %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::IfTrueGoto: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(IfTrueGoto, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    if-true-goto %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::XPushFunction: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(XPushFunction, VariableVmFunction::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    x-push-function %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::ContextSetCatch: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(ContextSetCatch, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-catch %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::ContextSetFinally: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(ContextSetFinally, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-finally %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::ContextSetTryBreak: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(ContextSetTryBreak, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-try-break %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::ContextSetTryContinue: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(ContextSetTryContinue, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-try-continue %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::ContextSetTryReturn: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(ContextSetTryReturn, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-try-return %p\n", pc, value);
#endif
			return pc;
		};
		    break;
		case ParserAsm::ContextSetTryThrow: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(ContextSetTryThrow, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-try-throw %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::InstructionListExtractAndDelete: {
			Variable *operator23 = VariableOperator23::newVariable();
			((VariableOperator23 *)operator23)->linkBegin = value;
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(InstructionListExtractAndDelete, operator23);
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    instruction-list-extract-and-delete %p\n", pc, value);
#endif
			return pc;
		};
		case ParserAsm::ContextSetPC: {
			XYO_QUANTUMSCRIPT_ASM_INSTRUCTION_PC(ContextSetPC, VariableVmProgramCounter::newVariable(value));
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%p    context-set-pc %p\n", pc, value);
#endif
			return pc;
		};
		default:
			break;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("%p **  asm-unknown-pc %04X:%p\n", pc, type_, value);
#endif
		lastIsMark_ = true;
		return nullptr;
	};

	void Asm::linkProgramCounter(ProgramCounter *old_, ProgramCounter *new_) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("%p -- link %p = %p\n", nullptr, old_, new_);
#endif
		asmLink_.set(old_, new_);

#ifdef XYO_QUANTUMSCRIPT_DISABLE_ASM_OPTIMIZER
#else
		if (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.asmType == ParserAsm::IfArgumentsSymbolNotEqualNumberGoto) {
			((VariableOperator31 *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.operand.value()))->pc = new_;
			return;
		};
		if (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.asmType == ParserAsm::IfSymbolNotLessNumberGoto) {
			((VariableOperator31 *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.operand.value()))->pc = new_;
			return;
		};
		if (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.asmType == ParserAsm::IfArgumentsSymbolNotLessNumberGoto) {
			((VariableOperator31 *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.operand.value()))->pc = new_;
			return;
		};
#endif

		if (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.asmType == ParserAsm::InstructionListExtractAndDelete) {
			((VariableOperator23 *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.operand.value()))->pc = new_;
			return;
		};

		if (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.operand) {

			if (TIsType<VariableVmProgramCounter>(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.operand)) {
				((VariableVmProgramCounter *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.operand.value()))->value = new_;
				return;
			};

			((VariableVmFunction *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.operand.value()))->value = new_;
		};
	};

	void Asm::linkProgramCounterEnd(ProgramCounter *old_, ProgramCounter *new_) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("%p -- link-end %p = %p\n", nullptr, old_, new_);
#endif
		((VariableVmFunction *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(old_)->value.operand.value()))->valueEnd = new_;
	};

	void Asm::linkProgramCounterSource(ProgramCounter *pc_, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("%p -- link-source %p - %u:%u\n", nullptr, pc_, sourceSymbol, sourceLineNumber);
#endif
		(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc_))->value.sourceSymbol = sourceSymbol;
		(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc_))->value.sourceLineNumber = sourceLineNumber;
	};

	void Asm::removeProgramCounter(ProgramCounter *pc_) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("%p -- remove %p\n", nullptr, pc_);
#endif
		ProgramCounter *next_;
		TDoubleEndedQueue<InstructionX>::Node *instruction = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc_);
		next_ = (ProgramCounter *)(instruction->next);
		instructionList->extractNode(instruction);
		instructionList->deleteNode(instruction);
		//
		TRedBlackTree<ProgramCounter *, ProgramCounter *>::Node *scan;
		for (scan = asmLink_.begin(); scan; scan = scan->successor()) {
			if (scan->value == pc_) {
				linkProgramCounter(scan->key, next_);
			};
		};
	};

#ifdef XYO_QUANTUMSCRIPT_DISABLE_ASM_OPTIMIZER
#else
	ProgramCounter *Asm::optimizeCode(ProgramCounter *pc) {
		TDoubleEndedQueue<InstructionX>::Node *pc_ = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc);

		//  1. arguments-push-symbol X
		//  2. push-number Y
		//  3. operator-minus
		//  - translate to -
		//  1. operator-minus-arguments-symbol-x-number X Y

		if (pc_->value.asmType == ParserAsm::OperatorMinus) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::PushNumber) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::ArgumentsPushSymbol) {
							VariableOperator21 *operatorX = TMemory<VariableOperator21>::newMemory();
							operatorX->symbol = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
							operatorX->value = ((VariableNumber *)(pc_->back->value.operand.value()))->value;
							//
							pc_->back->back->value.asmType = ParserAsm::OperatorMinusArgumentsSymbolXNumber;
							pc_->back->back->value.procedure = InstructionVmOperatorMinusArgumentsSymbolXNumber;
							pc_->back->back->value.operand = operatorX;
							pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);
							//
							instructionList->popFromTail();
							instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							printf("%p    [2] operator-minus-arguments-symbol-x-number %u %g\n", pc, operatorX->symbol, operatorX->value);
#	endif

							return pc;
						};
					};
				};
			};
		};

		// 1. local-variables-push-symbol Y
		// 2. local-variables-push-symbol X
		// 3. operator-minus
		// - translate to -
		// 1. operator-minus-local-variables-symbol-2 X Y

		if (pc_->value.asmType == ParserAsm::OperatorMinus) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
							VariableOperator22 *operatorX = TMemory<VariableOperator22>::newMemory();
							operatorX->symbol2 = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
							operatorX->symbol1 = ((VariableSymbol *)(pc_->back->value.operand.value()))->value;
							//
							pc_->back->back->value.asmType = ParserAsm::OperatorMinusLocalVariablesSymbol2;
							pc_->back->back->value.procedure = InstructionVmOperatorMinusLocalVariablesSymbol2;
							pc_->back->back->value.operand = operatorX;
							pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);
							//
							instructionList->popFromTail();
							instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							printf("%p    [2] operator-minus-local-variables-symbol-2 %u %u\n", pc, operatorX->symbol1, operatorX->symbol2);
#	endif

							return pc;
						};
					};
				};
			};
		};

		// 1. local-variables-push-symbol Y
		// 2. local-variables-push-symbol X
		// 3. operator-plus
		// - translate to -
		// 1. operator-plus-local-variables-symbol-2 X Y

		if (pc_->value.asmType == ParserAsm::OperatorPlus) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
							VariableOperator22 *operatorX = TMemory<VariableOperator22>::newMemory();
							operatorX->symbol2 = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
							operatorX->symbol1 = ((VariableSymbol *)(pc_->back->value.operand.value()))->value;
							//
							pc_->back->back->value.asmType = ParserAsm::OperatorPlusLocalVariablesSymbol2;
							pc_->back->back->value.procedure = InstructionVmOperatorPlusLocalVariablesSymbol2;
							pc_->back->back->value.operand = operatorX;
							pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);
							//
							instructionList->popFromTail();
							instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							printf("%p    [2] operator-plus-local-variables-symbol-2 %u %u\n", pc, operatorX->symbol1, operatorX->symbol2);
#	endif

							return pc;
						};
					};
				};
			};
		};

		// 1. push-number X
		// 2. push-number Y
		// 3. operator-plus
		// - translate to -
		// 1. push-number Z ; (Z=X+Y)

		if (pc_->value.asmType == ParserAsm::OperatorPlus) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::PushNumber) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::PushNumber) {
							Number v1;
							Number v2;
							v1 = pc_->back->back->value.operand->toNumber();
							v2 = pc_->back->value.operand->toNumber();
							//
							pc_->back->back->value.asmType = ParserAsm::PushNumber;
							pc_->back->back->value.procedure = InstructionVmPushNumber;
							pc_->back->back->value.operand = VariableNumber::newVariable(v1 + v2);
							pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);
							//
							instructionList->popFromTail();
							instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							printf("%p    [2] push-number " XYO_QUANTUMSCRIPT_FORMAT_NUMBER "\n", pc, (v1 + v2));
#	endif

							return pc;
						};
					};
				};
			};
		};

		//  1. push-object-reference X
		//  2. operator-plus-plus-left
		//  3. pop1
		//  - translate to -
		//  1. symbol-plus-plus X

		if (pc_->value.asmType == ParserAsm::Pop1) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::OperatorPlusPlusLeft) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::PushObjectReference) {
#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							uint32_t symbol_ = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
#	endif
							pc_->back->back->value.asmType = ParserAsm::SymbolPlusPlus;
							pc_->back->back->value.procedure = InstructionVmSymbolPlusPlus;
							pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);
							//
							instructionList->popFromTail();
							instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							printf("%p    [2] symbol-plus-plus %u\n", pc, symbol_);
#	endif

							return pc;
						};
					};
				};
			};
		};

		//  1. push-object-reference X
		//  2. operator-plus-plus-right
		//  3. pop1
		//  - translate to -
		//  1. symbol-plus-plus X

		if (pc_->value.asmType == ParserAsm::Pop1) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::OperatorPlusPlusRight) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::PushObjectReference) {
#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							uint32_t symbol_ = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
#	endif
							pc_->back->back->value.asmType = ParserAsm::SymbolPlusPlus;
							pc_->back->back->value.procedure = InstructionVmSymbolPlusPlus;
							pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);
							//
							instructionList->popFromTail();
							instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							printf("%p    [2] symbol-plus-plus %u\n", pc, symbol_);
#	endif

							return pc;
						};
					};
				};
			};
		};

		//  1. operator-assign
		//  2. pop1
		//  - translate to -
		//  1. assign

		if (pc_->value.asmType == ParserAsm::Pop1) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::OperatorAssign) {
					pc_->back->value.asmType = ParserAsm::Assign;
					pc_->back->value.procedure = InstructionVmAssign;
					pc = reinterpret_cast<ProgramCounter *>(pc_->back);
					instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
					printf("%p    [1] assign\n", pc);
#	endif

					return optimizeCode(pc);
				};
			};
		};

		// 1. local-variables-push-symbol X
		// 2. pop1
		// - translate to - // do not completely remove - branch can jump to undefined pc
		// 1. nop

		if (pc_->value.asmType == ParserAsm::Pop1) {

			if (pc_->back) {

				if (pc_->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {

					pc_->back->value.asmType = ParserAsm::Nop;
					pc_->back->value.procedure = InstructionVmNop;
					pc = reinterpret_cast<ProgramCounter *>(pc_->back);
					instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
					printf("%p    [1] nop\n", pc);
#	endif

					return pc;
				};
			};
		};

		// 1. arguments-push-symbol X
		// 2. push-number Y
		// 3. operator-equal
		// 4. if-false-goto Z
		// - translate to -
		// 1. if-arguments-symbol-not-equal-number-goto X Y Z

		if (pc_->value.asmType == ParserAsm::IfFalseGoto) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::OperatorEqual) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::PushNumber) {
							if (pc_->back->back->back) {
								if (pc_->back->back->back->value.asmType == ParserAsm::ArgumentsPushSymbol) {
									VariableOperator31 *operatorX = TMemory<VariableOperator31>::newMemory();
									operatorX->symbol = ((VariableSymbol *)(pc_->back->back->back->value.operand.value()))->value;
									operatorX->value = ((VariableNumber *)(pc_->back->back->value.operand.value()))->value;
									operatorX->pc = ((VariableVmProgramCounter *)(pc_->value.operand.value()))->value;
									//
									pc_->back->back->back->value.asmType = ParserAsm::IfArgumentsSymbolNotEqualNumberGoto;
									pc_->back->back->back->value.procedure = InstructionVmIfArgumentsSymbolNotEqualNumberGoto;
									pc_->back->back->back->value.operand = operatorX;
									pc = reinterpret_cast<ProgramCounter *>(pc_->back->back->back);
									//
									instructionList->popFromTail();
									instructionList->popFromTail();
									instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
									printf("%p    [3] if-arguments-symbol-not-equal-number-goto %u %g %p\n", pc, operatorX->symbol, operatorX->value, operatorX->pc);
#	endif

									return pc;
								};
							};
						};
					};
				};
			};
		};

		// 1. push-symbol X
		// 2. push-number Y
		// 3. operator-less
		// 4. if-false-goto Z
		// - translate to -
		// 1. if-symbol-not-less-number-goto X Y Z

		if (pc_->value.asmType == ParserAsm::IfFalseGoto) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::OperatorLess) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::PushNumber) {
							if (pc_->back->back->back) {
								if (pc_->back->back->back->value.asmType == ParserAsm::PushSymbol) {
									VariableOperator31 *operatorX = TMemory<VariableOperator31>::newMemory();
									operatorX->symbol = ((VariableSymbol *)(pc_->back->back->back->value.operand.value()))->value;
									operatorX->value = ((VariableNumber *)(pc_->back->back->value.operand.value()))->value;
									operatorX->pc = ((VariableVmProgramCounter *)(pc_->value.operand.value()))->value;
									//
									pc_->back->back->back->value.asmType = ParserAsm::IfSymbolNotLessNumberGoto;
									pc_->back->back->back->value.procedure = InstructionVmIfSymbolNotLessNumberGoto;
									pc_->back->back->back->value.operand = operatorX;
									pc = reinterpret_cast<ProgramCounter *>(pc_->back->back->back);
									//
									instructionList->popFromTail();
									instructionList->popFromTail();
									instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
									printf("%p    [3] if-symbol-not-less-number-goto %u %g %p\n", pc, operatorX->symbol, operatorX->value, operatorX->pc);
#	endif

									return pc;
								};
							};
						};
					};
				};
			};
		};

		// 1. arguments-push-symbol X
		// 2. push-number Y
		// 3. operator-less
		// 4. if-false-goto Z
		// - translate to -
		// 1. if-arguments-symbol-not-less-number-goto X Y Z

		if (pc_->value.asmType == ParserAsm::IfFalseGoto) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::OperatorLess) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::PushNumber) {
							if (pc_->back->back->back) {
								if (pc_->back->back->back->value.asmType == ParserAsm::ArgumentsPushSymbol) {
									VariableOperator31 *operatorX = TMemory<VariableOperator31>::newMemory();
									operatorX->symbol = ((VariableSymbol *)(pc_->back->back->back->value.operand.value()))->value;
									operatorX->value = ((VariableNumber *)(pc_->back->back->value.operand.value()))->value;
									operatorX->pc = ((VariableVmProgramCounter *)(pc_->value.operand.value()))->value;
									//
									pc_->back->back->back->value.asmType = ParserAsm::IfArgumentsSymbolNotLessNumberGoto;
									pc_->back->back->back->value.procedure = InstructionVmIfArgumentsSymbolNotLessNumberGoto;
									pc_->back->back->back->value.operand = operatorX;
									pc = reinterpret_cast<ProgramCounter *>(pc_->back->back->back);
									//
									instructionList->popFromTail();
									instructionList->popFromTail();
									instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
									printf("%p    [3] if-arguments-symbol-not-less-number-goto %u %g %p\n", pc, operatorX->symbol, operatorX->value, operatorX->pc);
#	endif

									return pc;
								};
							};
						};
					};
				};
			};
		};

		// 1. local-variables-push-symbol Y
		// 2. local-variables-push-symbol X
		// 3. operator-mul
		// - translate to -
		// X!=Y > operator-mul-local-variables-symbol-2 X Y
		// X==Y > operator-pow2-local-variables-symbol X

		if (pc_->value.asmType == ParserAsm::OperatorMul) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {

							// X==Y > operator-pow2-local-variables-symbol X
							if (((VariableSymbol *)(pc_->back->back->value.operand.value()))->value ==
							    ((VariableSymbol *)(pc_->back->value.operand.value()))->value) {
								//
								pc_->back->back->value.asmType = ParserAsm::OperatorPow2LocalVariablesSymbol;
								pc_->back->back->value.procedure = InstructionVmOperatorPow2LocalVariablesSymbol;
								pc_->back->back->value.operand = pc_->back->value.operand;
								pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);
								//
								instructionList->popFromTail();
								instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
								printf("%p    [2] operator-pow2-local-variables-symbol-2 %u\n", pc, ((VariableSymbol *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->value);
#	endif
								return pc;
							};

							// X!=Y > operator-mul-local-variables-symbol-2 X Y
							VariableOperator22 *operatorX = TMemory<VariableOperator22>::newMemory();
							operatorX->symbol2 = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
							operatorX->symbol1 = ((VariableSymbol *)(pc_->back->value.operand.value()))->value;
							//
							pc_->back->back->value.asmType = ParserAsm::OperatorMulLocalVariablesSymbol2;
							pc_->back->back->value.procedure = InstructionVmOperatorMulLocalVariablesSymbol2;
							pc_->back->back->value.operand = operatorX;
							pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);
							//
							instructionList->popFromTail();
							instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							printf("%p    [2] operator-mul-local-variables-symbol-2 %u %u\n", pc, operatorX->symbol1, operatorX->symbol2);
#	endif
							return pc;
						};
					};
				};
			};
		};

		// 1. operator-greater
		// 2. if-false-goto X
		// - translate to -
		// 1. if-not-greater X

		if (pc_->value.asmType == ParserAsm::IfFalseGoto) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::OperatorGreater) {

					pc_->back->value.asmType = ParserAsm::IfNotGreaterGoto;
					pc_->back->value.procedure = InstructionVmIfNotGreaterGoto;
					pc_->back->value.operand = pc_->value.operand;
					pc_->value.operand = nullptr;
					pc = reinterpret_cast<ProgramCounter *>(pc_->back);
					instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
					printf("%p    [1] if-not-greater %p\n", pc, ((VariableVmProgramCounter *)(((TDoubleEndedQueue<InstructionX>::Node *)pc)->value.operand.value()))->value);
#	endif

					return pc;
				};
			};
		};

		// 1. x-call
		// 2. return
		// - translate to -
		// 1. x-tail-call

		if (pc_->value.asmType == ParserAsm::Return) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::XCall) {

					pc_->back->value.asmType = ParserAsm::XTailCall;
					pc_->back->value.procedure = InstructionVmXTailCall;
					pc = reinterpret_cast<ProgramCounter *>(pc_->back);
					instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
					printf("%p    [1] x-tail-call\n", pc);
#	endif

					return pc;
				};
			};
		};

		// 1. x-call-this
		// 2. return
		// - translate to -
		// 1. x-tail-call-this

		if (pc_->value.asmType == ParserAsm::Return) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::XCallThis) {

					pc_->back->value.asmType = ParserAsm::XTailCallThis;
					pc_->back->value.procedure = InstructionVmXTailCallThis;
					pc = reinterpret_cast<ProgramCounter *>(pc_->back);
					instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
					printf("%p    [1] x-tail-call-this\n", pc);
#	endif

					return pc;
				};
			};
		};

		// 1. x-call-this-mode-call
		// 2. return
		// - translate to -
		// 1. x-tail-call-this-mode-call

		if (pc_->value.asmType == ParserAsm::Return) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::XCallThisModeCall) {

					pc_->back->value.asmType = ParserAsm::XTailCallThisModeCall;
					pc_->back->value.procedure = InstructionVmXTailCallThisModeCall;
					pc = reinterpret_cast<ProgramCounter *>(pc_->back);
					instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
					printf("%p    [1] x-tail-call-this-mode-call\n", pc);
#	endif

					return pc;
				};
			};
		};

		// 1. x-call-this-mode-apply
		// 2. return
		// - translate to -
		// 1. x-tail-call-this-mode-apply

		if (pc_->value.asmType == ParserAsm::Return) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::XCallThisModeApply) {

					pc_->back->value.asmType = ParserAsm::XTailCallThisModeApply;
					pc_->back->value.procedure = InstructionVmXTailCallThisModeApply;
					pc = reinterpret_cast<ProgramCounter *>(pc_->back);
					instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
					printf("%p    [1] x-tail-call-this-mode-apply\n", pc);
#	endif

					return pc;
				};
			};
		};

		// 1. x-call-with-this-reference X
		// 2. return
		// - translate to -
		// 1. x-call-with-this-reference X

		if (pc_->value.asmType == ParserAsm::Return) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::XCallWithThisReference) {

					pc_->back->value.asmType = ParserAsm::XTailCallWithThisReference;
					pc_->back->value.procedure = InstructionVmXTailCallWithThisReference;
					pc = reinterpret_cast<ProgramCounter *>(pc_->back);
					instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
					printf("%p    [1] x-tail-call-with-this-reference %u\n", pc, ((VariableSymbol *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->value);
#	endif

					return pc;
				};
			};
		};

		// 1. x-call-symbol X
		// 2. return
		// - translate to -
		// 1. x-tail-call-symbol X

		if (pc_->value.asmType == ParserAsm::Return) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::XCallSymbol) {

					pc_->back->value.asmType = ParserAsm::XTailCallSymbol;
					pc_->back->value.procedure = InstructionVmXTailCallSymbol;
					pc = reinterpret_cast<ProgramCounter *>(pc_->back);
					instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
					printf("%p    [1] x-tail-call-symbol %u\n", pc, ((VariableSymbol *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->value);
#	endif

					return pc;
				};
			};
		};

		// 1. local-variables-object-reference X
		// 2. operator-pow2-local-variables-symbol Y
		// 3. assign
		// - translate to -
		// 1. assign-local-variables-object-reference-pow2-local-variables-symbol X Y

		if (pc_->value.asmType == ParserAsm::Assign) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::OperatorPow2LocalVariablesSymbol) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushObjectReference) {

							VariableOperator22 *operatorX = TMemory<VariableOperator22>::newMemory();
							operatorX->symbol1 = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
							operatorX->symbol2 = ((VariableSymbol *)(pc_->back->value.operand.value()))->value;

							pc_->back->back->value.asmType = ParserAsm::AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol;
							pc_->back->back->value.procedure = InstructionVmAssignLocalVariablesObjectReferencePow2LocalVariablesSymbol;
							pc_->back->back->value.operand = operatorX;
							pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);

							instructionList->popFromTail();
							instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							printf("%p    [2] assign-local-variables-object-reference-pow2-local-variables-symbol %u %u\n", pc,
							       ((VariableOperator22 *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->symbol1,
							       ((VariableOperator22 *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->symbol2);
#	endif

							return pc;
						};
					};
				};
			};
		};

		//  1. local-variables-object-reference X
		//  2. operator-plus-plus-left
		//  3. pop1
		//  - translate to -
		//  1. local-variables-plus-plus X

		if (pc_->value.asmType == ParserAsm::Pop1) {

			if (pc_->back) {
				if (pc_->back->value.asmType == ParserAsm::OperatorPlusPlusLeft) {
					if (pc_->back->back) {
						if (pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushObjectReference) {
#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							uint32_t symbol_ = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
#	endif
							pc_->back->back->value.asmType = ParserAsm::LocalVariablesPlusPlus;
							pc_->back->back->value.procedure = InstructionVmLocalVariablesPlusPlus;
							pc = reinterpret_cast<ProgramCounter *>(pc_->back->back);
							//
							instructionList->popFromTail();
							instructionList->popFromTail();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
							printf("%p    [2] local-variables-plus-plus %u\n", pc, symbol_);
#	endif

							return pc;
						};
					};
				};
			};
		};

		// ---

		return pc;
	};

#endif

};
