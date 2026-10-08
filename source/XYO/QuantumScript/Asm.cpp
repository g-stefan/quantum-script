// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <XYO/QuantumScript/Asm.hpp>
#include <XYO/QuantumScript/ParserAsm.hpp>
#include <XYO/QuantumScript/AsmOpcode.hpp>

#include <XYO/QuantumScript/VariableSymbol.hpp>
#include <XYO/QuantumScript/VariableBoolean.hpp>
#include <XYO/QuantumScript/VariableNumber.hpp>
#include <XYO/QuantumScript/VariableString.hpp>

#include <XYO/QuantumScript/VariableArgumentLevel.hpp>
#include <XYO/QuantumScript/VariableOperator21.hpp>
#include <XYO/QuantumScript/VariableOperator22.hpp>
#include <XYO/QuantumScript/VariableOperator31.hpp>

namespace XYO::QuantumScript {

	Asm::Asm() {
		lastIsMark_ = false;
		code.newMemory();
	};

	void Asm::endProcessing() {
		code->empty();
		lastIsMark_ = false;
	};

	void Asm::link(Linker &linker, InstructionList &instructionList) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("------ link %u\n", (unsigned int)code->length());
#endif
		linker.link(*(code.value()), instructionList);
		code->empty();
		lastIsMark_ = false;
	};

	AsmLabel Asm::append(CodeUnit &unit) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("------ append %u\n", (unsigned int)unit.length());
#endif
		// a pending mark stays as a mark instruction
		lastIsMark_ = false;
		return code->append(unit);
	};

	void Asm::discard(AsmLabel label) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("------ discard %u\n", label);
#endif
		code->truncate(label);
		lastIsMark_ = false;
	};

	// Reuse the pending mark or append a new instruction
	AsmLabel Asm::newInstruction_() {
		if (lastIsMark_) {
			lastIsMark_ = false;
			return code->lastLabel();
		};
		return code->push();
	};

	void Asm::setInstruction_(AsmLabel pc, int type_, Variable *operand, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
		CodeInstruction &instruction = code->at(pc);
		instruction.reset();
		instruction.asmType = type_;
		instruction.operand = operand;
		instruction.sourceSymbol = sourceSymbol;
		instruction.sourceLineNumber = sourceLineNumber;
	};

	// Opcodes that can complete an optimizer pattern
	static bool isOptimizerTrigger(int type_) {
		switch (type_) {
		case ParserAsm::OperatorPlus:
		case ParserAsm::OperatorMinus:
		case ParserAsm::OperatorUnaryPlus:
		case ParserAsm::OperatorMul:
		case ParserAsm::Pop1:
		case ParserAsm::Return:
		case ParserAsm::IfFalseGoto:
			return true;
		default:
			break;
		};
		return false;
	};

	AsmLabel Asm::assemble(int type_, const char *name_, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
		const AsmOpcodeInfo *info = AsmOpcode::getInfo(type_);
		Variable *operand = nullptr;
		Symbol symbolId = 0;

		if (info) {
			switch (info->operand) {
			case AsmOperand::None:
				if (info->procedure == nullptr) {
					info = nullptr;
				};
				break;
			case AsmOperand::Symbol:
				symbolId = Context::getSymbol(name_);
				operand = VariableSymbol::newVariable(symbolId);
				break;
			case AsmOperand::Index: {
				uint32_t index;
				if (sscanf(name_, XYO_QUANTUMSCRIPT_FORMAT_UINT32, &index) != 1) {
					index = 0;
				};
				operand = VariableSymbol::newVariable(index);
			};
				break;
			case AsmOperand::Boolean:
				operand = VariableBoolean::newVariable(StringCore::isEqual(name_, "true"));
				break;
			case AsmOperand::Number: {
				Number valueNumber;
				if (type_ == ParserAsm::PushNaN) {
					valueNumber = NAN;
				} else if (type_ == ParserAsm::PushInfinity) {
					valueNumber = INFINITY;
				} else if (sscanf(name_, XYO_QUANTUMSCRIPT_FORMAT_NUMBER_INPUT, &valueNumber) != 1) {
					valueNumber = 0;
				};
				operand = VariableNumber::newVariable(valueNumber);
			};
				break;
			case AsmOperand::String:
				operand = VariableString::newVariable(name_);
				break;
			default:
				info = nullptr;
				break;
			};
		};

		if (!info) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("       **  asm-unknown %04X:%s\n", type_, name_);
#endif
			return 0;
		};

		AsmLabel pc = newInstruction_();

		// push-nan and push-infinity are push-number
		int asmType = type_;
		if (info->operand == AsmOperand::Number) {
			asmType = ParserAsm::PushNumber;
		};
		setInstruction_(pc, asmType, operand, sourceSymbol, sourceLineNumber);

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		switch (info->operand) {
		case AsmOperand::None:
			printf("%6u    %s\n", pc, info->name);
			break;
		case AsmOperand::Symbol:
			printf("%6u    %s %s : %u\n", pc, info->name, name_, symbolId);
			break;
		case AsmOperand::Number:
			if (type_ != ParserAsm::PushNumber) {
				printf("%6u    %s\n", pc, info->name);
				break;
			};
			printf("%6u    %s %s\n", pc, info->name, name_);
			break;
		default:
			printf("%6u    %s %s\n", pc, info->name, name_);
			break;
		};
