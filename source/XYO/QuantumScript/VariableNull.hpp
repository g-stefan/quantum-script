// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLENULL_HPP
#define XYO_QUANTUMSCRIPT_VARIABLENULL_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableNull;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableNull> : public TMemoryPoolActive<XYO::QuantumScript::VariableNull> {};
};

namespace XYO::QuantumScript {

	class VariableNull : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableNull);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableNull);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeNull;

		public:
			XYO_QUANTUMSCRIPT_EXPORT VariableNull();

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable();

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT Number toNumber();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
