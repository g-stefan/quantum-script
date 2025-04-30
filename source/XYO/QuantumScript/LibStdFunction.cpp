// Quantum Script
// Copyright (c) 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2016-2025 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <XYO/QuantumScript/LibStdFunction.hpp>

#include <XYO/QuantumScript/LibStdFunction.Source.cpp>

// #define XYO_QUANTUMSCRIPT_DEBUG_RUNTIME

namespace XYO::QuantumScript {

	namespace LibStdFunction {

		void initExecutive(Executive *executive, void *extensionId) {
			executive->compileStringX(libStdFunctionSource);
		};

	};
};