#endif

		if (type_ == ParserAsm::Mark) {
			lastIsMark_ = true;
			return pc;
		};

		if (isOptimizerTrigger(type_)) {
			return optimizeCode(pc);
		};
		return pc;
	};

	AsmLabel Asm::assembleX(int type_, const char *name_, const char *nameX_, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
		const AsmOpcodeInfo *info = AsmOpcode::getInfo(type_);
		if (!info || info->operand != AsmOperand::IndexLevel) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("       **  asm-unknown-x %04X:%s:%s\n", type_, name_, nameX_);
#endif
			return 0;
		};

		int value;
		int level;
		if (sscanf(name_, "%d", &value) != 1) {
			value = 0;
		};
		if (sscanf(nameX_, "%d", &level) != 1) {
			level = 0;
		};

		AsmLabel pc = newInstruction_();
		setInstruction_(pc, type_, VariableArgumentLevel::newVariable(value, level), sourceSymbol, sourceLineNumber);

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("%6u    %s %s:%s\n", pc, info->name, name_, nameX_);
#endif
		return pc;
	};

	AsmLabel Asm::assembleProgramCounter_(int type_, AsmLabel value, ProgramCounter *valueExternal, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
		const AsmOpcodeInfo *info = AsmOpcode::getInfo(type_);
		if (info) {
			switch (info->operand) {
			case AsmOperand::ProgramCounter:
				break;
			case AsmOperand::Function:
			case AsmOperand::ProgramCounterRange:
				if (valueExternal) {
					info = nullptr;
				};
				break;
			default:
				info = nullptr;
				break;
			};
		};

		if (!info) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("       **  asm-unknown-pc %04X:%u:%p\n", type_, value, valueExternal);
#endif
			return 0;
		};

		AsmLabel pc = newInstruction_();
		setInstruction_(pc, type_, nullptr, sourceSymbol, sourceLineNumber);
		CodeInstruction &instruction = code->at(pc);
		if (info->operand == AsmOperand::ProgramCounterRange) {
			instruction.targetRange = value;
		} else {
			instruction.target = value;
			instruction.targetExternal = valueExternal;
		};

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		if (valueExternal) {
			printf("%6u    %s external %p\n", pc, info->name, valueExternal);
		} else {
			printf("%6u    %s %u\n", pc, info->name, value);
		};
#endif

		if (isOptimizerTrigger(type_)) {
			return optimizeCode(pc);
		};
		return pc;
	};

	AsmLabel Asm::assembleProgramCounter(int type_, AsmLabel value, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
		return assembleProgramCounter_(type_, value, nullptr, sourceSymbol, sourceLineNumber);
	};

	AsmLabel Asm::assembleProgramCounterExternal(int type_, ProgramCounter *value, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
		return assembleProgramCounter_(type_, 0, value, sourceSymbol, sourceLineNumber);
	};

	AsmLabel Asm::assembleNative(int type_, Variable *operand) {
		const AsmOpcodeInfo *info = AsmOpcode::getInfo(type_);
		if (!info || info->operand != AsmOperand::Native) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("       **  asm-unknown-native %04X\n", type_);
#endif
			return 0;
		};

		AsmLabel pc = newInstruction_();
		setInstruction_(pc, type_, operand, 0, 0);

