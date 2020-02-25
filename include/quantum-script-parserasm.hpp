//
// Quantum Script
//
// Copyright (c) 2020 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_PARSERASM_HPP
#define QUANTUM_SCRIPT_PARSERASM_HPP

namespace Quantum {
	namespace Script {


		class ParserAsm {
			public:

				enum {
					Unknown = 0x00,
					Nop = 0x01,
					Mark = 0x02,
					PushUndefined = 0x03,
					PushSymbol = 0x04,
					PushNumber = 0x05,
					PushNaN = 0x06,
					PushInfinity = 0x07,
					OperatorAssign = 0x08,
					OperatorEqual = 0x09,
					OperatorEqualStrict = 0x0A,
					OperatorPlus = 0x0B,
					OperatorMinus = 0x0C,
					OperatorUnaryPlus = 0x0D,

					OperatorMul = 0x0E,
					OperatorDiv = 0x0F,
					OperatorMod = 0x10,
					OperatorNotEqual = 0x11,
					OperatorNotEqualStrict = 0x12,
					OperatorNot = 0x13,
					OperatorLessOrEqual = 0x14,
					OperatorLess = 0x15,
					OperatorGreaterOrEqual = 0x16,
					OperatorGreater = 0x17,
					OperatorLogicalOr = 0x18,
					OperatorBitwiseOr = 0x19,
					OperatorLogicalAnd = 0x1A,
					OperatorBitwiseAnd = 0x1B,
					OperatorBitwiseNot = 0x1C,
					OperatorBitwiseLeftShift = 0x1D,
					OperatorBitwiseRightShift = 0x1E,
					OperatorBitwiseRightShiftZero = 0x1F,
					OperatorBitwiseXor = 0x20,

					OperatorAssignPlus = 0x21,
					OperatorAssignMinus = 0x22,
					OperatorAssignMod = 0x23,
					OperatorAssignDiv = 0x24,
					OperatorAssignMul = 0x25,
					OperatorAssignBitwiseOr = 0x26,
					OperatorAssignBitwiseAnd = 0x27,
					OperatorAssignBitwiseLeftShift = 0x28,
					OperatorAssignBitwiseRightShift = 0x29,
					OperatorAssignBitwiseRightShiftZero = 0x2A,
					OperatorAssignBitwiseXor = 0x2B,

					OperatorConditional = 0x2C,
					OperatorIn = 0x2D,

					Reference = 0x2E,
					EnterContext = 0x2F,
					LeaveContext = 0x30,
					IfFalseGoto = 0x31,
					Goto = 0x32,
					ContextSetBreak = 0x33,
					ContextSetContinue = 0x34,
					Break = 0x35,
					Continue = 0x36,
					IfTrueGoto = 0x37,
					OperatorEqual1 = 0x38,
					Pop1 = 0x39,
					PushNull = 0x3A,
					XPushFunction = 0x3B,
					Return = 0x3C,
					ContextSetCatch = 0x3D,
					ContextSetFinally = 0x3E,
					Throw = 0x3F,
					ContextSetThis = 0x40,
					ContextPushThis = 0x41,
					PushNewObject = 0xA0,
					PushString = 0x42,
					ContextSetTryBreak = 0x43,
					ContextSetTryContinue = 0x44,
					ContextSetTryReturn = 0x45,
					ContextSetTryThrow = 0x46,
					EnterFirstContext = 0x47,
					Assign = 0x48,
					OperatorReference = 0x49,
					PushNewArray = 0x4A,
					ArrayPush = 0x4B,
					OperatorArrayPush = 0x4C,
					PushObjectReference = 0x4D,
					ReferenceObjectReference = 0x4E,
					OperatorInstanceOf = 0x4F,
					OperatorTypeOf = 0x51,

					OperatorReferenceReference = 0x52,
					OperatorArrayPushReference = 0x53,
					OperatorObjectReferenceValue = 0x54,
					OperatorPlusPlusLeft = 0x56,
					OperatorPlusPlusRight = 0x57,
					OperatorMinusMinusLeft = 0x58,
					OperatorMinusMinusRight = 0x59,

					XPushNewArray = 0x5A,
					XArrayPushWithTransfer = 0x5B,
					XCall = 0x5C,
					XCallThis = 0x5D,
					XCallThisModeCall = 0x5E,
					XCallThisModeApply = 0x5F,
					XCallWithThisReference = 0x60,
					XCallSymbol = 0x61,
					AssignReverse = 0x62,

					Duplicate = 0x63,
					PushBoolean = 0x64,

					OperatorAssignXPrototype = 0x65,
					ArgumentsPushObjectReference = 0x66,
					ArgumentsPushSymbol = 0x67,

					AssignNewObject = 0x68,
					Catch = 0x69,

					ArgumentsLevelPushObjectReference = 0x6A,
					ArgumentsLevelPushSymbol = 0x6B,

					LocalVariablesPushObjectReference = 0x6C,
					LocalVariablesPushSymbol = 0x6D,
					LocalVariablesLevelPushObjectReference = 0x6E,
					LocalVariablesLevelPushSymbol = 0x6F,
					EndExecution = 0x70,
					Yield = 0x71,

					ContextSetReference = 0x72,
					OperatorSetReferenceIndexKey = 0x73,
					OperatorNextReferenceIndex = 0x74,
					OperatorSetReferenceIndexValue = 0x75,
					ContextSetRegisterValue = 0x76,
					ContextPushRegisterValue = 0x77,

					FunctionHint = 0x78,
					ContextPushSelf = 0x79,
					CurrentContextExit = 0x7A,
					PushArguments = 0x7B,
					InstructionListExtractAndDelete = 0x7C,
					OperatorReferenceDeleteIndex = 0x7D,
					OperatorReferenceDeleteReference = 0x7E,

					//--- optimization
					SymbolPlusPlus = 0x7F,
					OperatorMinusArgumentsSymbolXNumber = 0x80,
					OperatorMinusLocalVariablesSymbol2 = 0x81,
					OperatorPlusLocalVariablesSymbol2 = 0x82,
					IfArgumentsSymbolNotEqualNumberGoto = 0x83,
					IfSymbolNotLessNumberGoto = 0x84,
					IfArgumentsSymbolNotLessNumberGoto = 0x85,
					OperatorMulLocalVariablesSymbol2 = 0x86,
					IfNotGreaterGoto = 0x87,
					//--- stack garbage collect on try/throw/catch/finally
					ContextSetStack = 0x88,
					//---
					ClearIncludedFile = 0x89,
					//--- optimization
					OperatorPow2LocalVariablesSymbol = 0x8A,
					//--- tail call
					XTailCall = 0x8B,
					XTailCallThis = 0x8C,
					XTailCallThisModeCall = 0x8D,
					XTailCallThisModeApply = 0x8E,
					XTailCallWithThisReference = 0x8F,
					XTailCallSymbol = 0x90,
					//--- optimization
					AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol = 0x91,
					LocalVariablesPlusPlus = 0x92,
					//
					PushGlobal = 0x93,
					//
					ContextSetPC = 0x94,
					//
					End = 0x95
				};
		};

	};
};

#endif
