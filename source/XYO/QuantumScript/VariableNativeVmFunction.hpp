// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLENATIVEFUNCTION_HPP
#define XYO_QUANTUMSCRIPT_VARIABLENATIVEFUNCTION_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_INSTRUCTIONX_HPP
#	include <XYO/QuantumScript/InstructionX.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableNativeVmFunction;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableNativeVmFunction> : public TMemoryPoolActive<XYO::QuantumScript::VariableNativeVmFunction> {};
};

namespace XYO::QuantumScript {

	class VariableNativeVmFunction : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableNativeVmFunction);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableNativeVmFunction);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeFunction;

		public:
			InstructionProcedure procedure;
			TPointer<Variable> operand;

			XYO_QUANTUMSCRIPT_EXPORT VariableNativeVmFunction();

			inline void activeDestructor() {
				operand.deleteMemory();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(InstructionProcedure procedure, Variable *operand);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
