//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "quantum-script-asm.hpp"
#include "quantum-script-parserasm.hpp"
#include "quantum-script-programcounter.hpp"
#include "quantum-script-instructionx.hpp"

#include "quantum-script-variablesymbol.hpp"
#include "quantum-script-variableboolean.hpp"
#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

#include "quantum-script-variableargumentlevel.hpp"
#include "quantum-script-variablevmprogramcounter.hpp"
#include "quantum-script-variablevmfunction.hpp"
#include "quantum-script-variableoperator21.hpp"
#include "quantum-script-variableoperator22.hpp"
#include "quantum-script-variableoperator23.hpp"
#include "quantum-script-variableoperator31.hpp"


//#define QUANTUM_SCRIPT_DEBUG_ASM 1

#define QUANTUM_SCRIPT_ASM_INSTRUCTION(Type,Operand) \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.asmType=ParserAsm::Type;\
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.procedure=InstructionVm##Type;\
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.operand=Operand; \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.sourceSymbol=sourceSymbol; \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.sourceLineNumber=sourceLineNumber;

#define QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(Type,Operand) \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.asmType=ParserAsm::Type;\
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.procedure=InstructionVm##Type;\
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.operand=Operand; \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.sourceSymbol=sourceSymbol; \
	reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc)->value.sourceLineNumber=sourceLineNumber;


namespace XYO {
	namespace DataStructures {

		template<>
		class TComparator<Quantum::Script::ProgramCounter *> {
			public:
				typedef Quantum::Script::ProgramCounter *T;

				inline static bool isLess(const T &a, const T &b) {
					return (compare(a, b) < 0);
				};

				inline static int compare(const T &a, const T &b) {
					return ((uint8_t *)(const_cast<T>(a))) - ((uint8_t *)const_cast<T>(b));
				};
		};

	};
};


namespace Quantum {
	namespace Script {

		using namespace XYO;


		Asm::Asm() {
			lastIsMark_ = false;
			asmLink_.empty();
		};

		void Asm::resetLinks() {
			asmLink_.empty();
		};

		void Asm::endProcessing() {
			if(instructionList) {
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
			pc = reinterpret_cast<ProgramCounter *> (pc_);


			reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (pc)->value.procedure = procedure;
			reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (pc)->value.operand = operand;
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
			pc = reinterpret_cast<ProgramCounter *> (pc_);

			switch (type_) {
				case ParserAsm::PushUndefined: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushUndefined, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-undefined\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushSymbol: {
						Symbol symbolId = Context::getSymbol(name_);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushSymbol, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-symbol %s : %d\n", pc, name_, symbolId);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushBoolean: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushBoolean, VariableBoolean::newVariable(StringCore::isEqual(name_, "true")));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-boolean %s\n", pc, name_);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushNumber: {
						Number valueNumber;
						sscanf(name_, QUANTUM_SCRIPT_FORMAT_NUMBER_INPUT, &valueNumber);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushNumber, VariableNumber::newVariable(valueNumber));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-number %s\n", pc, name_);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushNaN: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushNumber, VariableNumber::newVariable(NAN));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-nan\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushInfinity: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushNumber, VariableNumber::newVariable(INFINITY));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-infinity\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssign: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssign, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignPlus: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignPlus, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-plus\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignMinus: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignMinus, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-minus\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignMul: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignMul, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-mul\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignDiv: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignDiv, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-div\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignMod: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignMod, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-mod\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignBitwiseOr: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseOr, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-bitwise-or\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignBitwiseAnd: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseAnd, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-bitwise-and\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignBitwiseLeftShift: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseLeftShift, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-bitwise-left-shift\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignBitwiseRightShift: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseRightShift, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-bitwise-right-shift\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignBitwiseRightShiftZero: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseRightShiftZero, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-bitwise-right-shift-zero\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorAssignBitwiseXor: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignBitwiseXor, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-bitwise-xor\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorEqual: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorEqual, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-equal\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorEqualStrict: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorEqualStrict, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-equal-strict\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorPlus: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorPlus, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-plus\n", pc);
#endif
						return optimizeCode(pc);
					};
					break;
				case ParserAsm::OperatorMinus: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorMinus, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-minus\n", pc);
#endif
						return optimizeCode(pc);
					};
					break;
				case ParserAsm::OperatorUnaryPlus: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorUnaryPlus, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-unary-plus\n", pc);
#endif
						return optimizeCode(pc);
					};
					break;
				case ParserAsm::OperatorMul: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorMul, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-mul\n", pc);
#endif
						return optimizeCode(pc);
					};
					break;
				case ParserAsm::OperatorDiv: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorDiv, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-div\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorMod: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorMod, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-mod\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorNotEqual: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorNotEqual, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-not-equal\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorNotEqualStrict: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorNotEqualStrict, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-not-equal-strict\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorNot: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorNot, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-not\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorLessOrEqual: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorLessOrEqual, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-less-or-equal\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorLess: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorLess, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-less\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorGreaterOrEqual: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorGreaterOrEqual, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-greater-or-equal\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorGreater: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorGreater, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-greater\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorLogicalOr: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorLogicalOr, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-logical-or\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorBitwiseOr: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorBitwiseOr, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-bitwise-or\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorLogicalAnd: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorLogicalAnd, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-logical-and\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorBitwiseAnd: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorBitwiseAnd, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-bitwise-and\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorBitwiseNot: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorBitwiseNot, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-bitwise-not\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorBitwiseLeftShift: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorBitwiseLeftShift, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-bitwise-left-shift\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorBitwiseRightShift: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorBitwiseRightShift, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-bitwise-right-shift\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorBitwiseRightShiftZero: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorBitwiseRightShiftZero, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-bitwise-right-shift-zero\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorBitwiseXor: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorBitwiseXor, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-bitwise-xor\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorIn: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorIn, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-in\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::Nop: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Nop, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    nop\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::Reference: {
						Symbol symbolId = Context::getSymbol(name_);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Reference, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    reference %s : %d\n", pc, name_, symbolId);
#endif
						return pc;
					};
					break;
				case ParserAsm::EnterContext: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(EnterContext, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    enter-context\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::LeaveContext: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(LeaveContext, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    leave-context\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::Mark: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Mark, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    mark\n", pc);
#endif
						lastIsMark_ = true;
						return pc;
					};
					break;
				case ParserAsm::Break: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Break, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    break\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::Continue: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Continue, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    continue\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorEqual1: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorEqual1, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-equal1\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::Pop1: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Pop1, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    pop1\n", pc);
#endif

						return optimizeCode(pc);
					};
					break;
				case ParserAsm::PushNull: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushNull, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-null\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::Return: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Return, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    return\n", pc);
