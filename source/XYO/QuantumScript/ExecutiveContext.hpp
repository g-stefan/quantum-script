// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVECONTEXT_HPP
#define XYO_QUANTUMSCRIPT_EXECUTIVECONTEXT_HPP

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVECONTEXTPC_HPP
#	include <XYO/QuantumScript/ExecutiveContextPC.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVECONTEXTFUNCTION_HPP
#	include <XYO/QuantumScript/ExecutiveContextFunction.hpp>
#endif

namespace XYO::QuantumScript {
	class ExecutiveContext;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::ExecutiveContext> : public TMemoryPoolActive<XYO::QuantumScript::ExecutiveContext> {};
};

namespace XYO::QuantumScript {

	class ExecutiveContext : public TStackPointerUnsafe<ExecutiveContextPc, TMemoryPoolActive> {
			XYO_DISALLOW_COPY_ASSIGN_MOVE(ExecutiveContext);

		public:
			inline ExecutiveContext(){};

			inline void enter(ExecutiveContextPc *&out) {
				out = TMemory<ExecutiveContextPc>::newMemory();
				out->functionContext = head->value->functionContext;
				push(out);
			};

			inline void enterMaster(ExecutiveContextFunction *&out) {
				out = TMemory<ExecutiveContextFunction>::newMemory();
				push(out);
			};

			inline void leave(ExecutiveContextPc *&out, ExecutiveContextFunction *&outFunction) {
				pop();
				if (head) {
					out = head->value;
					outFunction = out->functionContext;
				} else {
					out = nullptr;
					outFunction = nullptr;
				};
			};

			inline static void initMemory() {
				TMemory<TStackPointerUnsafe<ExecutiveContextPc, TMemoryPoolActive>>::initMemory();
				TMemory<ExecutiveContextFunction>::initMemory();
			};

			inline void activeDestructor() {
				TStackPointerUnsafe<ExecutiveContextPc, TMemoryPoolActive>::activeDestructor();
			};
	};

};

#endif
