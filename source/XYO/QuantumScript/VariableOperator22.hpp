// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEOPERATOR22_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEOPERATOR22_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableOperator22;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableOperator22> : public TMemoryPoolActive<XYO::QuantumScript::VariableOperator22> {};
};

namespace XYO::QuantumScript {

	class VariableOperator22 : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableOperator22);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableOperator22);

		public:
			Symbol symbol1;
			Symbol symbol2;

			XYO_QUANTUMSCRIPT_EXPORT VariableOperator22();

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