#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("%6u    %s\n", pc, info->name);
#endif
		return pc;
	};

	void Asm::setFunctionHint(AsmLabel fn, int hint) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("    -- function-hint %u:%04X\n", fn, hint);
#endif
		code->at(fn).functionHint = hint;
	};

	void Asm::linkProgramCounter_(AsmLabel old_, AsmLabel new_, ProgramCounter *newExternal) {
		CodeInstruction &instruction = code->at(old_);
		const AsmOpcodeInfo *info = AsmOpcode::getInfo(instruction.asmType);
		if (info) {
			switch (info->operand) {
			case AsmOperand::ProgramCounter:
			case AsmOperand::Function:
			case AsmOperand::ProgramCounterRange:
			case AsmOperand::IndexNumberGoto:
			case AsmOperand::SymbolNumberGoto:
				instruction.target = new_;
				instruction.targetExternal = newExternal;
				return;
			default:
				break;
			};
		};
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("    **  link-no-target %u\n", old_);
#endif
	};

	void Asm::linkProgramCounter(AsmLabel old_, AsmLabel new_) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("    -- link %u = %u\n", old_, new_);
#endif
		linkProgramCounter_(old_, new_, nullptr);
	};

	void Asm::linkProgramCounterExternal(AsmLabel old_, ProgramCounter *new_) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("    -- link %u = external %p\n", old_, new_);
#endif
		linkProgramCounter_(old_, 0, new_);
	};

	void Asm::linkProgramCounterEnd(AsmLabel old_, AsmLabel new_) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("    -- link-end %u = %u\n", old_, new_);
#endif
		code->at(old_).targetRange = new_;
	};

	void Asm::linkProgramCounterSource(AsmLabel pc, uint32_t sourceSymbol, uint32_t sourceLineNumber) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("    -- link-source %u - %u:%u\n", pc, sourceSymbol, sourceLineNumber);
#endif
		code->at(pc).sourceSymbol = sourceSymbol;
		code->at(pc).sourceLineNumber = sourceLineNumber;
	};

	// The instruction is kept as removed, links to it resolve to the next instruction
	void Asm::removeProgramCounter(AsmLabel pc) {
#ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
		printf("    -- remove %u\n", pc);
#endif
		code->at(pc).isRemoved = true;
	};

