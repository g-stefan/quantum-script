// Quantum Script
// Copyright (c) 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_VARIABLEVMPROGRAMCOUNTER_HPP
#define XYO_QUANTUMSCRIPT_VARIABLEVMPROGRAMCOUNTER_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PROGRAMCOUNTER_HPP
#	include <XYO/QuantumScript/ProgramCounter.hpp>
#endif

namespace XYO::QuantumScript {

	class VariableVmProgramCounter;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::VariableVmProgramCounter> : public TMemoryPoolActive<XYO::QuantumScript::VariableVmProgramCounter> {};
};

namespace XYO::QuantumScript {

	class VariableVmProgramCounter : public Variable {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(VariableVmProgramCounter);
			XYO_DYNAMIC_TYPE_DEFINE(XYO_QUANTUMSCRIPT_EXPORT, VariableVmProgramCounter);

		protected:
			XYO_QUANTUMSCRIPT_EXPORT static const char *strTypeVmProgramCounter;

		public:
			ProgramCounter *value;

			XYO_QUANTUMSCRIPT_EXPORT VariableVmProgramCounter();

			XYO_QUANTUMSCRIPT_EXPORT static Variable *newVariable(ProgramCounter *value);

			XYO_QUANTUMSCRIPT_EXPORT String getVariableType();

			XYO_QUANTUMSCRIPT_EXPORT bool toBoolean();
			XYO_QUANTUMSCRIPT_EXPORT String toString();
	};

};

#endif
