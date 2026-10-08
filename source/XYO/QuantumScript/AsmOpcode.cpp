// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript/AsmOpcode.hpp>
#include <XYO/QuantumScript/Instruction.hpp>

namespace XYO::QuantumScript {

	// Indexed by opcode, must follow ParserAsm order
	static constexpr AsmOpcodeInfo opcodeTable[] = {
		{ParserAsm::Unknown, "unknown", nullptr, AsmOperand::None},
		{ParserAsm::Nop, "nop", InstructionVmNop, AsmOperand::None},
		{ParserAsm::Mark, "mark", InstructionVmMark, AsmOperand::None},
		{ParserAsm::PushUndefined, "push-undefined", InstructionVmPushUndefined, AsmOperand::None},
		{ParserAsm::PushSymbol, "push-symbol", InstructionVmPushSymbol, AsmOperand::Symbol},
		{ParserAsm::PushNumber, "push-number", InstructionVmPushNumber, AsmOperand::Number},
		{ParserAsm::PushNaN, "push-nan", nullptr, AsmOperand::Number},
		{ParserAsm::PushInfinity, "push-infinity", nullptr, AsmOperand::Number},
		{ParserAsm::OperatorAssign, "operator-assign", InstructionVmOperatorAssign, AsmOperand::None},
		{ParserAsm::OperatorEqual, "operator-equal", InstructionVmOperatorEqual, AsmOperand::None},
		{ParserAsm::OperatorEqualStrict, "operator-equal-strict", InstructionVmOperatorEqualStrict, AsmOperand::None},
		{ParserAsm::OperatorPlus, "operator-plus", InstructionVmOperatorPlus, AsmOperand::None},
		{ParserAsm::OperatorMinus, "operator-minus", InstructionVmOperatorMinus, AsmOperand::None},
		{ParserAsm::OperatorUnaryPlus, "operator-unary-plus", InstructionVmOperatorUnaryPlus, AsmOperand::None},
		{ParserAsm::OperatorMul, "operator-mul", InstructionVmOperatorMul, AsmOperand::None},
		{ParserAsm::OperatorDiv, "operator-div", InstructionVmOperatorDiv, AsmOperand::None},
		{ParserAsm::OperatorMod, "operator-mod", InstructionVmOperatorMod, AsmOperand::None},
		{ParserAsm::OperatorNotEqual, "operator-not-equal", InstructionVmOperatorNotEqual, AsmOperand::None},
		{ParserAsm::OperatorNotEqualStrict, "operator-not-equal-strict", InstructionVmOperatorNotEqualStrict, AsmOperand::None},
		{ParserAsm::OperatorNot, "operator-not", InstructionVmOperatorNot, AsmOperand::None},
		{ParserAsm::OperatorLessOrEqual, "operator-less-or-equal", InstructionVmOperatorLessOrEqual, AsmOperand::None},
		{ParserAsm::OperatorLess, "operator-less", InstructionVmOperatorLess, AsmOperand::None},
		{ParserAsm::OperatorGreaterOrEqual, "operator-greater-or-equal", InstructionVmOperatorGreaterOrEqual, AsmOperand::None},
		{ParserAsm::OperatorGreater, "operator-greater", InstructionVmOperatorGreater, AsmOperand::None},
		{ParserAsm::OperatorLogicalOr, "operator-logical-or", InstructionVmOperatorLogicalOr, AsmOperand::None},
		{ParserAsm::OperatorBitwiseOr, "operator-bitwise-or", InstructionVmOperatorBitwiseOr, AsmOperand::None},
		{ParserAsm::OperatorLogicalAnd, "operator-logical-and", InstructionVmOperatorLogicalAnd, AsmOperand::None},
		{ParserAsm::OperatorBitwiseAnd, "operator-bitwise-and", InstructionVmOperatorBitwiseAnd, AsmOperand::None},
		{ParserAsm::OperatorBitwiseNot, "operator-bitwise-not", InstructionVmOperatorBitwiseNot, AsmOperand::None},
		{ParserAsm::OperatorBitwiseLeftShift, "operator-bitwise-left-shift", InstructionVmOperatorBitwiseLeftShift, AsmOperand::None},
		{ParserAsm::OperatorBitwiseRightShift, "operator-bitwise-right-shift", InstructionVmOperatorBitwiseRightShift, AsmOperand::None},
		{ParserAsm::OperatorBitwiseRightShiftZero, "operator-bitwise-right-shift-zero", InstructionVmOperatorBitwiseRightShiftZero, AsmOperand::None},
		{ParserAsm::OperatorBitwiseXor, "operator-bitwise-xor", InstructionVmOperatorBitwiseXor, AsmOperand::None},
		{ParserAsm::OperatorAssignPlus, "operator-assign-plus", InstructionVmOperatorAssignPlus, AsmOperand::None},
		{ParserAsm::OperatorAssignMinus, "operator-assign-minus", InstructionVmOperatorAssignMinus, AsmOperand::None},
		{ParserAsm::OperatorAssignMod, "operator-assign-mod", InstructionVmOperatorAssignMod, AsmOperand::None},
		{ParserAsm::OperatorAssignDiv, "operator-assign-div", InstructionVmOperatorAssignDiv, AsmOperand::None},
		{ParserAsm::OperatorAssignMul, "operator-assign-mul", InstructionVmOperatorAssignMul, AsmOperand::None},
		{ParserAsm::OperatorAssignBitwiseOr, "operator-assign-bitwise-or", InstructionVmOperatorAssignBitwiseOr, AsmOperand::None},
		{ParserAsm::OperatorAssignBitwiseAnd, "operator-assign-bitwise-and", InstructionVmOperatorAssignBitwiseAnd, AsmOperand::None},
		{ParserAsm::OperatorAssignBitwiseLeftShift, "operator-assign-bitwise-left-shift", InstructionVmOperatorAssignBitwiseLeftShift, AsmOperand::None},
		{ParserAsm::OperatorAssignBitwiseRightShift, "operator-assign-bitwise-right-shift", InstructionVmOperatorAssignBitwiseRightShift, AsmOperand::None},
		{ParserAsm::OperatorAssignBitwiseRightShiftZero, "operator-assign-bitwise-right-shift-zero", InstructionVmOperatorAssignBitwiseRightShiftZero, AsmOperand::None},
		{ParserAsm::OperatorAssignBitwiseXor, "operator-assign-bitwise-xor", InstructionVmOperatorAssignBitwiseXor, AsmOperand::None},
		{ParserAsm::OperatorConditional, "operator-conditional", nullptr, AsmOperand::None},
		{ParserAsm::OperatorIn, "operator-in", InstructionVmOperatorIn, AsmOperand::None},
		{ParserAsm::Reference, "reference", InstructionVmReference, AsmOperand::Symbol},
		{ParserAsm::EnterContext, "enter-context", InstructionVmEnterContext, AsmOperand::None},
		{ParserAsm::LeaveContext, "leave-context", InstructionVmLeaveContext, AsmOperand::None},
		{ParserAsm::IfFalseGoto, "if-false-goto", InstructionVmIfFalseGoto, AsmOperand::ProgramCounter},
		{ParserAsm::Goto, "goto", InstructionVmGoto, AsmOperand::ProgramCounter},
		{ParserAsm::ContextSetBreak, "context-set-break", InstructionVmContextSetBreak, AsmOperand::ProgramCounter},
		{ParserAsm::ContextSetContinue, "context-set-continue", InstructionVmContextSetContinue, AsmOperand::ProgramCounter},
		{ParserAsm::Break, "break", InstructionVmBreak, AsmOperand::None},
		{ParserAsm::Continue, "continue", InstructionVmContinue, AsmOperand::None},
		{ParserAsm::IfTrueGoto, "if-true-goto", InstructionVmIfTrueGoto, AsmOperand::ProgramCounter},
		{ParserAsm::OperatorEqual1, "operator-equal1", InstructionVmOperatorEqual1, AsmOperand::None},
		{ParserAsm::Pop1, "pop1", InstructionVmPop1, AsmOperand::None},
		{ParserAsm::PushNull, "push-null", InstructionVmPushNull, AsmOperand::None},
		{ParserAsm::XPushFunction, "x-push-function", InstructionVmXPushFunction, AsmOperand::Function},
		{ParserAsm::Return, "return", InstructionVmReturn, AsmOperand::None},
		{ParserAsm::ContextSetCatch, "context-set-catch", InstructionVmContextSetCatch, AsmOperand::ProgramCounter},
		{ParserAsm::ContextSetFinally, "context-set-finally", InstructionVmContextSetFinally, AsmOperand::ProgramCounter},
		{ParserAsm::Throw, "throw", InstructionVmThrow, AsmOperand::None},
		{ParserAsm::ContextSetThis, "context-set-this", InstructionVmContextSetThis, AsmOperand::None},
		{ParserAsm::ContextPushThis, "context-push-this", InstructionVmContextPushThis, AsmOperand::None},
		{ParserAsm::PushNewObject, "push-new-object", InstructionVmPushNewObject, AsmOperand::None},
		{ParserAsm::PushString, "push-string", InstructionVmPushString, AsmOperand::String},
		{ParserAsm::ContextSetTryBreak, "context-set-try-break", InstructionVmContextSetTryBreak, AsmOperand::ProgramCounter},
		{ParserAsm::ContextSetTryContinue, "context-set-try-continue", InstructionVmContextSetTryContinue, AsmOperand::ProgramCounter},
		{ParserAsm::ContextSetTryReturn, "context-set-try-return", InstructionVmContextSetTryReturn, AsmOperand::ProgramCounter},
		{ParserAsm::ContextSetTryThrow, "context-set-try-throw", InstructionVmContextSetTryThrow, AsmOperand::ProgramCounter},
		{ParserAsm::EnterFirstContext, "enter-first-context", InstructionVmEnterFirstContext, AsmOperand::None},
		{ParserAsm::Assign, "assign", InstructionVmAssign, AsmOperand::None},
		{ParserAsm::OperatorReference, "operator-reference", InstructionVmOperatorReference, AsmOperand::None},
		{ParserAsm::PushNewArray, "push-new-array", InstructionVmPushNewArray, AsmOperand::None},
		{ParserAsm::ArrayPush, "array-push", InstructionVmArrayPush, AsmOperand::None},
		{ParserAsm::OperatorArrayPush, "operator-array-push", InstructionVmOperatorArrayPush, AsmOperand::None},
		{ParserAsm::PushObjectReference, "push-object-reference", InstructionVmPushObjectReference, AsmOperand::Symbol},
		{ParserAsm::ReferenceObjectReference, "reference-object-reference", InstructionVmReferenceObjectReference, AsmOperand::Symbol},
		{ParserAsm::OperatorInstanceOf, "operator-instance-of", InstructionVmOperatorInstanceOf, AsmOperand::None},
		{ParserAsm::OperatorTypeOf, "operator-type-of", InstructionVmOperatorTypeOf, AsmOperand::None},
		{ParserAsm::OperatorReferenceReference, "operator-reference-reference", InstructionVmOperatorReferenceReference, AsmOperand::None},
		{ParserAsm::OperatorPlusPlusLeft, "operator-plus-plus-left", InstructionVmOperatorPlusPlusLeft, AsmOperand::None},
		{ParserAsm::OperatorPlusPlusRight, "operator-plus-plus-right", InstructionVmOperatorPlusPlusRight, AsmOperand::None},
		{ParserAsm::OperatorMinusMinusLeft, "operator-minus-minus-left", InstructionVmOperatorMinusMinusLeft, AsmOperand::None},
		{ParserAsm::OperatorMinusMinusRight, "operator-minus-minus-right", InstructionVmOperatorMinusMinusRight, AsmOperand::None},
		{ParserAsm::XPushNewArray, "x-push-new-array", InstructionVmXPushNewArray, AsmOperand::None},
		{ParserAsm::XArrayPushWithTransfer, "x-array-push-with-transfer", InstructionVmXArrayPushWithTransfer, AsmOperand::None},
		{ParserAsm::XCall, "x-call", InstructionVmXCall, AsmOperand::None},
		{ParserAsm::XCallThis, "x-call-this", InstructionVmXCallThis, AsmOperand::None},
		{ParserAsm::XCallThisModeCall, "x-call-this-mode-call", InstructionVmXCallThisModeCall, AsmOperand::None},
		{ParserAsm::XCallThisModeApply, "x-call-this-mode-apply", InstructionVmXCallThisModeApply, AsmOperand::None},
		{ParserAsm::XCallWithThisReference, "x-call-with-this-reference", InstructionVmXCallWithThisReference, AsmOperand::Symbol},
		{ParserAsm::XCallSymbol, "x-call-symbol", InstructionVmXCallSymbol, AsmOperand::Symbol},
		{ParserAsm::AssignReverse, "assign-reverse", InstructionVmAssignReverse, AsmOperand::None},
		{ParserAsm::Duplicate, "duplicate", InstructionVmDuplicate, AsmOperand::None},
		{ParserAsm::PushBoolean, "push-boolean", InstructionVmPushBoolean, AsmOperand::Boolean},
		{ParserAsm::OperatorAssignXPrototype, "operator-assign-x-prototype", InstructionVmOperatorAssignXPrototype, AsmOperand::None},
		{ParserAsm::ArgumentsPushObjectReference, "arguments-push-object-reference", InstructionVmArgumentsPushObjectReference, AsmOperand::Index},
		{ParserAsm::ArgumentsPushSymbol, "arguments-push-symbol", InstructionVmArgumentsPushSymbol, AsmOperand::Index},
		{ParserAsm::AssignNewObject, "assign-new-object", InstructionVmAssignNewObject, AsmOperand::None},
		{ParserAsm::Catch, "catch", InstructionVmCatch, AsmOperand::None},
		{ParserAsm::ArgumentsLevelPushObjectReference, "arguments-level-push-object-reference", InstructionVmArgumentsLevelPushObjectReference, AsmOperand::IndexLevel},
		{ParserAsm::ArgumentsLevelPushSymbol, "arguments-level-push-symbol", InstructionVmArgumentsLevelPushSymbol, AsmOperand::IndexLevel},
		{ParserAsm::LocalVariablesPushObjectReference, "local-variables-push-object-reference", InstructionVmLocalVariablesPushObjectReference, AsmOperand::Index},
		{ParserAsm::LocalVariablesPushSymbol, "local-variables-push-symbol", InstructionVmLocalVariablesPushSymbol, AsmOperand::Index},
		{ParserAsm::LocalVariablesLevelPushObjectReference, "local-variables-level-push-object-reference", InstructionVmLocalVariablesLevelPushObjectReference, AsmOperand::IndexLevel},
		{ParserAsm::LocalVariablesLevelPushSymbol, "local-variables-level-push-symbol", InstructionVmLocalVariablesLevelPushSymbol, AsmOperand::IndexLevel},
		{ParserAsm::EndExecution, "end-execution", InstructionVmEndExecution, AsmOperand::None},
		{ParserAsm::Yield, "yield", InstructionVmYield, AsmOperand::None},
		{ParserAsm::ContextSetReference, "context-set-reference", InstructionVmContextSetReference, AsmOperand::None},
		{ParserAsm::OperatorSetReferenceIndexKey, "operator-set-reference-index-key", InstructionVmOperatorSetReferenceIndexKey, AsmOperand::None},
		{ParserAsm::OperatorNextReferenceIndex, "operator-next-reference-index", InstructionVmOperatorNextReferenceIndex, AsmOperand::None},
		{ParserAsm::OperatorSetReferenceIndexValue, "operator-set-reference-index-value", InstructionVmOperatorSetReferenceIndexValue, AsmOperand::None},
		{ParserAsm::ContextSetRegisterValue, "context-set-register-value", InstructionVmContextSetRegisterValue, AsmOperand::None},
		{ParserAsm::ContextPushRegisterValue, "context-push-register-value", InstructionVmContextPushRegisterValue, AsmOperand::None},
		{ParserAsm::ContextPushSelf, "context-push-self", InstructionVmContextPushSelf, AsmOperand::None},
		{ParserAsm::CurrentContextExit, "current-context-exit", InstructionVmCurrentContextExit, AsmOperand::None},
		{ParserAsm::PushArguments, "push-arguments", InstructionVmPushArguments, AsmOperand::None},
		{ParserAsm::InstructionListExtractAndDelete, "instruction-list-extract-and-delete", InstructionVmInstructionListExtractAndDelete, AsmOperand::ProgramCounterRange},
		{ParserAsm::OperatorReferenceDeleteIndex, "operator-reference-delete-index", InstructionVmOperatorReferenceDeleteIndex, AsmOperand::None},
		{ParserAsm::OperatorReferenceDeleteReference, "operator-reference-delete-reference", InstructionVmOperatorReferenceDeleteReference, AsmOperand::Symbol},
		{ParserAsm::SymbolPlusPlus, "symbol-plus-plus", InstructionVmSymbolPlusPlus, AsmOperand::Symbol},
		{ParserAsm::OperatorMinusArgumentsSymbolXNumber, "operator-minus-arguments-symbol-x-number", InstructionVmOperatorMinusArgumentsSymbolXNumber, AsmOperand::IndexNumber},
		{ParserAsm::OperatorMinusLocalVariablesSymbol2, "operator-minus-local-variables-symbol-2", InstructionVmOperatorMinusLocalVariablesSymbol2, AsmOperand::IndexIndex},
		{ParserAsm::OperatorPlusLocalVariablesSymbol2, "operator-plus-local-variables-symbol-2", InstructionVmOperatorPlusLocalVariablesSymbol2, AsmOperand::IndexIndex},
		{ParserAsm::IfArgumentsSymbolNotEqualNumberGoto, "if-arguments-symbol-not-equal-number-goto", InstructionVmIfArgumentsSymbolNotEqualNumberGoto, AsmOperand::IndexNumberGoto},
		{ParserAsm::IfSymbolNotLessNumberGoto, "if-symbol-not-less-number-goto", InstructionVmIfSymbolNotLessNumberGoto, AsmOperand::SymbolNumberGoto},
		{ParserAsm::IfArgumentsSymbolNotLessNumberGoto, "if-arguments-symbol-not-less-number-goto", InstructionVmIfArgumentsSymbolNotLessNumberGoto, AsmOperand::IndexNumberGoto},
		{ParserAsm::OperatorMulLocalVariablesSymbol2, "operator-mul-local-variables-symbol-2", InstructionVmOperatorMulLocalVariablesSymbol2, AsmOperand::IndexIndex},
		{ParserAsm::IfNotGreaterGoto, "if-not-greater-goto", InstructionVmIfNotGreaterGoto, AsmOperand::ProgramCounter},
		{ParserAsm::ContextSetStack, "context-set-stack", InstructionVmContextSetStack, AsmOperand::None},
		{ParserAsm::ClearIncludedFile, "clear-included-file", InstructionVmClearIncludedFile, AsmOperand::None},
		{ParserAsm::OperatorPow2LocalVariablesSymbol, "operator-pow2-local-variables-symbol", InstructionVmOperatorPow2LocalVariablesSymbol, AsmOperand::Index},
		{ParserAsm::XTailCall, "x-tail-call", InstructionVmXTailCall, AsmOperand::None},
		{ParserAsm::XTailCallThis, "x-tail-call-this", InstructionVmXTailCallThis, AsmOperand::None},
		{ParserAsm::XTailCallThisModeCall, "x-tail-call-this-mode-call", InstructionVmXTailCallThisModeCall, AsmOperand::None},
		{ParserAsm::XTailCallThisModeApply, "x-tail-call-this-mode-apply", InstructionVmXTailCallThisModeApply, AsmOperand::None},
		{ParserAsm::XTailCallWithThisReference, "x-tail-call-with-this-reference", InstructionVmXTailCallWithThisReference, AsmOperand::Symbol},
		{ParserAsm::XTailCallSymbol, "x-tail-call-symbol", InstructionVmXTailCallSymbol, AsmOperand::Symbol},
		{ParserAsm::AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol, "assign-local-variables-object-reference-pow2-local-variables-symbol", InstructionVmAssignLocalVariablesObjectReferencePow2LocalVariablesSymbol, AsmOperand::IndexIndex},
		{ParserAsm::LocalVariablesPlusPlus, "local-variables-plus-plus", InstructionVmLocalVariablesPlusPlus, AsmOperand::Index},
		{ParserAsm::PushGlobal, "push-global", InstructionVmPushGlobal, AsmOperand::None},
		{ParserAsm::ContextSetPC, "context-set-pc", InstructionVmContextSetPC, AsmOperand::ProgramCounter},
		{ParserAsm::CallNative, "call-native", InstructionVmCallNative, AsmOperand::Native},
		{ParserAsm::SetFunction, "set-function", InstructionVmSetFunction, AsmOperand::Native},
		{ParserAsm::ContextSetTryFinally, "context-set-try-finally", InstructionVmContextSetTryFinally, AsmOperand::ProgramCounter},
		{ParserAsm::ContextSetFinallyNext, "context-set-finally-next", InstructionVmContextSetFinallyNext, AsmOperand::ProgramCounter},
		{ParserAsm::FinallyEnd, "finally-end", InstructionVmFinallyEnd, AsmOperand::None},
	};

	static constexpr bool isOpcodeTableValid() {
		if (sizeof(opcodeTable) / sizeof(opcodeTable[0]) != ParserAsm::End) {
			return false;
		};
		for (int k = 0; k < ParserAsm::End; ++k) {
			if (opcodeTable[k].opcode != k) {
				return false;
			};
		};
		return true;
	};

	static_assert(isOpcodeTableValid(), "opcodeTable must list every opcode in ParserAsm order");

	const AsmOpcodeInfo *AsmOpcode::getInfo(int opcode) {
		if (opcode < 0 || opcode >= ParserAsm::End) {
			return nullptr;
		};
		return &opcodeTable[opcode];
	};

};
