// Quantum Script
// Copyright (c) 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2023 Grigore Stefan <g_stefan@yahoo.com>
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
