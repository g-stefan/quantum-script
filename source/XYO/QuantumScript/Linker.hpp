// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_LINKER_HPP
#define XYO_QUANTUMSCRIPT_LINKER_HPP

#ifndef XYO_QUANTUMSCRIPT_CODEUNIT_HPP
#	include <XYO/QuantumScript/CodeUnit.hpp>
#endif

namespace XYO::QuantumScript {

	// Append a CodeUnit to an instruction list: create the instructions,
	// bind the procedures and resolve the jump targets to addresses.
	class Linker : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Linker);

		protected:
			// address of each instruction of the last linked unit,
			// removed instructions resolve to the next instruction
			TDynamicArray<ProgramCounter *> address;

		public:
			XYO_QUANTUMSCRIPT_EXPORT Linker();

			XYO_QUANTUMSCRIPT_EXPORT void link(CodeUnit &unit, InstructionList &instructionList);

			// address of a label of the last linked unit, nullptr for no label
			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *getProgramCounter(AsmLabel label);

			// The body of a script function as a unit, to run it in another executive
			// (thread): nested functions are included, even if already extracted at runtime.
			// Native instructions, jumps outside of the function and variables of an
			// enclosing function are not allowed.
			XYO_QUANTUMSCRIPT_EXPORT static bool unlinkFunction(Variable *function, CodeUnit &unit, String &error);
	};

};

#endif
