// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEBOOLEAN_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEBOOLEAN_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableBoolean;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableBoolean> : public TMemoryPoolActive<XYO::QuantumScript::VariableBoolean> {};
};

namespace XYO::QuantumScript {

	class VariableBoolean : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableBoolean);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableBoolean);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTrue;
			XYO_QUANTUMSCRIPT_EXPORT static const char *strFalse;
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeBoolean;

		public:
			Boolean value;

			XYO_QUANTUMSCRIPT_EXPORT VariableBoolean();

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(bool value);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT Variable *instancePrototype();

			XYO_QUANTUMSCRIPT_EXPORT Variable *clone(SymbolList &inSymbolList);

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT Number toNumber();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