#endif
						return optimizeCode(pc);
					};
					break;
				case ParserAsm::Throw: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Throw, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    throw\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::ContextSetThis: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ContextSetThis, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-this\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::ContextPushThis: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ContextPushThis, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-push-this\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushNewObject: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushNewObject, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-new-object\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushString: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushString, VariableString::newVariable(name_));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-string %s\n", pc, name_);
#endif
						return pc;
					};
					break;
				case ParserAsm::EnterFirstContext: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(EnterFirstContext, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    enter-first-context\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::Assign: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Assign, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    assign\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorReference: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorReference, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-reference\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushNewArray: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushNewArray, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-new-array\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::ArrayPush: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ArrayPush, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    array-push\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorArrayPush: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorArrayPush, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-array-push\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushObjectReference: {
						Symbol symbolId = Context::getSymbol(name_);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushObjectReference, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-object-reference %s : %d\n", pc, name_, symbolId);
#endif
						return pc;
					};
					break;
				case ParserAsm::ReferenceObjectReference: {
						Symbol symbolId = Context::getSymbol(name_);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ReferenceObjectReference, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    reference-object-reference %s : %d\n", pc, name_, symbolId);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorInstanceOf: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorInstanceOf, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-instance-of\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::OperatorTypeOf: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorTypeOf, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-type-of\n", pc);
#endif
						return pc;
					};

				case ParserAsm::OperatorReferenceReference: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorReferenceReference, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-reference-reference\n", pc);
#endif
						return pc;
					};
				case ParserAsm::OperatorArrayPushReference: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorArrayPushReference, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-array-push-reference\n", pc);
#endif
						return pc;
					};
				case ParserAsm::OperatorObjectReferenceValue: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorObjectReferenceValue, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-object-reference-value\n", pc);
#endif
						return pc;
					};
				case ParserAsm::OperatorPlusPlusLeft: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorPlusPlusLeft, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-plus-plus-left\n", pc);
#endif
						return pc;
					};
				case ParserAsm::OperatorPlusPlusRight: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorPlusPlusRight, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-plus-plus-right\n", pc);
#endif
						return pc;
					};
				case ParserAsm::OperatorMinusMinusLeft: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorMinusMinusLeft, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-minus-minus-left\n", pc);
#endif
						return pc;
					};
				case ParserAsm::OperatorMinusMinusRight: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorMinusMinusRight, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-minus-minus-right\n", pc);
