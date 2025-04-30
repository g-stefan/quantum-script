// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEOPERATOR31_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEOPERATOR31_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PROGRAMCOUNTER_HPP
#	include <XYO/QuantumScript/ProgramCounter.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableOperator31;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableOperator31> : public TMemoryPoolActive<XYO::QuantumScript::VariableOperator31> {};
};

namespace XYO::QuantumScript {

	class VariableOperator31 : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableOperator31);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableOperator31);

		public:
			Symbol symbol;
			Number value;
			ProgramCounter *pc;

			XYO_QUANTUMSCRIPT_EXPORT VariableOperator31();

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
