// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEARGUMENTLEVEL_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEARGUMENTLEVEL_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableArgumentLevel;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableArgumentLevel> : public TMemoryPoolActive<XYO::QuantumScript::VariableArgumentLevel> {};
};

namespace XYO::QuantumScript {

	class VariableArgumentLevel : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableArgumentLevel);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableArgumentLevel);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeArgumentLevel;

		public:
			int value;
			int level;

			XYO_QUANTUMSCRIPT_EXPORT VariableArgumentLevel();

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(int value, int level);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