#endif
						return pc;
					};

				case ParserAsm::XCall: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(XCall, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    x-call\n", pc);
#endif
						return pc;
					};
				case ParserAsm::XCallThis: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(XCallThis, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    x-call-this\n", pc);
#endif
						return pc;
					};
				case ParserAsm::XCallThisModeCall: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(XCallThisModeCall, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    x-call-this-mode-call\n", pc);
#endif
						return pc;
					};
				case ParserAsm::XCallThisModeApply: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(XCallThisModeApply, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    x-call-this-mode-apply\n", pc);
#endif
						return pc;
					};
				case ParserAsm::XCallWithThisReference: {
						Symbol symbolId = Context::getSymbol(name_);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(XCallWithThisReference, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    x-call-with-this-reference %s : %d\n", pc, name_, symbolId);
#endif
						return pc;
					};
				case ParserAsm::XCallSymbol: {
						Symbol symbolId = Context::getSymbol(name_);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(XCallSymbol, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    x-call-symbol %s : %d\n", pc, name_, symbolId);
#endif
						return pc;
					};
				case ParserAsm::XArrayPushWithTransfer: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(XArrayPushWithTransfer, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    x-array-push-with-transfer\n", pc);
#endif
						return pc;
					};
				case ParserAsm::AssignReverse: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(AssignReverse, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    assign-reverse\n", pc);
#endif
						return pc;
					};
				case ParserAsm::Duplicate: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Duplicate, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    duplicate\n", pc);
#endif
						return pc;
					};
				case ParserAsm::OperatorAssignXPrototype: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorAssignXPrototype, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-assign-x-prototype\n", pc);
#endif
						return pc;
					};

				case ParserAsm::ArgumentsPushObjectReference: {
						uint32_t symbolId;
						sscanf(name_, QUANTUM_SCRIPT_FORMAT_DWORD, &symbolId);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ArgumentsPushObjectReference, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    arguments-push-object-reference %s\n", pc, name_);
#endif
						return pc;
					};

				case ParserAsm::ArgumentsPushSymbol: {
						uint32_t symbolId;
						sscanf( name_, QUANTUM_SCRIPT_FORMAT_DWORD, &symbolId);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ArgumentsPushSymbol, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    arguments-push-symbol %s\n", pc, name_);
#endif
						return pc;
					};

				case ParserAsm::AssignNewObject: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(AssignNewObject, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    assign-new-object\n", pc);
#endif
						return pc;
					};

				case ParserAsm::Catch: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Catch, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    catch\n", pc);
#endif
						return pc;
					};

				case ParserAsm::LocalVariablesPushObjectReference: {
						uint32_t symbolId;
						sscanf( name_, QUANTUM_SCRIPT_FORMAT_DWORD, &symbolId);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(LocalVariablesPushObjectReference, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    local-variables-push-object-reference %s\n", pc, name_);
#endif
						return pc;
					};

				case ParserAsm::LocalVariablesPushSymbol: {
						uint32_t symbolId;
						sscanf( name_, QUANTUM_SCRIPT_FORMAT_DWORD, &symbolId);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(LocalVariablesPushSymbol, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    local-variables-push-symbol %s\n", pc, name_);
#endif
						return pc;
					};


				case ParserAsm::EndExecution: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(EndExecution, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    end-execution\n", pc);
#endif
						return pc;
					};

				case ParserAsm::Yield: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(Yield, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    yield\n", pc);
#endif
						return pc;
					};

				case ParserAsm::ContextSetReference: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ContextSetReference, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-reference\n", pc);
#endif
						return pc;
					};

				case ParserAsm::OperatorSetReferenceIndexKey: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorSetReferenceIndexKey, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-set-reference-index-key\n", pc);
#endif
						return pc;
					};
				case ParserAsm::OperatorNextReferenceIndex: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorNextReferenceIndex, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-next-reference-index\n", pc);
#endif
						return pc;
					};

				case ParserAsm::OperatorSetReferenceIndexValue: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorSetReferenceIndexValue, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-set-reference-index-value\n", pc);
#endif
						return pc;
					};

				case ParserAsm::ContextSetRegisterValue: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ContextSetRegisterValue, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-register-value\n", pc);
#endif
						return pc;
					};



				case ParserAsm::ContextPushRegisterValue: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ContextPushRegisterValue, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-push-register-value\n", pc);
#endif
						return pc;
					};

				case ParserAsm::XPushNewArray: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(XPushNewArray, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    x-push-new-array\n", pc);
#endif
						return pc;
					};

				case ParserAsm::ContextPushSelf: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ContextPushSelf, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-push-self\n", pc);
#endif
						return pc;
					};
					break;

				case ParserAsm::CurrentContextExit: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(CurrentContextExit, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    current-context-exit\n", pc);
#endif
						return pc;
					};
					break;

				case ParserAsm::PushArguments: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushArguments, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-arguments\n", pc);
#endif
						return pc;
					};
					break;

				case ParserAsm::OperatorReferenceDeleteIndex: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorReferenceDeleteIndex, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-reference-delete-index\n", pc);
#endif
						return pc;
					};
					break;

				case ParserAsm::OperatorReferenceDeleteReference: {
						Symbol symbolId = Context::getSymbol(name_);
						QUANTUM_SCRIPT_ASM_INSTRUCTION(OperatorReferenceDeleteReference, VariableSymbol::newVariable(symbolId));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    operator-reference-delete-reference %s : %d\n", pc, name_, symbolId);
#endif
						return pc;
					};
					break;

				case ParserAsm::ContextSetStack: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ContextSetStack, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-stack\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::ClearIncludedFile: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(ClearIncludedFile, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    clear-included-file\n", pc);
#endif
						return pc;
					};
					break;
				case ParserAsm::PushGlobal: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION(PushGlobal, nullptr);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    push-global\n", pc);
