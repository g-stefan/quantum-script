// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLESYMBOL_HPP
#define XYO_QUANTUMSCRIPT_VARIABLESYMBOL_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_SYMBOLLIST_HPP
#	include <XYO/QuantumScript/SymbolList.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableSymbol;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableSymbol> : public TMemoryPoolActive<XYO::QuantumScript::VariableSymbol> {};
};

namespace XYO::QuantumScript {

	class VariableSymbol : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableSymbol);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableSymbol);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeSymbol;

		public:
			Symbol value;

			XYO_QUANTUMSCRIPT_EXPORT VariableSymbol();

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(Symbol value);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT TPointer<Variable> getPropertyBySymbol(Symbol symbolId);

			XYO_QUANTUMSCRIPT_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

			XYO_QUANTUMSCRIPT_EXPORT String toString();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT Number toNumber();
			XYO_QUANTUMSCRIPT_EXPORT bool isString();
	};

};

#endif
