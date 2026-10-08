// Quantum Script Native Runtime
// Copyright (c) 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_NATIVERUNTIME_DEPENDENCY_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

#include <XYO/QuantumScript/InstructionContext.hpp>
#include <XYO/QuantumScript/VariableFunction.hpp>
#include <XYO/QuantumScript/VariableFunctionWithYield.hpp>
#include <XYO/QuantumScript/VariableStackTrace.hpp>
#include <XYO/QuantumScript/VariableReferenceIndex.hpp>
#include <XYO/QuantumScript/VariableReferenceSymbol.hpp>
#include <XYO/QuantumScript/VariableReferenceVariable.hpp>
#include <XYO/QuantumScript/Iterator.hpp>

// -- Export

#ifdef QUANTUM_SCRIPT_NATIVE_RUNTIME_DLL_INTERNAL
#	ifndef XYO_QUANTUMSCRIPT_NATIVERUNTIME_INTERNAL
#		define XYO_QUANTUMSCRIPT_NATIVERUNTIME_INTERNAL
#	endif
#endif

#ifdef XYO_QUANTUMSCRIPT_NATIVERUNTIME_INTERNAL
#	define XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_QUANTUMSCRIPT_NATIVERUNTIME_LIBRARY
#	undef XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT
#	define XYO_QUANTUMSCRIPT_NATIVERUNTIME_EXPORT
#endif

// Interface version of the runtime, the generated code checks it.
// Changed when the generated code must be generated again.
#define XYO_QUANTUMSCRIPT_NATIVERUNTIME_ABI 1

#endif