#endif
						return pc;
					};
					break;

				default:
					break;
			};

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
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
			pc = reinterpret_cast<ProgramCounter *> (pc_);

			switch (type_) {

				case ParserAsm::ArgumentsLevelPushObjectReference: {
						int value;
						int level;
						sscanf( name_, "%d", &value);
						sscanf( nameX_, "%d", &level);

						QUANTUM_SCRIPT_ASM_INSTRUCTION(ArgumentsLevelPushObjectReference, VariableArgumentLevel::newVariable(value, level));

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    arguments-level-push-object-reference %s:%s\n", pc, name_, nameX_);
#endif
						return pc;
					};
				case ParserAsm::ArgumentsLevelPushSymbol: {
						int value;
						int level;
						sscanf( name_, "%d", &value);
						sscanf( nameX_, "%d", &level);

						QUANTUM_SCRIPT_ASM_INSTRUCTION(ArgumentsLevelPushSymbol, VariableArgumentLevel::newVariable(value, level));

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    arguments-level-push-symbol %s:%s\n", pc, name_, nameX_);
#endif
						return pc;
					};

				case ParserAsm::LocalVariablesLevelPushObjectReference: {
						int value;
						int level;
						sscanf( name_, "%d", &value);
						sscanf( nameX_, "%d", &level);

						QUANTUM_SCRIPT_ASM_INSTRUCTION(LocalVariablesLevelPushObjectReference, VariableArgumentLevel::newVariable(value, level));

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    local-variables-level-push-object-reference %s:%s\n", pc, name_, nameX_);
#endif
						return pc;
					};
				case ParserAsm::LocalVariablesLevelPushSymbol: {
						int value;
						int level;
						sscanf( name_, "%d", &value);
						sscanf( nameX_, "%d", &level);

						QUANTUM_SCRIPT_ASM_INSTRUCTION(LocalVariablesLevelPushSymbol, VariableArgumentLevel::newVariable(value, level));

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    local-variables-level-push-symbol %s:%s\n", pc, name_, nameX_);
#endif
						return pc;
					};


				case ParserAsm::FunctionHint: {
						ProgramCounter *fn_;
						int hint;
						sscanf(name_, "%p", &fn_);
						sscanf(nameX_, "%d", &hint);

						((VariableVmFunction *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (fn_))->value.operand.value()))->functionHint = hint;
						lastIsMark_ = true;

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    function-hint %p:%04X\n", pc, fn_, hint);
#endif
						return pc;
					};




				default:
					break;
			};


