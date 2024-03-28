// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLESTRING_HPP
#define XYO_QUANTUMSCRIPT_VARIABLESTRING_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableString;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableString> : public TMemoryPoolActive<XYO::QuantumScript::VariableString> {};
};

namespace XYO::QuantumScript {

	class VariableString : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableString);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableString);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeString;

		public:
			String value;

			XYO_QUANTUMSCRIPT_EXPORT VariableString();

			inline void activeDestructor() {
				value.activeDestructor();
			};

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(String value);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);

			XYO_QUANTUMSCRIPT_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXPORT static void initMemory();

			XYO_QUANTUMSCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

			XYO_QUANTUMSCRIPT_EXPORT bool hasPropertyByVariable(Variable *variable);

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT Number toNumber();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
			XYO_QUANTUMSCRIPT_EXPORT bool isString();
	};

};

#endif
