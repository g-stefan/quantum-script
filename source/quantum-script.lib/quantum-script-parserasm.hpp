//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
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
					Unknown,
					Nop,
					Mark,
					PushUndefined,
					PushSymbol,
					PushNumber,
					PushNaN,
					PushInfinity,
					OperatorAssign,
					OperatorEqual,
					OperatorEqualStrict,
					OperatorPlus,
					OperatorMinus,
					OperatorUnaryPlus,

					OperatorMul,
					OperatorDiv,
					OperatorMod,
					OperatorNotEqual,
					OperatorNotEqualStrict,
					OperatorNot,
					OperatorLessOrEqual,
					OperatorLess,
					OperatorGreaterOrEqual,
					OperatorGreater,
					OperatorLogicalOr,
					OperatorBitwiseOr,
					OperatorLogicalAnd,
					OperatorBitwiseAnd,
					OperatorBitwiseNot,
					OperatorBitwiseLeftShift,
					OperatorBitwiseRightShift,
					OperatorBitwiseRightShiftZero,
					OperatorBitwiseXor,

					OperatorAssignPlus,
					OperatorAssignMinus,
					OperatorAssignMod,
					OperatorAssignDiv,
					OperatorAssignMul,
					OperatorAssignBitwiseOr,
					OperatorAssignBitwiseAnd,
					OperatorAssignBitwiseLeftShift,
					OperatorAssignBitwiseRightShift,
					OperatorAssignBitwiseRightShiftZero,
					OperatorAssignBitwiseXor,

					OperatorConditional,
					OperatorIn,

					Reference,
					EnterContext,
					LeaveContext,
					IfFalseGoto,
					Goto,
					ContextSetBreak,
					ContextSetContinue,
					Break,
					Continue,
					IfTrueGoto,
					OperatorEqual1,
					Pop1,
					PushNull,
					XPushFunction,
					Return,
					ContextSetCatch,
					ContextSetFinally,
					Throw,
					ContextSetThis,
					ContextPushThis,
					PushNewObject,
					PushString,
					ContextSetTryBreak,
					ContextSetTryContinue,
					ContextSetTryReturn,
					ContextSetTryThrow,
					EnterFirstContext,
					Assign,
					OperatorReference,
					PushNewArray,
					ArrayPush,
					OperatorArrayPush,
					PushObjectReference,
					ReferenceObjectReference,
					OperatorInstanceOf,
					OperatorTypeOf,

					OperatorReferenceReference,
					OperatorPlusPlusLeft,
					OperatorPlusPlusRight,
					OperatorMinusMinusLeft,
					OperatorMinusMinusRight,

					XPushNewArray,
					XArrayPushWithTransfer,
					XCall,
					XCallThis,
					XCallThisModeCall,
					XCallThisModeApply,
					XCallWithThisReference,
					XCallSymbol,
					AssignReverse,

					Duplicate,
					PushBoolean,

					OperatorAssignXPrototype,
					ArgumentsPushObjectReference,
					ArgumentsPushSymbol,

					AssignNewObject,
					Catch,

					ArgumentsLevelPushObjectReference,
					ArgumentsLevelPushSymbol,

					LocalVariablesPushObjectReference,
					LocalVariablesPushSymbol,
					LocalVariablesLevelPushObjectReference,
					LocalVariablesLevelPushSymbol,
					EndExecution,
					Yield,

					ContextSetReference,
					OperatorSetReferenceIndexKey,
					OperatorNextReferenceIndex,
					OperatorSetReferenceIndexValue,
					ContextSetRegisterValue,
					ContextPushRegisterValue,

					FunctionHint,
					ContextPushSelf,
					CurrentContextExit,
					PushArguments,
					InstructionListExtractAndDelete,
					OperatorReferenceDeleteIndex,
					OperatorReferenceDeleteReference,

					//--- optimization
					SymbolPlusPlus,
					OperatorMinusArgumentsSymbolXNumber,
					OperatorMinusLocalVariablesSymbol2,
					OperatorPlusLocalVariablesSymbol2,
					IfArgumentsSymbolNotEqualNumberGoto,
					IfSymbolNotLessNumberGoto,
					IfArgumentsSymbolNotLessNumberGoto,
					OperatorMulLocalVariablesSymbol2,
					IfNotGreaterGoto,
					//--- stack garbage collect on try/throw/catch/finally
					ContextSetStack,
					//---
					ClearIncludedFile,
					//--- optimization
					OperatorPow2LocalVariablesSymbol,
					//--- tail call
					XTailCall,
					XTailCallThis,
					XTailCallThisModeCall,
					XTailCallThisModeApply,
					XTailCallWithThisReference,
					XTailCallSymbol,
					//--- optimization
					AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol,
					LocalVariablesPlusPlus,
					//
					PushGlobal,
					//
					ContextSetPC,
					//
					End
				};
		};

	};
};

#endif
