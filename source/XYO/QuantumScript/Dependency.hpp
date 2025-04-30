// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_DEPENDENCY_HPP

#ifndef XYO_QUANTUMSCRIPT_CONFIG_HPP
#	include <XYO/QuantumScript/Config.hpp>
#endif

#include <math.h>

#ifndef XYO_SYSTEM_HPP
#	include <XYO/System.hpp>
#endif

// -- Export               
#ifdef QUANTUM_SCRIPT_INTERNAL
#	ifndef XYO_QUANTUMSCRIPT_INTERNAL
#		define XYO_QUANTUMSCRIPT_INTERNAL
#	endif
#endif

#ifdef QUANTUM_SCRIPT_DLL_INTERNAL
#	ifndef XYO_QUANTUMSCRIPT_DLL_INTERNAL
#		define XYO_QUANTUMSCRIPT_DLL_INTERNAL
#	endif
#endif

#ifdef XYO_QUANTUMSCRIPT_DLL_INTERNAL
#	define XYO_QUANTUMSCRIPT_EXPORT XYO_PLATFORM_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_EXPORT XYO_PLATFORM_LIBRARY_IMPORT
#endif
#ifdef XYO_QUANTUMSCRIPT_LIBRARY
#	undef XYO_QUANTUMSCRIPT_EXPORT
#	define XYO_QUANTUMSCRIPT_EXPORT
#endif

// -- Defaults

#ifndef XYO_QUANTUMSCRIPT_DEFAULT_STACK_TRACE_LEVEL
#	define XYO_QUANTUMSCRIPT_DEFAULT_STACK_TRACE_LEVEL 16
#endif

// --

namespace XYO::QuantumScript {
	using namespace XYO::System;
};

#endif
