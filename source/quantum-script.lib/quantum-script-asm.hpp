//
// Quantum Script
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_ASM_HPP
#define QUANTUM_SCRIPT_ASM_HPP

#ifndef QUANTUM_SCRIPT__DEPENDENCY_HPP
#	include "quantum-script--dependency.hpp"
#endif

#ifndef QUANTUM_SCRIPT_INSTRUCTION_HPP
#	include "quantum-script-instruction.hpp"
#endif

namespace Quantum {
	namespace Script {

		using namespace XYO;

		typedef struct SAsmLink {
				ProgramCounter *old_;
				ProgramCounter *new_;
		} AsmLink;

		class Asm : public Object {
				XYO_DISALLOW_COPY_ASSIGN_MOVE(Asm);

			protected:
				bool lastIsMark_;
				TRedBlackTree<ProgramCounter *, ProgramCounter *> asmLink_;

			public:
				TPointer<InstructionList> instructionList;

				QUANTUM_SCRIPT_EXPORT Asm();

				QUANTUM_SCRIPT_EXPORT void resetLinks();

				QUANTUM_SCRIPT_EXPORT ProgramCounter *assemble(int type_, const char *name_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
				QUANTUM_SCRIPT_EXPORT ProgramCounter *assembleX(int type_, const char *name_, const char *nameX_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
				QUANTUM_SCRIPT_EXPORT ProgramCounter *assembleProgramCounter(int type_, ProgramCounter *pc_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
				QUANTUM_SCRIPT_EXPORT void linkProgramCounter(ProgramCounter *old_, ProgramCounter *new_);
				QUANTUM_SCRIPT_EXPORT void linkProgramCounterEnd(ProgramCounter *old_, ProgramCounter *new_);
				QUANTUM_SCRIPT_EXPORT void linkProgramCounterSource(ProgramCounter *pc_, uint32_t sourceSymbol, uint32_t sourceLineNumber);
				QUANTUM_SCRIPT_EXPORT void removeProgramCounter(ProgramCounter *pc_);

				QUANTUM_SCRIPT_EXPORT ProgramCounter *assembleDirect(InstructionProcedure procedure, Variable *operand);

#ifdef QUANTUM_SCRIPT_DISABLE_ASM_OPTIMIZER
				inline ProgramCounter *optimizeCode(ProgramCounter *pc) {
					return pc;
				};
#else
				ProgramCounter *optimizeCode(ProgramCounter *);
#endif

				QUANTUM_SCRIPT_EXPORT void endProcessing();
		};

	};
};

#endif