#ifdef QUANTUM_SCRIPT_DEBUG_ASM
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
			pc = reinterpret_cast<ProgramCounter *> (pc_);

			switch (type_) {
				case ParserAsm::IfFalseGoto: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(IfFalseGoto, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    if-false-goto %p\n", pc, value);
#endif
						return optimizeCode(pc);
					};
				case ParserAsm::Goto: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(Goto, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    goto %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::ContextSetBreak: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(ContextSetBreak, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-break %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::ContextSetContinue: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(ContextSetContinue, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-continue %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::IfTrueGoto: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(IfTrueGoto, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    if-true-goto %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::XPushFunction: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(XPushFunction, VariableVmFunction::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    x-push-function %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::ContextSetCatch: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(ContextSetCatch, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-catch %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::ContextSetFinally: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(ContextSetFinally, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-finally %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::ContextSetTryBreak: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(ContextSetTryBreak, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-try-break %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::ContextSetTryContinue: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(ContextSetTryContinue, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-try-continue %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::ContextSetTryReturn: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(ContextSetTryReturn, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-try-return %p\n", pc, value);
#endif
						return pc;
					};
					break;
				case ParserAsm::ContextSetTryThrow: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(ContextSetTryThrow, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-try-throw %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::InstructionListExtractAndDelete: {
						Variable *operator23 = VariableOperator23::newVariable();
						((VariableOperator23 *)operator23)->linkBegin = value;
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(InstructionListExtractAndDelete, operator23);
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    instruction-list-extract-and-delete %p\n", pc, value);
#endif
						return pc;
					};
				case ParserAsm::ContextSetPC: {
						QUANTUM_SCRIPT_ASM_INSTRUCTION_PC(ContextSetPC, VariableVmProgramCounter::newVariable(value));
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    context-set-pc %p\n", pc, value);
#endif
						return pc;
					};
				default:
					break;
			};

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
			printf("%p **  asm-unknown-pc %04X:%p\n", pc, type_, value);
#endif
			lastIsMark_ = true;
			return nullptr;
		};

		void Asm::linkProgramCounter(ProgramCounter *old_, ProgramCounter *new_) {
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
			printf("%p -- link %p = %p\n", nullptr, old_, new_);
#endif
			asmLink_.set(old_, new_);

#ifdef QUANTUM_SCRIPT_DISABLE_ASM_OPTIMIZER
#else
			if(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.asmType == ParserAsm::IfArgumentsSymbolNotEqualNumberGoto) {
				((VariableOperator31 *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.operand.value()))->pc = new_;
				return;
			};
			if(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.asmType == ParserAsm::IfSymbolNotLessNumberGoto) {
				((VariableOperator31 *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.operand.value()))->pc = new_;
				return;
			};
			if(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.asmType == ParserAsm::IfArgumentsSymbolNotLessNumberGoto) {
				((VariableOperator31 *)(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.operand.value()))->pc = new_;
				return;
			};
#endif

			if(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.asmType == ParserAsm::InstructionListExtractAndDelete) {
				((VariableOperator23 *) (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.operand.value()))->pc = new_;
				return;
			};

			if(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.operand) {

				if(VariableVmProgramCounter::isVariableVmProgramCounter(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.operand)) {
					((VariableVmProgramCounter *) (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.operand.value()))->value = new_;
					return;
				};

				((VariableVmFunction *) (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.operand.value()))->value = new_;
			};
		};


		void Asm::linkProgramCounterEnd(ProgramCounter *old_, ProgramCounter *new_) {
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
			printf("%p -- link-end %p = %p\n", nullptr, old_, new_);
#endif
			((VariableVmFunction *) (reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (old_)->value.operand.value()))->valueEnd = new_;
		};

		void Asm::linkProgramCounterSource(ProgramCounter *pc_, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
			printf("%p -- link-source %p - %d:%d\n", nullptr, pc_, sourceSymbol, sourceLineNumber);
#endif
			(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (pc_))->value.sourceSymbol = sourceSymbol;
			(reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (pc_))->value.sourceLineNumber = sourceLineNumber;
		};


		void Asm::removeProgramCounter(ProgramCounter *pc_) {
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
			printf("%p -- remove %p\n", nullptr, pc_);
#endif
			ProgramCounter *next_;
			TDoubleEndedQueue<InstructionX>::Node *instruction = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *> (pc_);
			next_ = (ProgramCounter *)(instruction->next);
			instructionList->extractNode(instruction);
			instructionList->deleteNode(instruction);
			//
			TRedBlackTree<ProgramCounter *, ProgramCounter *>::Node *scan;
			for(scan = asmLink_.begin(); scan; scan = scan->successor()) {
				if(scan->value == pc_) {
					linkProgramCounter(scan->key, next_);
				};
			};
		};

#ifdef QUANTUM_SCRIPT_DISABLE_ASM_OPTIMIZER
#else
		ProgramCounter *Asm::optimizeCode(ProgramCounter *pc) {
			TDoubleEndedQueue<InstructionX>::Node *pc_ = reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc);

			//  1. arguments-push-symbol X
			//  2. push-number Y
			//  3. operator-minus
			//  - translate to -
			//  1. operator-minus-arguments-symbol-x-number X Y

			if(pc_->value.asmType == ParserAsm::OperatorMinus) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::PushNumber) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::ArgumentsPushSymbol) {
								VariableOperator21 *operatorX = TMemory<VariableOperator21>::newMemory();
								operatorX->symbol = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
								operatorX->value = ((VariableNumber *)(pc_->back->value.operand.value()))->value;
								//
								pc_->back->back->value.asmType = ParserAsm::OperatorMinusArgumentsSymbolXNumber;
								pc_->back->back->value.procedure = InstructionVmOperatorMinusArgumentsSymbolXNumber;
								pc_->back->back->value.operand = operatorX;
								pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);
								//
								instructionList->popFromTail();
								instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								printf("%p    [2] operator-minus-arguments-symbol-x-number %d %g\n", pc, operatorX->symbol, operatorX->value);
#endif

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

			if(pc_->value.asmType == ParserAsm::OperatorMinus) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
								VariableOperator22 *operatorX = TMemory<VariableOperator22>::newMemory();
								operatorX->symbol2 = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
								operatorX->symbol1 = ((VariableSymbol *)(pc_->back->value.operand.value()))->value;
								//
								pc_->back->back->value.asmType = ParserAsm::OperatorMinusLocalVariablesSymbol2;
								pc_->back->back->value.procedure = InstructionVmOperatorMinusLocalVariablesSymbol2;
								pc_->back->back->value.operand = operatorX;
								pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);
								//
								instructionList->popFromTail();
								instructionList->popFromTail();


#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								printf("%p    [2] operator-minus-local-variables-symbol-2 %d %d\n", pc, operatorX->symbol1, operatorX->symbol2);
#endif

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

			if(pc_->value.asmType == ParserAsm::OperatorPlus) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
								VariableOperator22 *operatorX = TMemory<VariableOperator22>::newMemory();
								operatorX->symbol2 = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
								operatorX->symbol1 = ((VariableSymbol *)(pc_->back->value.operand.value()))->value;
								//
								pc_->back->back->value.asmType = ParserAsm::OperatorPlusLocalVariablesSymbol2;
								pc_->back->back->value.procedure = InstructionVmOperatorPlusLocalVariablesSymbol2;
								pc_->back->back->value.operand = operatorX;
								pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);
								//
								instructionList->popFromTail();
								instructionList->popFromTail();


#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								printf("%p    [2] operator-plus-local-variables-symbol-2 %d %d\n", pc, operatorX->symbol1, operatorX->symbol2);
#endif

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

			if(pc_->value.asmType == ParserAsm::OperatorPlus) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::PushNumber) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::PushNumber) {
								Number v1;
								Number v2;
								v1 = pc_->back->back->value.operand->toNumber();
								v2 = pc_->back->value.operand->toNumber();
								//
								pc_->back->back->value.asmType = ParserAsm::PushNumber;
								pc_->back->back->value.procedure = InstructionVmPushNumber;
								pc_->back->back->value.operand = VariableNumber::newVariable(v1 + v2);
								pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);
								//
								instructionList->popFromTail();
								instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								printf("%p    [2] push-number " QUANTUM_SCRIPT_FORMAT_NUMBER "\n", pc, (v1 + v2));
#endif

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

			if(pc_->value.asmType == ParserAsm::Pop1) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::OperatorPlusPlusLeft) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::PushObjectReference) {
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								uint32_t symbol_ = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
#endif
								pc_->back->back->value.asmType = ParserAsm::SymbolPlusPlus;
								pc_->back->back->value.procedure = InstructionVmSymbolPlusPlus;
								pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);
								//
								instructionList->popFromTail();
								instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								printf("%p    [2] symbol-plus-plus %d\n", pc, symbol_);
#endif

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

			if(pc_->value.asmType == ParserAsm::Pop1) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::OperatorPlusPlusRight) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::PushObjectReference) {
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								uint32_t symbol_ = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
#endif
								pc_->back->back->value.asmType = ParserAsm::SymbolPlusPlus;
								pc_->back->back->value.procedure = InstructionVmSymbolPlusPlus;
								pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);
								//
								instructionList->popFromTail();
								instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								printf("%p    [2] symbol-plus-plus %d\n", pc, symbol_);
