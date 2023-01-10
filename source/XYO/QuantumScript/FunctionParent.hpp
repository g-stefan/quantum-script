// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_FUNCTIONPARENT_HPP
#define XYO_QUANTUMSCRIPT_FUNCTIONPARENT_HPP

#ifndef XYO_QUANTUMSCRIPT_VARIABLEARRAY_HPP
#	include <XYO/QuantumScript/VariableArray.hpp>
#endif

namespace XYO::QuantumScript {

	class FunctionParent;

};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::FunctionParent> : public TMemoryPoolActive<XYO::QuantumScript::FunctionParent> {};
};

namespace XYO::QuantumScript {

	class FunctionParent : public Object {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(FunctionParent);

		public:
			TPointerX<FunctionParent> functionParent;
			TPointerX<VariableArray> variables;
			TPointerX<VariableArray> arguments;

			inline FunctionParent() {
				functionParent.pointerLink(this);
				variables.pointerLink(this);
				arguments.pointerLink(this);
			};

			inline void activeDestructor() {
				functionParent.deleteMemory();
				variables.deleteMemory();
				arguments.deleteMemory();
			};
	};

};

#endif
