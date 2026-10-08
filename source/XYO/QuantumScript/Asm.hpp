// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_ASM_HPP
#define XYO_QUANTUMSCRIPT_ASM_HPP

#ifndef XYO_QUANTUMSCRIPT_DEPENDENCY_HPP
#	include <XYO/QuantumScript/Dependency.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_INSTRUCTION_HPP
#	include <XYO/QuantumScript/Instruction.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_LINKER_HPP
#	include <XYO/QuantumScript/Linker.hpp>
#endif

namespace XYO::QuantumScript {

	// Assemble into a CodeUnit, the code is added to an instruction list by link
	class Asm : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Asm);

		protected:
			bool lastIsMark_;

			AsmLabel newInstruction_();
			void setInstruction_(AsmLabel pc, int type_, Variable *operand, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			AsmLabel assembleProgramCounter_(int type_, AsmLabel value, ProgramCounter *valueExternal, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			void linkProgramCounter_(AsmLabel old_, AsmLabel new_, ProgramCounter *newExternal);

		public:
			TPointer<CodeUnit> code;

			XYO_QUANTUMSCRIPT_EXPORT Asm();

			XYO_QUANTUMSCRIPT_EXPORT AsmLabel assemble(int type_, const char *name_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			XYO_QUANTUMSCRIPT_EXPORT AsmLabel assembleX(int type_, const char *name_, const char *nameX_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			XYO_QUANTUMSCRIPT_EXPORT AsmLabel assembleProgramCounter(int type_, AsmLabel value, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			XYO_QUANTUMSCRIPT_EXPORT void linkProgramCounter(AsmLabel old_, AsmLabel new_);
			XYO_QUANTUMSCRIPT_EXPORT void linkProgramCounterEnd(AsmLabel old_, AsmLabel new_);
			XYO_QUANTUMSCRIPT_EXPORT void linkProgramCounterSource(AsmLabel pc, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			XYO_QUANTUMSCRIPT_EXPORT void removeProgramCounter(AsmLabel pc);

			// jump to an instruction already linked, outside of the unit (runtime only, not serializable)
			XYO_QUANTUMSCRIPT_EXPORT AsmLabel assembleProgramCounterExternal(int type_, ProgramCounter *value, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			XYO_QUANTUMSCRIPT_EXPORT void linkProgramCounterExternal(AsmLabel old_, ProgramCounter *new_);

			// CallNative, SetFunction - operand is a native object bound by the runtime
			XYO_QUANTUMSCRIPT_EXPORT AsmLabel assembleNative(int type_, Variable *operand);
			// fn is the XPushFunction instruction of the function
			XYO_QUANTUMSCRIPT_EXPORT void setFunctionHint(AsmLabel fn, int hint);

#ifdef XYO_QUANTUMSCRIPT_DISABLE_ASM_OPTIMIZER
			inline AsmLabel optimizeCode(AsmLabel pc) {
				return pc;
			};
#else
			AsmLabel optimizeCode(AsmLabel pc);
#endif

			// append the code to the instruction list and start a new unit
			XYO_QUANTUMSCRIPT_EXPORT void link(Linker &linker, InstructionList &instructionList);
			// append unit to the code, returns the label of its first instruction
			XYO_QUANTUMSCRIPT_EXPORT AsmLabel append(CodeUnit &unit);
			// remove the code from label to the end of the unit
			XYO_QUANTUMSCRIPT_EXPORT void discard(AsmLabel label);

			XYO_QUANTUMSCRIPT_EXPORT void endProcessing();
	};

};

#endif
