// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXECUTIVECONTEXTPC_HPP
#define XYO_QUANTUMSCRIPT_EXECUTIVECONTEXTPC_HPP

#ifndef XYO_QUANTUMSCRIPT_DEPENDENCY_HPP
#	include <XYO/QuantumScript/Dependency.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_PROGRAMCOUNTER_HPP
#	include <XYO/QuantumScript/ProgramCounter.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_VARIABLE_HPP
#	include <XYO/QuantumScript/Variable.hpp>
#endif

#ifndef XYO_QUANTUMSCRIPT_ITERATOR_HPP
#	include <XYO/QuantumScript/Iterator.hpp>
#endif

namespace XYO::QuantumScript {

	class ExecutiveContextPc;
};

namespace XYO::ManagedMemory {
	template <>
	class TMemory<XYO::QuantumScript::ExecutiveContextPc> : public TMemoryPoolActive<XYO::QuantumScript::ExecutiveContextPc> {};
};

namespace XYO::QuantumScript {

	class ExecutiveContextFunction;

	class ExecutiveContextPc : public Object {
			XYO_PLATFORM_DISALLOW_COPY_ASSIGN_MOVE(ExecutiveContextPc);

		public:
			ProgramCounter *catch_;
			ProgramCounter *finally_;
			ProgramCounter *break_;
			ProgramCounter *continue_;
			ProgramCounter *tryBreak_;
			ProgramCounter *tryContinue_;
			ProgramCounter *tryReturn_;
			ProgramCounter *tryThrow_;
			ProgramCounter *pc_;
			ProgramCounter *tryFinally_;  // finally of the try, not a call (pc_ is not set)
			ProgramCounter *finallyNext_; // a finally context: continue here at the end of the finally

			size_t stackLink_; // operand stack depth + 1, 0 - not set

			ExecutiveContextFunction *functionContext;

			TPointer<Variable> registerValue;
			TPointer<Iterator> iteratorValue;
			TPointer<Variable> referenceValue;

			inline ExecutiveContextPc() {
				functionContext = nullptr;

				catch_ = nullptr;
				finally_ = nullptr;
				break_ = nullptr;
				continue_ = nullptr;
				tryBreak_ = nullptr;
				tryContinue_ = nullptr;
				tryReturn_ = nullptr;
				tryThrow_ = nullptr;
				pc_ = nullptr;
				tryFinally_ = nullptr;
				finallyNext_ = nullptr;
				stackLink_ = 0;
			};

			inline void activeConstructor() {
				catch_ = nullptr;
				finally_ = nullptr;
				break_ = nullptr;
				continue_ = nullptr;
				tryBreak_ = nullptr;
				tryContinue_ = nullptr;
				tryReturn_ = nullptr;
				tryThrow_ = nullptr;
				pc_ = nullptr;
				tryFinally_ = nullptr;
				finallyNext_ = nullptr;
				stackLink_ = 0;
			};

			inline void activeDestructor() {
				registerValue.deleteMemory();
				iteratorValue.deleteMemory();
				referenceValue.deleteMemory();
			};

			inline static void initMemory() {
				TMemory<Variable>::initMemory();
				TMemory<Iterator>::initMemory();
			};
	};

};

#endif
