// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_INSTRUCTIONX_HPP
#define XYO_QUANTUMSCRIPT_INSTRUCTIONX_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PARSERASM_HPP
#	include <XYO/QuantumScript/ParserAsm.hpp>
#endif

namespace XYO::QuantumScript {

	class InstructionX;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::InstructionX> : public TMemoryPoolActive<XYO::QuantumScript::InstructionX> {};
};

namespace XYO::QuantumScript {

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

			inline InstructionX(InstructionX &&value) {
				procedure = value.procedure;
				operand = std::move(value.operand);
			};

			inline InstructionX &operator=(InstructionX &value) {
				procedure = value.procedure;
				operand = value.operand;				
				return *this;
			};


			inline InstructionX &operator=(InstructionX &&value) {
				procedure = value.procedure;
				operand = std::move(value.operand);
				return *this;
			};
	};

};

#endif
