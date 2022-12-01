// Quantum Script
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_DEPENDENCY_HPP

#include <math.h>

#ifndef XYO_SYSTEM_HPP
#	include <XYO/System.hpp>
#endif

// -- Export

#ifdef XYO_QUANTUMSCRIPT_INTERNAL
#	define XYO_QUANTUMSCRIPT_EXPORT XYO_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_EXPORT XYO_LIBRARY_IMPORT
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
