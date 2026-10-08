// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_CODEUNIT_HPP
#define XYO_QUANTUMSCRIPT_CODEUNIT_HPP

#ifndef XYO_QUANTUMSCRIPT_ASMOPCODE_HPP
#	include <XYO/QuantumScript/AsmOpcode.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PARSERFUNCTIONHINT_HPP
#	include <XYO/QuantumScript/ParserFunctionHint.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PROGRAMCOUNTER_HPP
#	include <XYO/QuantumScript/ProgramCounter.hpp>
#endif

namespace XYO::QuantumScript {

	// Label of an instruction in a CodeUnit: index + 1, 0 is no label
	typedef uint32_t AsmLabel;

	// Instruction of a CodeUnit, jump targets are labels, not addresses.
	// The operand holds the data part only, the linker creates the jump operands:
	// ProgramCounter, Function, ProgramCounterRange, IndexNumberGoto, SymbolNumberGoto.
	class CodeInstruction {
		public:
			int asmType;
			TPointer<Variable> operand;
			AsmLabel target;                 // jump target
			AsmLabel targetRange;            // Function: end, ProgramCounterRange: begin
			ProgramCounter *targetExternal;  // jump target outside of the unit, overrides target, runtime only
			int functionHint;
			uint32_t sourceSymbol;
			uint32_t sourceLineNumber;
			bool isRemoved;                  // links to a removed instruction resolve to the next instruction

			inline CodeInstruction() {
				reset();
			};

			inline void reset() {
				asmType = ParserAsm::Unknown;
				operand.deleteMemory();
				target = 0;
				targetRange = 0;
				targetExternal = nullptr;
				functionHint = ParserFunctionHint::All; // as VariableVmFunction
				sourceSymbol = 0;
				sourceLineNumber = 0;
				isRemoved = false;
			};

			inline void activeConstructor() {
				reset();
			};

			inline void activeDestructor() {
				operand.deleteMemory();
			};
	};

	class CodeUnit : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(CodeUnit);

		public:
			TDynamicArray<CodeInstruction> instruction;

			XYO_QUANTUMSCRIPT_EXPORT CodeUnit();

			inline size_t length() const {
				return instruction.length();
			};

			inline CodeInstruction &at(AsmLabel label) {
				return instruction.index(label - 1);
			};

			inline AsmLabel lastLabel() const {
				return (AsmLabel)instruction.length();
			};

			// label of the previous instruction that is not removed, 0 if none
			XYO_QUANTUMSCRIPT_EXPORT AsmLabel previous(AsmLabel label);

			XYO_QUANTUMSCRIPT_EXPORT AsmLabel push();
			// remove the last instruction that is not removed and the removed ones after it
			XYO_QUANTUMSCRIPT_EXPORT void pop();
			// remove label and all instructions after it
			XYO_QUANTUMSCRIPT_EXPORT void truncate(AsmLabel label);
			XYO_QUANTUMSCRIPT_EXPORT void empty();

			// append the instructions of unit, labels are relocated
			// returns the label of the first appended instruction, 0 if unit is empty
			XYO_QUANTUMSCRIPT_EXPORT AsmLabel append(CodeUnit &unit);
	};

};

#endif
