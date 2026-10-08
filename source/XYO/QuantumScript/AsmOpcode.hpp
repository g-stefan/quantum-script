// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_ASMOPCODE_HPP
#define XYO_QUANTUMSCRIPT_ASMOPCODE_HPP

#ifndef XYO_QUANTUMSCRIPT_INSTRUCTIONX_HPP
#	include <XYO/QuantumScript/InstructionX.hpp>
#endif

namespace XYO::QuantumScript {

	// Operand layout of an opcode, describes what the instruction operand holds.
	// Symbol is a name interned in Context, Index is an argument / local variable index.
	class AsmOperand {
		public:
			enum {
				None,
				Symbol,              // VariableSymbol, symbol
				Index,               // VariableSymbol, argument / local variable index
				Boolean,             // VariableBoolean
				Number,              // VariableNumber
				String,              // VariableString
				IndexLevel,          // VariableArgumentLevel, index + function level
				ProgramCounter,      // VariableVmProgramCounter, jump target
				Function,            // VariableVmFunction, function begin / end
				IndexNumber,         // VariableOperator21, index + number
				IndexIndex,          // VariableOperator22, index + index
				ProgramCounterRange, // VariableOperator23, range begin + jump target
				IndexNumberGoto,     // VariableOperator31, index + number + jump target
				SymbolNumberGoto,    // VariableOperator31, symbol + number + jump target
				Native               // native object, bound by the runtime
			};
	};

	class AsmOpcodeInfo {
		public:
			int opcode;
			const char *name;
			InstructionProcedure procedure; // nullptr for pseudo opcodes
			int operand;
	};

	class AsmOpcode {
		public:
			// nullptr if opcode is out of range
			XYO_QUANTUMSCRIPT_EXPORT static const AsmOpcodeInfo *getInfo(int opcode);
	};

};

#endif
