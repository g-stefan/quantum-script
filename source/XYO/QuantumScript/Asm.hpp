// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_ASM_HPP
#define XYO_QUANTUMSCRIPT_ASM_HPP

#ifndef XYO_QUANTUMSCRIPT_DEPENDENCY_HPP
#	include <XYO/QuantumScript/Dependency.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_INSTRUCTION_HPP
#	include <XYO/QuantumScript/Instruction.hpp>
#endif

namespace XYO::QuantumScript {

	typedef struct SAsmLink {
			ProgramCounter *old_;
			ProgramCounter *new_;
	} AsmLink;

	class Asm : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(Asm);

		protected:
			bool lastIsMark_;
			TRedBlackTree<ProgramCounter *, ProgramCounter *> asmLink_;

		public:
			TPointer<InstructionList> instructionList;

			XYO_QUANTUMSCRIPT_EXPORT Asm();

			XYO_QUANTUMSCRIPT_EXPORT void resetLinks();

			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *assemble(int type_, const char *name_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *assembleX(int type_, const char *name_, const char *nameX_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *assembleProgramCounter(int type_, ProgramCounter *pc_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			XYO_QUANTUMSCRIPT_EXPORT void linkProgramCounter(ProgramCounter *old_, ProgramCounter *new_);
			XYO_QUANTUMSCRIPT_EXPORT void linkProgramCounterEnd(ProgramCounter *old_, ProgramCounter *new_);
			XYO_QUANTUMSCRIPT_EXPORT void linkProgramCounterSource(ProgramCounter *pc_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
			XYO_QUANTUMSCRIPT_EXPORT void removeProgramCounter(ProgramCounter *pc_);

			XYO_QUANTUMSCRIPT_EXPORT ProgramCounter *assembleDirect(InstructionProcedure procedure, Variable *operand);

#ifdef XYO_QUANTUMSCRIPT_DISABLE_ASM_OPTIMIZER
			inline ProgramCounter *optimizeCode(ProgramCounter *pc) {
				return pc;
			};
#else
			ProgramCounter *optimizeCode(ProgramCounter *);
#endif

			XYO_QUANTUMSCRIPT_EXPORT void endProcessing();
	};

};

#endif
