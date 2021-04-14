//
// Quantum Script
//
// Copyright (c) 2020-2021 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#ifndef QUANTUM_SCRIPT_INSTRUCTIONX_HPP
#define QUANTUM_SCRIPT_INSTRUCTIONX_HPP

#ifndef QUANTUM_SCRIPT_VARIABLE_HPP
#include "quantum-script-variable.hpp"
#endif

#ifndef QUANTUM_SCRIPT_PARSERASM_HPP
#include "quantum-script-parserasm.hpp"
#endif


namespace Quantum {
	namespace Script {

		class InstructionX;

	};
};


namespace XYO {
	namespace ManagedMemory {
		template<>
		class TMemory<Quantum::Script::InstructionX>:
			public TMemoryPoolActive<Quantum::Script::InstructionX> {};
	};
};


namespace Quantum {
	namespace Script {
		using namespace XYO;

		class InstructionX;
		class InstructionContext;
		typedef void (*InstructionProcedure)(InstructionContext *context, Variable *operand);
		typedef TDoubleEndedQueue<InstructionX, TMemoryPoolActive> InstructionList;

		class InstructionX {
			public:
				InstructionProcedure procedure;
				TPointer<Variable> operand;
				int asmType;
				uint32_t sourceSymbol;
				uint32_t sourceLineNumber;

				inline InstructionX() {
					procedure = nullptr;
					operand = nullptr;
					asmType = ParserAsm::Unknown;
					sourceSymbol = 0;
					sourceLineNumber = 0;
				};

				inline ~InstructionX() {
					operand.deleteMemory();
				};

				inline void activeDestructor() {
					operand.deleteMemory();
				};

				inline InstructionX(InstructionX &value) {
					procedure = value.procedure;
					operand = value.operand;
				};

				inline InstructionX &operator=(InstructionX &value) {
					operand = value.operand;
					procedure = value.procedure;
					return *this;
				};

		};

	};
};


#endif
