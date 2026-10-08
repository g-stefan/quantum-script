// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_PARSERASM_HPP
#define XYO_QUANTUMSCRIPT_PARSERASM_HPP

namespace XYO::QuantumScript {

	// Opcode values are part of the bytecode format.
	// They are stable across minor versions: append new opcodes before End,
	// never renumber or reuse a value. Only a major version can change them.
	class ParserAsm {
		public:
			enum {
				Unknown = 0,
				Nop = 1,
				Mark = 2,
				PushUndefined = 3,
				PushSymbol = 4,
				PushNumber = 5,
				PushNaN = 6,
				PushInfinity = 7,
				OperatorAssign = 8,
				OperatorEqual = 9,
				OperatorEqualStrict = 10,
				OperatorPlus = 11,
				OperatorMinus = 12,
				OperatorUnaryPlus = 13,

				OperatorMul = 14,
				OperatorDiv = 15,
				OperatorMod = 16,
				OperatorNotEqual = 17,
				OperatorNotEqualStrict = 18,
				OperatorNot = 19,
				OperatorLessOrEqual = 20,
				OperatorLess = 21,
				OperatorGreaterOrEqual = 22,
				OperatorGreater = 23,
				OperatorLogicalOr = 24,
				OperatorBitwiseOr = 25,
				OperatorLogicalAnd = 26,
				OperatorBitwiseAnd = 27,
				OperatorBitwiseNot = 28,
				OperatorBitwiseLeftShift = 29,
				OperatorBitwiseRightShift = 30,
				OperatorBitwiseRightShiftZero = 31,
				OperatorBitwiseXor = 32,

				OperatorAssignPlus = 33,
				OperatorAssignMinus = 34,
				OperatorAssignMod = 35,
				OperatorAssignDiv = 36,
				OperatorAssignMul = 37,
				OperatorAssignBitwiseOr = 38,
				OperatorAssignBitwiseAnd = 39,
				OperatorAssignBitwiseLeftShift = 40,
				OperatorAssignBitwiseRightShift = 41,
				OperatorAssignBitwiseRightShiftZero = 42,
				OperatorAssignBitwiseXor = 43,

				OperatorConditional = 44,
				OperatorIn = 45,

				Reference = 46,
				EnterContext = 47,
				LeaveContext = 48,
				IfFalseGoto = 49,
				Goto = 50,
				ContextSetBreak = 51,
				ContextSetContinue = 52,
				Break = 53,
				Continue = 54,
				IfTrueGoto = 55,
				OperatorEqual1 = 56,
				Pop1 = 57,
				PushNull = 58,
				XPushFunction = 59,
				Return = 60,
				ContextSetCatch = 61,
				ContextSetFinally = 62,
				Throw = 63,
				ContextSetThis = 64,
				ContextPushThis = 65,
				PushNewObject = 66,
				PushString = 67,
				ContextSetTryBreak = 68,
				ContextSetTryContinue = 69,
				ContextSetTryReturn = 70,
				ContextSetTryThrow = 71,
				EnterFirstContext = 72,
				Assign = 73,
				OperatorReference = 74,
				PushNewArray = 75,
				ArrayPush = 76,
				OperatorArrayPush = 77,
				PushObjectReference = 78,
				ReferenceObjectReference = 79,
				OperatorInstanceOf = 80,
				OperatorTypeOf = 81,

				OperatorReferenceReference = 82,
				OperatorPlusPlusLeft = 83,
				OperatorPlusPlusRight = 84,
				OperatorMinusMinusLeft = 85,
				OperatorMinusMinusRight = 86,

				XPushNewArray = 87,
				XArrayPushWithTransfer = 88,
				XCall = 89,
				XCallThis = 90,
				XCallThisModeCall = 91,
				XCallThisModeApply = 92,
				XCallWithThisReference = 93,
				XCallSymbol = 94,
				AssignReverse = 95,

				Duplicate = 96,
				PushBoolean = 97,

				OperatorAssignXPrototype = 98,
				ArgumentsPushObjectReference = 99,
				ArgumentsPushSymbol = 100,

				AssignNewObject = 101,
				Catch = 102,

				ArgumentsLevelPushObjectReference = 103,
				ArgumentsLevelPushSymbol = 104,

				LocalVariablesPushObjectReference = 105,
				LocalVariablesPushSymbol = 106,
				LocalVariablesLevelPushObjectReference = 107,
				LocalVariablesLevelPushSymbol = 108,
				EndExecution = 109,
				Yield = 110,

				ContextSetReference = 111,
				OperatorSetReferenceIndexKey = 112,
				OperatorNextReferenceIndex = 113,
				OperatorSetReferenceIndexValue = 114,
				ContextSetRegisterValue = 115,
				ContextPushRegisterValue = 116,

				ContextPushSelf = 117,
				CurrentContextExit = 118,
				PushArguments = 119,
				InstructionListExtractAndDelete = 120,
				OperatorReferenceDeleteIndex = 121,
				OperatorReferenceDeleteReference = 122,

				//--- optimization
				SymbolPlusPlus = 123,
				OperatorMinusArgumentsSymbolXNumber = 124,
				OperatorMinusLocalVariablesSymbol2 = 125,
				OperatorPlusLocalVariablesSymbol2 = 126,
				IfArgumentsSymbolNotEqualNumberGoto = 127,
				IfSymbolNotLessNumberGoto = 128,
				IfArgumentsSymbolNotLessNumberGoto = 129,
				OperatorMulLocalVariablesSymbol2 = 130,
				IfNotGreaterGoto = 131,
				//--- stack garbage collect on try/throw/catch/finally
				ContextSetStack = 132,
				//---
				ClearIncludedFile = 133,
				//--- optimization
				OperatorPow2LocalVariablesSymbol = 134,
				//--- tail call
				XTailCall = 135,
				XTailCallThis = 136,
				XTailCallThisModeCall = 137,
				XTailCallThisModeApply = 138,
				XTailCallWithThisReference = 139,
				XTailCallSymbol = 140,
				//--- optimization
				AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol = 141,
				LocalVariablesPlusPlus = 142,
				//
				PushGlobal = 143,
				//
				ContextSetPC = 144,
				//--- native, bound by the runtime (not serializable)
				CallNative = 145,
				SetFunction = 146,
				//--- try / finally, the finally is not a call
				ContextSetTryFinally = 147,
				ContextSetFinallyNext = 148,
				FinallyEnd = 149,
				//
				End = 150
			};
	};

};

#endif
