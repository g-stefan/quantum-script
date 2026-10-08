// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_RUN_HPP
#define XYO_QUANTUMSCRIPT_NATIVERUNTIME_RUN_HPP

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_COROUTINE_HPP
#	include <XYO/QuantumScript.NativeRuntime/Coroutine.hpp>
#endif

namespace XYO::QuantumScript::NativeRuntime {

	// The module for the engine of the current thread (after ExecutiveX::initExecutive):
	// the named units are set as include functions (Script.include(name)),
	// returns the main unit, a function to call with this = Script.
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT TPointer<Variable> load(const ModuleInfo &info);

	// Loads and calls the main unit with this = Script, as the interpreter runs a script.
	// On an uncaught error returns false: error is "Error: message", stackTrace
	// the stack trace. Script.exit is not an error, see ExecutiveX::getExitCode.
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT bool execute(const ModuleInfo &info, String &error, String &stackTrace);

	// Init the engine, execute, the error and the stack trace on stdout,
	// end the engine. Returns the exit code.
	XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT int run(const ModuleInfo &info, int cmdN, char *cmdS[], QuantumScriptInitExecutiveProc initExecutive);

};

#endif
