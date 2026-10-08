// Quantum Script
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_APPLICATION_COMPILECPP_HPP
#define XYO_QUANTUMSCRIPT_APPLICATION_COMPILECPP_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

namespace XYO::QuantumScript::Application {

	// Bytecode to C++ for the native runtime (quantum-script.native-runtime).
	// Each script function and the top level code of each unit is a native
	// function (VariableFunction, VariableFunctionWithYield for coroutines):
	// the generated code has no bytecode, nothing is added to the instruction
	// list of the engine. Jumps, loops, try / catch / finally, break, continue
	// are resolved when generating, the operand stack and the local variables
	// are C++ locals, script exceptions are C++ exceptions.
	class CompileCpp {
		public:
			// withInline - simple instructions and expressions are C++ code with
			// number fast paths, else every value is a Variable and every
			// instruction calls the native runtime
			static bool generate(const uint8_t *bytecode, size_t size, const String &sourceName, const String &namespaceName, bool withInline, String &output, String &error);
	};

};

#endif
