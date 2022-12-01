// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLENUMBER_HPP
#define XYO_QUANTUMSCRIPT_VARIABLENUMBER_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableNumber;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableNumber> : public TMemoryPoolActive<XYO::QuantumScript::VariableNumber> {};
};

namespace XYO::QuantumScript {

	class VariableNumber : public Variable {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(VariableNumber);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableNumber);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeNumber;

		public:
			Number value;

			XYO_QUANTUMSCRIPT_EXPORT VariableNumber();

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(Number value);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT Number toNumber();
			XYO_QUANTUMSCRIPT_EXPORT String toString();

			XYO_QUANTUMSCRIPT_EXPORT static String toStringX(Number value_);
	};
};

#endif