#endif

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

			if(pc_->value.asmType == ParserAsm::Pop1) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::OperatorAssign) {
						pc_->back->value.asmType = ParserAsm::Assign;
						pc_->back->value.procedure = InstructionVmAssign;
						pc = reinterpret_cast<ProgramCounter *> (pc_->back);
						instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    [1] assign\n", pc);
#endif

						return optimizeCode(pc);
					};
				};

			};


			// 1. local-variables-push-symbol X
			// 2. pop1
			// - translate to - // do not completely remove - branch can jump to undefined pc
			// 1. nop

			if(pc_->value.asmType == ParserAsm::Pop1) {

				if(pc_->back) {

					if(pc_->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {

						pc_->back->value.asmType = ParserAsm::Nop;
						pc_->back->value.procedure = InstructionVmNop;
						pc = reinterpret_cast<ProgramCounter *> (pc_->back);
						instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    [1] nop\n", pc);
#endif

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

			if(pc_->value.asmType == ParserAsm::IfFalseGoto) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::OperatorEqual) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::PushNumber) {
								if(pc_->back->back->back) {
									if(pc_->back->back->back->value.asmType == ParserAsm::ArgumentsPushSymbol) {
										VariableOperator31 *operatorX = TMemory<VariableOperator31>::newMemory();
										operatorX->symbol = ((VariableSymbol *)(pc_->back->back->back->value.operand.value()))->value;
										operatorX->value = ((VariableNumber *)(pc_->back->back->value.operand.value()))->value;
										operatorX->pc = ((VariableVmProgramCounter *)(pc_->value.operand.value()))->value;
										//
										pc_->back->back->back->value.asmType = ParserAsm::IfArgumentsSymbolNotEqualNumberGoto;
										pc_->back->back->back->value.procedure = InstructionVmIfArgumentsSymbolNotEqualNumberGoto;
										pc_->back->back->back->value.operand = operatorX;
										pc = reinterpret_cast<ProgramCounter *> (pc_->back->back->back);
										//
										instructionList->popFromTail();
										instructionList->popFromTail();
										instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
										printf("%p    [3] if-arguments-symbol-not-equal-number-goto %d %g %p\n", pc, operatorX->symbol, operatorX->value, operatorX->pc);
#endif

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

			if(pc_->value.asmType == ParserAsm::IfFalseGoto) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::OperatorLess) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::PushNumber) {
								if(pc_->back->back->back) {
									if(pc_->back->back->back->value.asmType == ParserAsm::PushSymbol) {
										VariableOperator31 *operatorX = TMemory<VariableOperator31>::newMemory();
										operatorX->symbol = ((VariableSymbol *)(pc_->back->back->back->value.operand.value()))->value;
										operatorX->value = ((VariableNumber *)(pc_->back->back->value.operand.value()))->value;
										operatorX->pc = ((VariableVmProgramCounter *)(pc_->value.operand.value()))->value;
										//
										pc_->back->back->back->value.asmType = ParserAsm::IfSymbolNotLessNumberGoto;
										pc_->back->back->back->value.procedure = InstructionVmIfSymbolNotLessNumberGoto;
										pc_->back->back->back->value.operand = operatorX;
										pc = reinterpret_cast<ProgramCounter *> (pc_->back->back->back);
										//
										instructionList->popFromTail();
										instructionList->popFromTail();
										instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
										printf("%p    [3] if-symbol-not-less-number-goto %d %g %p\n", pc, operatorX->symbol, operatorX->value, operatorX->pc);
#endif

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

			if(pc_->value.asmType == ParserAsm::IfFalseGoto) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::OperatorLess) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::PushNumber) {
								if(pc_->back->back->back) {
									if(pc_->back->back->back->value.asmType == ParserAsm::ArgumentsPushSymbol) {
										VariableOperator31 *operatorX = TMemory<VariableOperator31>::newMemory();
										operatorX->symbol = ((VariableSymbol *)(pc_->back->back->back->value.operand.value()))->value;
										operatorX->value = ((VariableNumber *)(pc_->back->back->value.operand.value()))->value;
										operatorX->pc = ((VariableVmProgramCounter *)(pc_->value.operand.value()))->value;
										//
										pc_->back->back->back->value.asmType = ParserAsm::IfArgumentsSymbolNotLessNumberGoto;
										pc_->back->back->back->value.procedure = InstructionVmIfArgumentsSymbolNotLessNumberGoto;
										pc_->back->back->back->value.operand = operatorX;
										pc = reinterpret_cast<ProgramCounter *> (pc_->back->back->back);
										//
										instructionList->popFromTail();
										instructionList->popFromTail();
										instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
										printf("%p    [3] if-arguments-symbol-not-less-number-goto %d %g %p\n", pc, operatorX->symbol, operatorX->value, operatorX->pc);
#endif

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

			if(pc_->value.asmType == ParserAsm::OperatorMul) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushSymbol) {

								// X==Y > operator-pow2-local-variables-symbol X
								if(((VariableSymbol *)(pc_->back->back->value.operand.value()))->value ==
									((VariableSymbol *)(pc_->back->value.operand.value()))->value) {
									//
									pc_->back->back->value.asmType = ParserAsm::OperatorPow2LocalVariablesSymbol;
									pc_->back->back->value.procedure = InstructionVmOperatorPow2LocalVariablesSymbol;
									pc_->back->back->value.operand = pc_->back->value.operand;
									pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);
									//
									instructionList->popFromTail();
									instructionList->popFromTail();


#ifdef QUANTUM_SCRIPT_DEBUG_ASM
									printf("%p    [2] operator-pow2-local-variables-symbol-2 %d\n", pc, ((VariableSymbol *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->value);
#endif
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
								pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);
								//
								instructionList->popFromTail();
								instructionList->popFromTail();


#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								printf("%p    [2] operator-mul-local-variables-symbol-2 %d %d\n", pc, operatorX->symbol1, operatorX->symbol2);
#endif
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

			if(pc_->value.asmType == ParserAsm::IfFalseGoto) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::OperatorGreater) {

						pc_->back->value.asmType = ParserAsm::IfNotGreaterGoto;
						pc_->back->value.procedure = InstructionVmIfNotGreaterGoto;
						pc_->back->value.operand = pc_->value.operand;
						pc_->value.operand = nullptr;
						pc = reinterpret_cast<ProgramCounter *> (pc_->back);
						instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    [1] if-not-greater %p\n", pc, ((VariableVmProgramCounter *)(((TDoubleEndedQueue<InstructionX>::Node *)pc)->value.operand.value()))->value);
#endif

						return pc;


					};
				};
			};


			// 1. x-call
			// 2. return
			// - translate to -
			// 1. x-tail-call

			if(pc_->value.asmType == ParserAsm::Return) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::XCall) {

						pc_->back->value.asmType = ParserAsm::XTailCall;
						pc_->back->value.procedure = InstructionVmXTailCall;
						pc = reinterpret_cast<ProgramCounter *> (pc_->back);
						instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    [1] x-tail-call\n", pc);
#endif

						return pc;

					};
				};

			};


			// 1. x-call-this
			// 2. return
			// - translate to -
			// 1. x-tail-call-this

			if(pc_->value.asmType == ParserAsm::Return) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::XCallThis) {

						pc_->back->value.asmType = ParserAsm::XTailCallThis;
						pc_->back->value.procedure = InstructionVmXTailCallThis;
						pc = reinterpret_cast<ProgramCounter *> (pc_->back);
						instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    [1] x-tail-call-this\n", pc);
#endif

						return pc;

					};
				};

			};


			// 1. x-call-this-mode-call
			// 2. return
			// - translate to -
			// 1. x-tail-call-this-mode-call

			if(pc_->value.asmType == ParserAsm::Return) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::XCallThisModeCall) {

						pc_->back->value.asmType = ParserAsm::XTailCallThisModeCall;
						pc_->back->value.procedure = InstructionVmXTailCallThisModeCall;
						pc = reinterpret_cast<ProgramCounter *> (pc_->back);
						instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    [1] x-tail-call-this-mode-call\n", pc);
#endif

						return pc;

					};
				};

			};

			// 1. x-call-this-mode-apply
			// 2. return
			// - translate to -
			// 1. x-tail-call-this-mode-apply

			if(pc_->value.asmType == ParserAsm::Return) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::XCallThisModeApply) {

						pc_->back->value.asmType = ParserAsm::XTailCallThisModeApply;
						pc_->back->value.procedure = InstructionVmXTailCallThisModeApply;
						pc = reinterpret_cast<ProgramCounter *> (pc_->back);
						instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    [1] x-tail-call-this-mode-apply\n", pc);
#endif

						return pc;

					};
				};


			};

			// 1. x-call-with-this-reference X
			// 2. return
			// - translate to -
			// 1. x-call-with-this-reference X

			if(pc_->value.asmType == ParserAsm::Return) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::XCallWithThisReference) {

						pc_->back->value.asmType = ParserAsm::XTailCallWithThisReference;
						pc_->back->value.procedure = InstructionVmXTailCallWithThisReference;
						pc = reinterpret_cast<ProgramCounter *> (pc_->back);
						instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    [1] x-tail-call-with-this-reference %d\n", pc, ((VariableSymbol *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->value);
#endif

						return pc;

					};
				};

			};

			// 1. x-call-symbol X
			// 2. return
			// - translate to -
			// 1. x-tail-call-symbol X

			if(pc_->value.asmType == ParserAsm::Return) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::XCallSymbol) {

						pc_->back->value.asmType = ParserAsm::XTailCallSymbol;
						pc_->back->value.procedure = InstructionVmXTailCallSymbol;
						pc = reinterpret_cast<ProgramCounter *> (pc_->back);
						instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
						printf("%p    [1] x-tail-call-symbol %d\n", pc, ((VariableSymbol *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->value);
#endif

						return pc;

					};
				};
			};

			// 1. local-variables-object-reference X
			// 2. operator-pow2-local-variables-symbol Y
			// 3. assign
			// - translate to -
			// 1. assign-local-variables-object-reference-pow2-local-variables-symbol X Y

			if(pc_->value.asmType == ParserAsm::Assign) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::OperatorPow2LocalVariablesSymbol) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushObjectReference) {

								VariableOperator22 *operatorX = TMemory<VariableOperator22>::newMemory();
								operatorX->symbol1 = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
								operatorX->symbol2 = ((VariableSymbol *)(pc_->back->value.operand.value()))->value;

								pc_->back->back->value.asmType = ParserAsm::AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol;
								pc_->back->back->value.procedure = InstructionVmAssignLocalVariablesObjectReferencePow2LocalVariablesSymbol;
								pc_->back->back->value.operand = operatorX;
								pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);

								instructionList->popFromTail();
								instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								printf("%p    [2] assign-local-variables-object-reference-pow2-local-variables-symbol %d %d\n", pc,
									((VariableOperator22 *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->symbol1,
									((VariableOperator22 *)((reinterpret_cast<TDoubleEndedQueue<InstructionX>::Node *>(pc))->value.operand.value()))->symbol2
								);
#endif

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

			if(pc_->value.asmType == ParserAsm::Pop1) {

				if(pc_->back) {
					if(pc_->back->value.asmType == ParserAsm::OperatorPlusPlusLeft) {
						if(pc_->back->back) {
							if(pc_->back->back->value.asmType == ParserAsm::LocalVariablesPushObjectReference) {
#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								uint32_t symbol_ = ((VariableSymbol *)(pc_->back->back->value.operand.value()))->value;
#endif
								pc_->back->back->value.asmType = ParserAsm::LocalVariablesPlusPlus;
								pc_->back->back->value.procedure = InstructionVmLocalVariablesPlusPlus;
								pc = reinterpret_cast<ProgramCounter *> (pc_->back->back);
								//
								instructionList->popFromTail();
								instructionList->popFromTail();

#ifdef QUANTUM_SCRIPT_DEBUG_ASM
								printf("%p    [2] local-variables-plus-plus %d\n", pc, symbol_);
#endif

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
};


