// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVECONTEXTFUNCTION_HPP
#define XYO_QUANTUMSCRIPT_EXECUTIVECONTEXTFUNCTION_HPP

#ifndef XYO_QUANTUMSCRIPT_DEPENDENCY_HPP
#	include <XYO/QuantumScript/Dependency.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVECONTEXTPC_HPP
#	include <XYO/QuantumScript/ExecutiveContextPC.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLEFUNCTION_HPP
#	include <XYO/QuantumScript/VariableFunction.hpp>
#endif

namespace XYO::QuantumScript {

	class ExecutiveContextFunction;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::ExecutiveContextFunction> : public TMemoryPoolActive<XYO::QuantumScript::ExecutiveContextFunction> {};
};

namespace XYO::QuantumScript {

	class ExecutiveContextFunction : public ExecutiveContextPc {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(ExecutiveContextFunction);

		public:
			TPointerX<Variable> thisFunction_;
			TPointerX<Variable> this_;

			TPointerX<VariableArray> functionArguments;
			TPointerX<VariableArray> functionLocalVariables;

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
			TPointerX<FunctionParent> functionParent;
#endif

			inline ExecutiveContextFunction() {

				functionContext = this;

				thisFunction_.pointerLink(this);
				this_.pointerLink(this);
				functionArguments.pointerLink(this);
				functionLocalVariables.pointerLink(this);

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
				functionParent.pointerLink(this);
#endif
			};

			inline void activeConstructor() {
				ExecutiveContextPc::activeConstructor();
			};

			inline void activeDestructor() {
				ExecutiveContextPc::activeDestructor();
				//
				thisFunction_.deleteMemory();
				this_.deleteMemory();
				functionArguments.deleteMemory();
				functionLocalVariables.deleteMemory();

#ifndef XYO_QUANTUMSCRIPT_DISABLE_CLOSURE
				functionParent.deleteMemory();
#endif
			};

			inline static void initMemory() {
				TMemory<ExecutiveContextPc>::initMemory();
			};
	};

};

#endif
