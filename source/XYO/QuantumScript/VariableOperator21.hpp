// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEOPERATOR21_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEOPERATOR21_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableOperator21;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableOperator21> : public TMemoryPoolActive<XYO::QuantumScript::VariableOperator21> {};
};

namespace XYO::QuantumScript {

	class VariableOperator21 : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableOperator21);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableOperator21);

		public:
			Symbol symbol;
			Number value;

			XYO_QUANTUMSCRIPT_EXPORT VariableOperator21();

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