#ifdef XYO_QUANTUMSCRIPT_DISABLE_ASM_OPTIMIZER
#else

	static inline Symbol operandSymbol(CodeInstruction &instruction) {
		return ((VariableSymbol *)(instruction.operand.value()))->value;
	};

	static inline Number operandNumber(CodeInstruction &instruction) {
		return ((VariableNumber *)(instruction.operand.value()))->value;
	};

	// Replace the last instructions with a single one, pc is the last instruction
	AsmLabel Asm::optimizeCode(AsmLabel pc) {
		CodeUnit &unit = *(code.value());

		AsmLabel back1 = unit.previous(pc);
		AsmLabel back2 = back1 ? unit.previous(back1) : 0;
		AsmLabel back3 = back2 ? unit.previous(back2) : 0;

		int type0 = unit.at(pc).asmType;
		int type1 = back1 ? unit.at(back1).asmType : ParserAsm::Unknown;
		int type2 = back2 ? unit.at(back2).asmType : ParserAsm::Unknown;
		int type3 = back3 ? unit.at(back3).asmType : ParserAsm::Unknown;

		//  1. arguments-push-symbol X
		//  2. push-number Y
		//  3. operator-minus
		//  - translate to -
		//  1. operator-minus-arguments-symbol-x-number X Y

		if (type0 == ParserAsm::OperatorMinus && type1 == ParserAsm::PushNumber && type2 == ParserAsm::ArgumentsPushSymbol) {
			VariableOperator21 *operatorX = (VariableOperator21 *)VariableOperator21::newVariable();
			operatorX->symbol = operandSymbol(unit.at(back2));
			operatorX->value = operandNumber(unit.at(back1));
			//
			unit.at(back2).asmType = ParserAsm::OperatorMinusArgumentsSymbolXNumber;
			unit.at(back2).operand = operatorX;
			//
			unit.pop();
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [2] operator-minus-arguments-symbol-x-number %u %g\n", back2, operatorX->symbol, operatorX->value);
#	endif

			return back2;
		};

		// 1. local-variables-push-symbol Y
		// 2. local-variables-push-symbol X
		// 3. operator-minus
		// - translate to -
		// 1. operator-minus-local-variables-symbol-2 X Y

		if (type0 == ParserAsm::OperatorMinus && type1 == ParserAsm::LocalVariablesPushSymbol && type2 == ParserAsm::LocalVariablesPushSymbol) {
			VariableOperator22 *operatorX = (VariableOperator22 *)VariableOperator22::newVariable();
			operatorX->symbol2 = operandSymbol(unit.at(back2));
			operatorX->symbol1 = operandSymbol(unit.at(back1));
			//
			unit.at(back2).asmType = ParserAsm::OperatorMinusLocalVariablesSymbol2;
			unit.at(back2).operand = operatorX;
			//
			unit.pop();
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [2] operator-minus-local-variables-symbol-2 %u %u\n", back2, operatorX->symbol1, operatorX->symbol2);
#	endif

			return back2;
		};

		// 1. local-variables-push-symbol Y
		// 2. local-variables-push-symbol X
		// 3. operator-plus
		// - translate to -
		// 1. operator-plus-local-variables-symbol-2 X Y

		if (type0 == ParserAsm::OperatorPlus && type1 == ParserAsm::LocalVariablesPushSymbol && type2 == ParserAsm::LocalVariablesPushSymbol) {
			VariableOperator22 *operatorX = (VariableOperator22 *)VariableOperator22::newVariable();
			operatorX->symbol2 = operandSymbol(unit.at(back2));
			operatorX->symbol1 = operandSymbol(unit.at(back1));
			//
			unit.at(back2).asmType = ParserAsm::OperatorPlusLocalVariablesSymbol2;
			unit.at(back2).operand = operatorX;
			//
			unit.pop();
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [2] operator-plus-local-variables-symbol-2 %u %u\n", back2, operatorX->symbol1, operatorX->symbol2);
#	endif

			return back2;
		};

		// 1. push-number X
		// 2. push-number Y
		// 3. operator-plus
		// - translate to -
		// 1. push-number Z ; (Z=X+Y)

		if (type0 == ParserAsm::OperatorPlus && type1 == ParserAsm::PushNumber && type2 == ParserAsm::PushNumber) {
			Number v1 = operandNumber(unit.at(back2));
			Number v2 = operandNumber(unit.at(back1));
			//
			unit.at(back2).operand = VariableNumber::newVariable(v1 + v2);
			//
			unit.pop();
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [2] push-number " XYO_QUANTUMSCRIPT_FORMAT_NUMBER "\n", back2, (v1 + v2));
#	endif

			return back2;
		};

		//  1. push-object-reference X
		//  2. operator-plus-plus-left / operator-plus-plus-right
		//  3. pop1
		//  - translate to -
		//  1. symbol-plus-plus X

		if (type0 == ParserAsm::Pop1 && (type1 == ParserAsm::OperatorPlusPlusLeft || type1 == ParserAsm::OperatorPlusPlusRight) && type2 == ParserAsm::PushObjectReference) {
			unit.at(back2).asmType = ParserAsm::SymbolPlusPlus;
			//
			unit.pop();
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [2] symbol-plus-plus %u\n", back2, operandSymbol(unit.at(back2)));
#	endif

			return back2;
		};

		//  1. operator-assign
		//  2. pop1
		//  - translate to -
		//  1. assign

		if (type0 == ParserAsm::Pop1 && type1 == ParserAsm::OperatorAssign) {
			unit.at(back1).asmType = ParserAsm::Assign;
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [1] assign\n", back1);
#	endif

			return optimizeCode(back1);
		};

		// 1. local-variables-push-symbol X
		// 2. pop1
		// - translate to - // do not completely remove - branch can jump to undefined pc
		// 1. nop

		if (type0 == ParserAsm::Pop1 && type1 == ParserAsm::LocalVariablesPushSymbol) {
			unit.at(back1).asmType = ParserAsm::Nop;
			unit.at(back1).operand.deleteMemory();
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [1] nop\n", back1);
#	endif

			return back1;
		};

		// 1. arguments-push-symbol X / push-symbol X
		// 2. push-number Y
		// 3. operator-equal / operator-less
		// 4. if-false-goto Z
		// - translate to -
		// 1. if-arguments-symbol-not-equal-number-goto X Y Z
		//    if-symbol-not-less-number-goto X Y Z
		//    if-arguments-symbol-not-less-number-goto X Y Z

		if (type0 == ParserAsm::IfFalseGoto && type2 == ParserAsm::PushNumber) {
			int asmType = ParserAsm::Unknown;
			if (type1 == ParserAsm::OperatorEqual && type3 == ParserAsm::ArgumentsPushSymbol) {
				asmType = ParserAsm::IfArgumentsSymbolNotEqualNumberGoto;
			} else if (type1 == ParserAsm::OperatorLess && type3 == ParserAsm::PushSymbol) {
				asmType = ParserAsm::IfSymbolNotLessNumberGoto;
			} else if (type1 == ParserAsm::OperatorLess && type3 == ParserAsm::ArgumentsPushSymbol) {
				asmType = ParserAsm::IfArgumentsSymbolNotLessNumberGoto;
			};
			if (asmType != ParserAsm::Unknown) {
				VariableOperator31 *operatorX = (VariableOperator31 *)VariableOperator31::newVariable();
				operatorX->symbol = operandSymbol(unit.at(back3));
				operatorX->value = operandNumber(unit.at(back2));
				operatorX->pc = nullptr; // set by the linker
				//
				AsmLabel target = unit.at(pc).target;
				ProgramCounter *targetExternal = unit.at(pc).targetExternal;
				unit.at(back3).asmType = asmType;
				unit.at(back3).operand = operatorX;
				unit.at(back3).target = target;
				unit.at(back3).targetExternal = targetExternal;
				//
				unit.pop();
				unit.pop();
				unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
				printf("%6u    [3] %s %u %g %u\n", back3, AsmOpcode::getInfo(asmType)->name, operatorX->symbol, operatorX->value, target);
#	endif

				return back3;
			};
		};

		// 1. local-variables-push-symbol Y
		// 2. local-variables-push-symbol X
		// 3. operator-mul
		// - translate to -
		// X!=Y > operator-mul-local-variables-symbol-2 X Y
		// X==Y > operator-pow2-local-variables-symbol X

		if (type0 == ParserAsm::OperatorMul && type1 == ParserAsm::LocalVariablesPushSymbol && type2 == ParserAsm::LocalVariablesPushSymbol) {

			// X==Y > operator-pow2-local-variables-symbol X
			if (operandSymbol(unit.at(back2)) == operandSymbol(unit.at(back1))) {
				unit.at(back2).asmType = ParserAsm::OperatorPow2LocalVariablesSymbol;
				unit.at(back2).operand = unit.at(back1).operand;
				//
				unit.pop();
				unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
				printf("%6u    [2] operator-pow2-local-variables-symbol %u\n", back2, operandSymbol(unit.at(back2)));
#	endif
				return back2;
			};

			// X!=Y > operator-mul-local-variables-symbol-2 X Y
			VariableOperator22 *operatorX = (VariableOperator22 *)VariableOperator22::newVariable();
			operatorX->symbol2 = operandSymbol(unit.at(back2));
			operatorX->symbol1 = operandSymbol(unit.at(back1));
			//
			unit.at(back2).asmType = ParserAsm::OperatorMulLocalVariablesSymbol2;
			unit.at(back2).operand = operatorX;
			//
			unit.pop();
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [2] operator-mul-local-variables-symbol-2 %u %u\n", back2, operatorX->symbol1, operatorX->symbol2);
#	endif
			return back2;
		};

		// 1. operator-greater
		// 2. if-false-goto X
		// - translate to -
		// 1. if-not-greater-goto X

		if (type0 == ParserAsm::IfFalseGoto && type1 == ParserAsm::OperatorGreater) {
			AsmLabel target = unit.at(pc).target;
			ProgramCounter *targetExternal = unit.at(pc).targetExternal;
			unit.at(back1).asmType = ParserAsm::IfNotGreaterGoto;
			unit.at(back1).operand.deleteMemory();
			unit.at(back1).target = target;
			unit.at(back1).targetExternal = targetExternal;
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [1] if-not-greater-goto %u\n", back1, target);
#	endif

			return back1;
		};

		// 1. x-call / x-call-this / x-call-this-mode-call / x-call-this-mode-apply
		//    x-call-with-this-reference X / x-call-symbol X
		// 2. return
		// - translate to -
		// 1. x-tail-call ...
		// keep return, tail call can fallback to normal call (coroutine)

		if (type0 == ParserAsm::Return) {
			int asmType = ParserAsm::Unknown;
			switch (type1) {
			case ParserAsm::XCall:
				asmType = ParserAsm::XTailCall;
				break;
			case ParserAsm::XCallThis:
				asmType = ParserAsm::XTailCallThis;
				break;
			case ParserAsm::XCallThisModeCall:
				asmType = ParserAsm::XTailCallThisModeCall;
				break;
			case ParserAsm::XCallThisModeApply:
				asmType = ParserAsm::XTailCallThisModeApply;
				break;
			case ParserAsm::XCallWithThisReference:
				asmType = ParserAsm::XTailCallWithThisReference;
				break;
			case ParserAsm::XCallSymbol:
				asmType = ParserAsm::XTailCallSymbol;
				break;
			default:
				break;
			};
			if (asmType != ParserAsm::Unknown) {
				unit.at(back1).asmType = asmType;

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
				printf("%6u    [1] %s\n", back1, AsmOpcode::getInfo(asmType)->name);
#	endif

				return pc;
			};
		};

		// 1. local-variables-object-reference X
		// 2. operator-pow2-local-variables-symbol Y
		// 3. assign
		// - translate to -
		// 1. assign-local-variables-object-reference-pow2-local-variables-symbol X Y

		if (type0 == ParserAsm::Assign && type1 == ParserAsm::OperatorPow2LocalVariablesSymbol && type2 == ParserAsm::LocalVariablesPushObjectReference) {
			VariableOperator22 *operatorX = (VariableOperator22 *)VariableOperator22::newVariable();
			operatorX->symbol1 = operandSymbol(unit.at(back2));
			operatorX->symbol2 = operandSymbol(unit.at(back1));

			unit.at(back2).asmType = ParserAsm::AssignLocalVariablesObjectReferencePow2LocalVariablesSymbol;
			unit.at(back2).operand = operatorX;

			unit.pop();
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [2] assign-local-variables-object-reference-pow2-local-variables-symbol %u %u\n", back2, operatorX->symbol1, operatorX->symbol2);
#	endif

			return back2;
		};

		//  1. local-variables-object-reference X
		//  2. operator-plus-plus-left
		//  3. pop1
		//  - translate to -
		//  1. local-variables-plus-plus X

		if (type0 == ParserAsm::Pop1 && type1 == ParserAsm::OperatorPlusPlusLeft && type2 == ParserAsm::LocalVariablesPushObjectReference) {
			unit.at(back2).asmType = ParserAsm::LocalVariablesPlusPlus;
			//
			unit.pop();
			unit.pop();

#	ifdef XYO_QUANTUMSCRIPT_DEBUG_ASM
			printf("%6u    [2] local-variables-plus-plus %u\n", back2, operandSymbol(unit.at(back2)));
#	endif

			return back2;
		};

		// ---

		return pc;
	};

#endif

};
